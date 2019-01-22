#ifndef GenericFunction_h
#define GenericFunction_h

// Worth reading the comments at the top of Field.h  

struct GenericFunction {

  // A single method within the generic function
  struct OneMethod {
    // Data
    seal::reflex::Member function_member;
    const Inserter* inserter;

    // ------------------------------------------------------------
    OneMethod(seal::reflex::Member fn) :
      function_member(fn)
    {
      // The inserter used for any single method is invariant, so find
      // it at method construction time, and store it forever
    
      // Find the type returned by this method
      seal::reflex::Type Reflex_type = this->function_member.type().returnType();
      // Find its inserter
      this->inserter = ::find_inserter(Reflex_type);
    }
  };

  // Data
  std::vector<OneMethod> methods;
  std::string name;
  bool is_static;
  bool is_constructor;
  PyObject* dispatch_table;

  // ------------------------------------------------------------
  // Constructor: Until methods are added, the Generic Function knows
  // nothing besides its name
  GenericFunction(std::string function_name)
    :  name(function_name),
       dispatch_table(PyDict_New())
  {}

  // ------------------------------------------------------------
  // Add a single method to this Generic Function
  void add_method(seal::reflex::Member new_method) {
    this->methods.push_back(OneMethod(new_method));
    // Consider subclassing rather than ifs, for the following.

    // For now assume that if one method is static, then they're all
    // static. I don't see a clean solution to this problem.
    this->is_static = new_method.isStatic();
    this->is_constructor = new_method.isConstructor();
  }

  // ------------------------------------------------------------
  // Calling the generic function involves checking the types of the
  // arguments and parameters and dispatiching to the appropriate
  // method, if there is one.
  PyObject* __call__(PyObject* args);

  // ------------------------------------------------------------
  // Implement the descriptor protocol for turning this fn into a
  // bound/unbound method.
  PyObject* __get__(PyObject *args) {
        // The GenericFunction itself:
    PyObject* func = PyTuple_GetItem(args, 0);
    // Static functions need no binding instance or class binding,
    // return them unaltered.
    if (this->is_static) { 
      Py_INCREF(func);
      return func;
    } 
    // The instance through which we were accessed.
    PyObject* obj  = PyTuple_GetItem(args, 1);
    // The class through which we were accessed.
    PyObject* type = PyTuple_GetItem(args, 2);
    // no instance  => unbound method (accessed through class).
    if (obj == Py_None)
      obj = NULL;
    return PyMethod_New(func, obj, type);
  }

  // ------------------------------------------------------------
  // Find the applicable method, and extractors
  std::vector<void*>* dispatch(PyObject* args) {
    // Lookup cached dispatch
    PyObject* Py_extractors_and_method = PyDict_GetItem(this->dispatch_table, args);
    if (Py_extractors_and_method) {
      return (std::vector<void*>*)PyCObject_AsVoidPtr(Py_extractors_and_method);
    } 

    // Lookup failed ...

    size_t number_of_arguments_passed = PyTuple_Size(args);
    std::vector<void*>* extractors_and_method = new std::vector<void*>(number_of_arguments_passed+1);

    // Try each method in this GenericFunction, in turn
    for (size_t candidate=0; candidate<this->methods.size(); candidate++) {

      // Nasty temporary hack to allow our user to get on with his
      // life. (More than one method matches, and the less preferred
      // match happens to be made first because of the order in which
      // the methods happen to appear in the dicitonary. So here, we
      // search the methods in reverse order. A proper general
      // solution will require more thought.)

      //OneMethod& this_method = methods[candidate]; // Original
      OneMethod& this_method = methods[methods.size()-candidate-1]; // The hack

      // Reject any method whose parameter count does not match the
      // argument count.
      size_t max_number_of_parameters_in_method = this_method.function_member.parameterCount(false);
      size_t min_number_of_parameters_in_method = this_method.function_member.parameterCount(true);
      if ( max_number_of_parameters_in_method < number_of_arguments_passed ||
	   min_number_of_parameters_in_method > number_of_arguments_passed ) { continue; }
      // Assume method matches, until proof to the contrary is found.
      bool this_method_matches = true;
      // Try to find a conversion from each argument, to its corresponding parameter type
      for (size_t argno=0; argno<number_of_arguments_passed; argno++) {
	Extractor* extractor = 
	  find_extractor(PyTuple_GetItem(args, argno),//->ob_type,
			 this_method.function_member.type().parameter(argno));
	if (extractor) {
	  (*extractors_and_method)[argno] = (void*)extractor;
	} else {
	  this_method_matches = false; // Conversion not possible: reject the whole method as inapplicable.
	  break; // No point in looking at other arguments.
	}
      }
      if (this_method_matches) {
	// Cache successful lookup
	(*extractors_and_method)[number_of_arguments_passed] = (void*)&this_method;
	PyDict_SetItem(this->dispatch_table, args, PyCObject_FromVoidPtr(extractors_and_method,0));
	return extractors_and_method;
      } 
    }
    delete extractors_and_method;
    return NULL;
  }
};

PyObject* GenericFunction::__call__(PyObject* args) {
  PyObject* self = NULL;

  // In the case of a non-static method, the first argument is
  // separated out for special treatment as Python's "self" or C++'s
  // "this".
  if (!this->is_static) {
    self = PyTuple_GetItem(args, 0);
    PyObject* newargs = PyTuple_GetSlice(args, 1, 99999);
    Py_DECREF(args);
    args = newargs;
  }

  // Dispatcher optimization: change the tuple type to one whose
  // hash and comparison considers the types of the items stored,
  // rather than the items themselves
  args->ob_type = &FunkyTuple_Type;

  // Find the appropriate method
  std::vector<void*>* extractors_and_method = this->dispatch(args);
  if (extractors_and_method) {

    // Now that we are sure that a call is about to take place,
    // reset the extractor buffers to forget any values that are
    // hanging around from the previous call to a method.  (There
    // will be more of these buffers, when complete)
    ExtractorBuffer<long>::buffer.reset();
    ExtractorBuffer<double>::buffer.reset();
    ExtractorBuffer<std::string>::buffer.reset();
      
    // One extractor for each argument ... plus the method itself
    // are stored in extractors_and_method
    size_t arg_count = extractors_and_method->size()-1; 

    // The vector of pointers to C++ arguments which will be passed
    // to Reflex's invoke
    std::vector<void*> argvec(arg_count); 
    // The method is stored after the extractors, one extractor for each argument.
    OneMethod* the_method = (OneMethod*)(*extractors_and_method)[arg_count];

    // Extract the C++ values from the Python arguments
    for (size_t i=0; i<arg_count; i++) {
      argvec[i] = (*((Extractor*)(*extractors_and_method)[i]))(PyTuple_GetItem(args,i));
    }

    seal::reflex::Object returned;
    // Invoke the method via Reflex
    try {
      if (!this->is_static) {
	seal::reflex::Object obj = ((ProxyObject*)self)->__Object__;
	returned = the_method->function_member.invoke(obj, argvec);
      } else {
	returned = the_method->function_member.invoke(argvec);
      }
    } catch (...) {
      PyErr_SetString(PyExc_Exception, "An exception was raised in C++");
      return 0;
    }
    // Pointers to pointers are not supported in Pyreflex (only one
    // level of indirection). The following is based on the assumption
    // that more than one lvl of indirection is simply not
    // encountered.

    // Stub functions promote pointers to the pointed-to values only
    // at one level of indirection. In the case of Fields such
    // promotion does not occur.

    // A consequence of the stub functions' special case for one-lvl
    // indirection is that we never see pointers returned from
    // function calls (if we assume the absence of multiple
    // indirections (T**, T*** etc.), so we do not need to perform the
    // dereference that needs to be done in Field::__get__.
    if (returned.type().isPointer()) {
      // Note that the type is dereferenced while the address is not:
      // it already has been dereferenced to the pointed-to value by
      // the stub functions.
      if (returned.type().isReference()) {
	// Aaaaaargh ... could we please, PLEASE, pretty please with
	// sugar on top, have the stub functions not make a special
	// case out of single pointers ... it makes such a huge mess
	// of everything. Please ?

	// So, in the case of T*&, the address() really is a T**, and
	// the type() is consistent with the address().
	returned = seal::reflex::Object(returned.type().toType(), *(void**)returned.address());
      } else {
	// In the case of T* the address() is a T* (not a T**), so the
	// type() is inconsistent with the address().
	returned = seal::reflex::Object(returned.type().toType(), returned.address());
      }
    }

    PyObject* it = (*(the_method->inserter)) (returned);
    return it;
  }
  std::string error = "No matching method found in generic function\n";
  error += "Arguments types were:\n";
  for (int i=0; i<PyTuple_Size(args); i++) {
    error += PyString_AsString(PyObject_GetAttrString(PyObject_Type(PyTuple_GetItem(args, i)), "__name__"));
    error += "  ";
  }
  error += "\nCandidates:\n";
  for (unsigned int i=0; i<methods.size(); i++) {
    error +=  methods[i].function_member.name() + ":  " + methods[i].function_member.type().name(4) + "\n";
  }
  PyErr_SetString(PyExc_TypeError, error.c_str());
  return NULL;
}


#endif
