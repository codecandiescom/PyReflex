// Conversion between Pyreflex proxy types and the underlying C++
// types. 
// Worth reading comments at the top of Field.h

// If the type is a Class, Class* or Class&, return the Class as
// reflex::Type, otherwise return something which is false in a
// Boolean context

//seal::reflex::Type have_we_a_class(seal::reflex::Type RT) {
//  if (RT.typeType() == seal::reflex::CLASS) {
//    return RT;
//  }
//  if (RT.typeType() == seal::reflex::POINTER) {
//    seal::reflex::Type toType = RT.toType();
//    if (toType.typeType() == seal::reflex::CLASS) {
//      return toType;
//    }
//  }
//  return seal::reflex::Type();
//}

// The above version tried to ensure that no more than one level of
// pointer indirection is unwrapped. We throw caution to the wind, and
// assume that two levels of indirection are never seen, for now. This
// allows us to easily cover the more common use case of mixed
// pointers and typedefs.

seal::reflex::Type have_we_a_class(seal::reflex::Type RT, bool& was_it_a_pointer) {
  while (RT.isPointer() || RT.isTypedef()) {
    if (RT.isPointer()) { 
      // The class inserter needs to know, for ownership determination
      was_it_a_pointer = true;
    }
    RT = RT.toType();
  }
  if (RT.isClass()) {
    return RT;
  }
  return seal::reflex::Type(); // invalid type == NOT FOUND
}

// Scratch-space for extractors to place their results, so that they
// may return pointers to them.

// reflex::FunctionMember::invoke takes a vector of void*s pointing to
// the arguments to the function. In the case of Python-builtins, the
// extractors create new instances of some relevant type. These must
// kept in a place which ensures that the aforementioned void*s remain
// valid until the call to invoke is complete. Thus we provide an
// expandable buffer for each relevant type, for temporarily storing
// these values. On each function call, we start filling the buffers
// from the start, overwriting whatever value happened to be there
// previously: it is no longer needed, it was used in a previous
// call). Thus a call to a function with parameter types (int, double,
// int) will place two values in the int buffer, and one value in the
// buffer for doubles.

template<class T>
struct ExtractorBuffer {
  // Singly-linked list
  struct Node {
    T item;
    Node* next;
    Node()     : item(),   next(NULL) {};
    Node(T it) : item(it), next(NULL) {};
    ~Node() { if (this->next) delete this->next; }
  };
  Node* first;
  Node* current;
  // ------------------------------------------------------------
  ExtractorBuffer(int n=1) : first(new Node), current(first) {
    // Unimplemented optimization:
    // Allocate contiguous space for the first n nodes, to ensure locality of memory.
    n++; // To keep the compilers quiet for now.
  }
  // ------------------------------------------------------------
  // Keep the argument somewhere you can point to, guaranteeing that
  // the pointer will remain valid until the "invoke" has finished.
  void* pointer_to(T it) {
    this->current->item = it;
    void* address = (void*) &(this->current->item);
    if (!this->current->next) 
      this->current->next = new Node;
    this->current = this->current->next;
    return address;
  }
  // ------------------------------------------------------------
  void reset() { this->current = this->first; }
  static ExtractorBuffer buffer;
};
template<class T> ExtractorBuffer<T> ExtractorBuffer<T>::buffer;


// ****************************** Extractors ***************************
//     Return the C objects which are embedded in Python objects.

struct Extractor {
  virtual void* operator() (PyObject* arg) = 0;
  virtual ~Extractor() {}
};

// Builtins -> fundamentals

template<class RETURN_TYPE, class A1>
struct BuiltinExtractor : public Extractor {
  RETURN_TYPE (*function)(A1);
  BuiltinExtractor(RETURN_TYPE (fn)(A1)): function(fn) {};
  void* operator() (PyObject* arg) {
    return ExtractorBuffer<RETURN_TYPE>::buffer.pointer_to(this->function(arg));
  }
};

template<class RETURN_TYPE, class A1>
Extractor* makeBuiltinExtractor(RETURN_TYPE (extractor_function)(A1)) {
  return new BuiltinExtractor<RETURN_TYPE, A1>(extractor_function);
}

struct CharStarExtractor : public Extractor {
  void* operator() (PyObject* arg) {
    // Warning! This returns the pointer to the internal buffer of the
    // Python string, so we must ensure that we copy it, before
    // handing it over to C++ (we don't do this anywhere, yet.)  This
    // copy would have to live in some extractor buffer, but the size
    // of each element of the buffer now becomes variable, which is a
    // bit tricky (ie, is not as easily handled as the other built-in
    // types. A similar issue will arise with array types.)

    return PyString_AsString(arg);
  }
};

struct StdStringExtractor : public Extractor {
  void* operator() (PyObject* arg) {
    return ExtractorBuffer<std::string>::buffer.pointer_to(std::string(PyString_AsString(arg)));
  }
};

struct CharExtractor : public Extractor {
  void* operator() (PyObject* arg) {
    // Warning! This returns the pointer to the internal buffer of the
    // Python string, so we must ensure that we copy it, before
    // handing it over to C++ (we don't do this anywhere, yet.) See
    // CharStarExtractor comments ...
    return PyString_AsString(arg);
  }
};

#if defined (__GNUC__) && ( __GNUC__ >=3)
#define abiupcast
#endif
#ifdef abiupcast
struct UpcastingExtractor : public Extractor {
  const std::type_info* source;
  const std::type_info* target;
  UpcastingExtractor(const std::type_info* src, const std::type_info* tgt)
    : source(src), target(tgt) {};
  void* operator()(PyObject* arg) {
    void* object_address = ((ProxyObject*)arg)->__Object__.address();
    void* res = dynamicUpcast(target, source, object_address);
    return res;
  }
};
#else
struct UpcastingExtractor : public Extractor {
  seal::reflex::Type source;
  seal::reflex::Type target;
  UpcastingExtractor(seal::reflex::Type& src, seal::reflex::Type& tgt)
    : source(src), target(tgt) {};
    void* operator()(PyObject* arg) {
      seal::reflex::Object object = ((ProxyObject*)arg)->__Object__;
      seal::reflex::Object res = source.castObject(target, object);
      return res.address();
    }
};
#endif

// ****************************** Inserters ****************************
//                Embed C objects into Python objects

struct Inserter {
  virtual PyObject* operator()(seal::reflex::Object arg) const = 0;
  virtual ~Inserter() {}
};

template<class ARG_TYPE>
struct BuiltinInserter : public Inserter {
  PyObject* (*function)(ARG_TYPE);
  BuiltinInserter(PyObject* (fn)(ARG_TYPE)): function(fn) {};
  PyObject* operator() (seal::reflex::Object arg) const	{
    return this->function(*(ARG_TYPE*)arg.address());
  }
};

template<class ARG_TYPE>
Inserter* makeBuiltinInserter(PyObject* (inserter_function)(ARG_TYPE)) {
  return new BuiltinInserter<ARG_TYPE>(inserter_function);
}

struct VoidInserter : public Inserter {
  PyObject* operator() (seal::reflex::Object ) const {
    Py_INCREF(Py_None);
    return Py_None;
  }
};

struct StdStringInserter : public Inserter {
  PyObject* operator() (seal::reflex::Object arg) const {
    return PyString_FromString(((std::string*)arg.address())->c_str());
  }
};

struct CharStarInserter : public Inserter {
  PyObject* operator() (seal::reflex::Object arg) const {
    return PyString_FromString((const char*)arg.address());
  }
};

struct CharInserter : public Inserter {
  PyObject* operator() (seal::reflex::Object arg) const {
    return PyString_FromStringAndSize((const char*)arg.address(), 1);
  }
};

struct ClassInserter : public Inserter {
  mutable PyTypeObject* formal_return_type;
  const seal::reflex::Type Reflex_return_type;
  bool downcast;
  bool is_it_a_pointer;

  ClassInserter(seal::reflex::Type Rrt, bool pointerp) : 
    formal_return_type(0), Reflex_return_type(Rrt), downcast(false), is_it_a_pointer(pointerp)
  {
    if (Reflex_return_type.isClass()) {
      if (Reflex_return_type.isVirtual()) {
        this->downcast = true;
      }

    } else {
      std::cout << "Warning: ClassInserter given a non-class as formal return type: " 
        	<< Reflex_return_type.name() << std::endl;
    }
  }
  // ------------------------------------------------------------
  PyObject* operator() (seal::reflex::Object object) const {

    // Allocate an uninitialized instance of the return type (no downcasting yet)

    if (! this->formal_return_type) {
      this->find_formal_return_type();
    }
    // What we want to do ...

    // Receive an Object
    // If it is a pointer, dereference it (but not if it is a pointer to a pointer?)
    // Ask Reflex to cast it down to its dynamic type
    // Create a Python proxy instance corresponding to the type() of the Object
    // Insert the Object into the proxy
    // Return the proxy

    // What actulally happens at the moment (Awaiting Reflex getting a
    // reliable dowcasting operation.)

    PyObject* py_instance = PyObject_CallMethod((PyObject*)&ProxyBaseObject_Type, "__new__", "O", this->formal_return_type);
    PyErr_Print();
    // Insert the C++ instance into the Python instance
    ((ProxyObject*)py_instance)->__Object__ = object;

    if (downcast) {
      dynamicDownCastOBJECT(NULL, py_instance);
      object = ((ProxyObject*)py_instance)->__Object__;
    }
    
    // For now, we want the contained reflex::Objects to contain the
    // 'final' type only, so we strip all pointers and typedefs from
    // the object.

    // XXX there is a rather complicated interaction between typedefs
    // and pointers, as a consequence of Reflex stub functions
    // pretending that pointers do not exist.

    // Hacking around Reflex' failure to support typedefs.
    if (object.type().isTypedef()) {
      object = seal::reflex::Object(object.type().toType(), object.address());
      ((ProxyObject*)py_instance)->__Object__ = object;
      
      if (object.type().isPointer()) {
	object = seal::reflex::Object(object.type().toType(), object.address());
	((ProxyObject*)py_instance)->__Object__ = object;
      }
    }
    
    else if (object.type().isPointer()) {
      object = seal::reflex::Object(object.type().toType(), *(void**)object.address());
      ((ProxyObject*)py_instance)->__Object__ = object;
    }
    
    if (!  ((ProxyObject*)py_instance)->__Object__.address() ) {
      return 0;
    }

    // If the object is returned by value, Python must take
    // responsibility for deleting its copy when the containing proxy
    // is garbage collected. If the object is returned by pointer or
    // by reference, Python should NOT delete the object when the
    // proxy is collected. The user may set the value of this
    // deletion-responsibility field explicitly (using
    // take/drop_ownership).
    if (this->is_it_a_pointer || // Aargh! What about typedefs ... have_we_a_class took care of it ?
	this->Reflex_return_type.isReference() ) {
      ((ProxyObject*)py_instance)->python_owns = false;
    } else {
      // Already done in proxyobject_newOBJECT, don't bother doing it
      // again ...
      ((ProxyObject*)py_instance)->python_owns = true;
      // but proxyobject_newOBJECT is only set as the __new__ of each
      // and every class individually, it is not set as the __new__ of
      // ProxyBaseObject, so py_instance is not created by
      // proxyobject_newOBJECT. This should be fixed to be strictly
      // correct, but as all our test pass it isn't worth the effort
      // at this point.
    }

    return py_instance;
  }
  // ------------------------------------------------------------
  void find_formal_return_type () const {
    // Find the PyReflex proxy class corresponding to the returned type
    PyObject* address = PyInt_FromLong((long)(Reflex_return_type.id()));
    this->formal_return_type = (PyTypeObject*)PyDict_GetItem(Reflex_Type_to_Python_proxy_map,
							     address); 
    Py_DECREF(address);
  }
};

// Bypassing the following functionality internally, for now.

//// ************************************************************
//// Hacking around missing Reflex functionality.
//// ************************************************************
//          bool Type_equivalent1(seal::reflex::Type self, seal::reflex::Type& other) {
//            while (self != other) {
//              if (other.typeType() != seal::reflex::TYPEDEF) {
//                return false;
//              }
//              other = other.toType();
//            }
//            return true;
//          }
//
//          bool Type_equivalent(seal::reflex::Type self, seal::reflex::Type other) {
//            // NB, other may be modified by the first call to equivalent1
//            return (Type_equivalent1(self,other) || Type_equivalent1(other, self));
//          }
//// ************************************************************
//// ************************************************************

void Type_bottom_type(seal::reflex::Type& Type, unsigned char& pointer_depth) {
  while (Type.isPointer() || Type.isTypedef()) {
    if (Type.isPointer()) {
      pointer_depth += 1;
    }
    Type = Type.toType();
  }
}

bool Type_equivalent(seal::reflex::Type LeftType, seal::reflex::Type RightType) {
  unsigned char LeftPointerDepth = 0;
  unsigned char RightPointerDepth = 0;
  Type_bottom_type(LeftType,  LeftPointerDepth);
  Type_bottom_type(RightType, RightPointerDepth);
  if (LeftType.id() == RightType.id() && LeftPointerDepth == RightPointerDepth) {
    return true;
  }
  return false;
}

bool Type_equivalent_wrap(seal::reflex::Type LeftType, std::string TypeName) {
  return Type_equivalent(LeftType, seal::reflex::Type::byName(TypeName));
}


bool Type_equivalent_ignore_pointers(seal::reflex::Type LeftType, std::string TypeName) {
  // Checks whether two types are equivalent, ignoring any pointer
  // indirection. ie T* is equivalent to T.

  // This implementation assumes that only one level of pointer
  // indirection is possible. Expect things to go wrong if a T**
  // appears. (By which I mean, T**** is equivalent to T, which is
  // probably not what the user intended.)

  seal::reflex::Type RightType = seal::reflex::Type::byName(TypeName);
  unsigned char LeftPointerDepth = 0;
  unsigned char RightPointerDepth = 0;
  Type_bottom_type(LeftType,  LeftPointerDepth);
  Type_bottom_type(RightType, RightPointerDepth);
  if (LeftType.id() == RightType.id()) {
    return true;
  }
  return false;
}

//********************* Extractor discovery ****************************
template<typename T>
T MyInt_As(PyObject* arg) {
  return (T)PyInt_AsLong(arg);
}

template<typename T>
T MyFloat_As(PyObject* arg) {
  return (T)PyFloat_AsDouble(arg);
}


struct Extractor_matching_table {
  PyTypeObject* fromPyType;
  char* toName;
  Extractor* extractor;
};

Extractor_matching_table extractor_matching_table[] = {
  {&PyInt_Type,     "int",            makeBuiltinExtractor(PyInt_AsLong)},
  {&PyInt_Type,     "unsigned int",   makeBuiltinExtractor(PyInt_AsLong)},
  {&PyInt_Type,     "short int",      makeBuiltinExtractor(MyInt_As<short>)},
  {&PyInt_Type,     "long int",       makeBuiltinExtractor(PyInt_AsLong)},
  {&PyInt_Type,     "unsigned char",  makeBuiltinExtractor(MyInt_As<unsigned char>)},
  {&PyInt_Type,     "signed char",    makeBuiltinExtractor(MyInt_As<signed char>)},
  {&PyBool_Type,    "bool",           makeBuiltinExtractor(PyInt_AsLong)},
  {&PyFloat_Type,   "double",         makeBuiltinExtractor(PyFloat_AsDouble)},
		    
  {&PyFloat_Type,   "float",          makeBuiltinExtractor(MyFloat_As<float>)},
		    
  {&PyInt_Type,     "float",          makeBuiltinExtractor(MyInt_As<float>)},
  {&PyInt_Type,     "double",         makeBuiltinExtractor(MyInt_As<double>)},

  {&PyString_Type,  "std::string",    new StdStringExtractor},
  //{&PyString_Type,  "char",           new CharStarExtractor},

  { NULL }
};

Extractor* find_builtin_extractor(PyTypeObject* Python_type, seal::reflex::Type Reflex_type) {

  Extractor* found_extractor = NULL;

  for (Extractor_matching_table* it=extractor_matching_table; it->fromPyType; it++) {
    if ((Python_type == it->fromPyType) && Type_equivalent_ignore_pointers(Reflex_type, it->toName)) {
      found_extractor = it->extractor;
      break;
    }
  }

  if (Python_type == &PyString_Type && Type_equivalent(Reflex_type, seal::reflex::Type::byName("char*"))) {
    found_extractor = new CharStarExtractor;
  }

  if (Python_type == &PyString_Type && Type_equivalent(Reflex_type, seal::reflex::Type::byName("char"))) {
    found_extractor = new CharExtractor;
  }

  return found_extractor;
}

// ------------------------------------------------------------
// Find the extractor for a single conversion
static Extractor* find_extractor(PyObject* Python_instance, seal::reflex::Type Reflex_type) {
  PyTypeObject* Python_type = (PyTypeObject*)(Python_instance->ob_type);
  Extractor* extractor = ::find_builtin_extractor(Python_type, Reflex_type);
  if (extractor) { return extractor; }
  
  // Check whether parameter is a class (maybe via pointer, reference
  // or typedef)
  bool was_it_a_pointer = false;
  seal::reflex::Type Reflex_target_type = have_we_a_class(Reflex_type, was_it_a_pointer);
  if (Reflex_target_type.id()) {
    // Get the Godel-wrapped reflex::Type corresponding to the Python_type
    PyObject* PyReflexSrcType = PyObject_GetAttrString((PyObject*)Python_type, "__Reflex_Type__");
    if (!PyReflexSrcType) {
      // The argument is not a reflected class
      PyErr_Clear();
    } else {
      // Extract the actual reflex::Type* from the above
      seal::reflex::Type Reflex_source_type = *(seal::reflex::Type*)
        (((ProxyObject*)PyReflexSrcType)->__the_instance__);
      Extractor* candidate =
#ifdef abiupcast
        new UpcastingExtractor(&(Reflex_source_type.typeInfo()),
			       &(Reflex_target_type.typeInfo()));
#else
        new UpcastingExtractor(Reflex_source_type,
			       Reflex_target_type);
#endif
      // Check whether the upcast succeeds to confirm that the types are compatible
      if ((*candidate)(Python_instance)) {
	return candidate;
      } else {
        delete candidate;
      }
    }
  }
  // For use in conversion diagnostics
  // std::cout << "dispatch: Conversion mismatch: "; PyObject_Print(Python_instance, stdout, 0); std::cout << " -> " << Reflex_type.name(4) << std::endl;
  return NULL;
}


//*********************** Inserter discovery *****************************
template<typename T>
PyObject* MyInt_From(T arg) {
  return PyInt_FromLong(arg);
}

template<typename T>
PyObject* MyFloat_From(T arg) {
  return PyFloat_FromDouble(arg);
}

template<typename T>
PyObject* MyBool_From(T b) {
  if (b) {
    Py_INCREF(Py_True);
    return Py_True;
  }
  Py_INCREF(Py_False);
  return Py_False;
}


struct Inserter_matching_table {
  char* fromName;
  Inserter* inserter;
};

Inserter_matching_table inserter_matching_table[] = {
  {"int",           makeBuiltinInserter(PyInt_FromLong)},
  {"unsigned int",  makeBuiltinInserter(MyInt_From<unsigned>)},
  {"unsigned char", makeBuiltinInserter(MyInt_From<unsigned char>)},
  {"short int",     makeBuiltinInserter(MyInt_From<short int>)},
  {"long int",      makeBuiltinInserter(MyInt_From<long int>)},
  {"bool",          makeBuiltinInserter(MyBool_From<bool>)},
  {"signed char",   makeBuiltinInserter(MyInt_From<signed char>)},
  {"unsigned char", makeBuiltinInserter(MyInt_From<unsigned char>)},
  {"double",        makeBuiltinInserter(PyFloat_FromDouble)},
  {"float",         makeBuiltinInserter(MyFloat_From<float>)},

  {"std::string",   new StdStringInserter},

  {"void",          new VoidInserter},

  { NULL }
};


Inserter* find_inserter(seal::reflex::Type Reflex_type) {

  Inserter* found_inserter = NULL;
  for (Inserter_matching_table* it=inserter_matching_table; it->inserter; it++) {
    if (Type_equivalent_ignore_pointers(Reflex_type, it->fromName)) {
      found_inserter = it->inserter;
      break;
    }
  }

  // chars are a special case, because the char* MUST be treated
  // differently from char. The former will be terminated by the NULL
  // character, while the latter is always of length 1, and is not
  // necessarily followed by the NULL terminator. The search performed
  // for other types assumes that T* and T are always treated
  // identically (because Reflex stub functions pretend that they ARE
  // the same thing).

  if (Type_equivalent_wrap(Reflex_type, "char")) {
    found_inserter = new CharInserter;
  }

  if (Type_equivalent_wrap(Reflex_type, "char*")) {
    found_inserter = new CharStarInserter;
  }

  if (found_inserter) {
    return found_inserter;
  }

  // Try finding an inserter for classes (maybe by pointer, reference
  // or typedef) to match the return type
  bool was_it_a_pointer = false;
  seal::reflex::Type Reflex_return_type = have_we_a_class(Reflex_type, was_it_a_pointer);
  if (Reflex_return_type) {
    // Should memoize this, rather than returning a new one each time.
    return new ClassInserter(Reflex_return_type, was_it_a_pointer);
  }
  //if (this->inserter) { return; }
  //std::cout << "No inserter found for " << Reflex_type.name() << std::endl;
  return NULL;
}


PyObject* CommonInserter(PyObject* the_class, void* arg, bool downcast=false) {
  if (!arg) {
    // Get the appropriate NULLPointer class
    PyObject* argtuple = PyTuple_New(1);
    PyTuple_SetItem(argtuple, 0, the_class);
    PyObject* NULL_pointer_class = PyObject_CallObject(NULLPointer, argtuple);
    Py_DECREF(argtuple);
    // Instantiate the NULLPoniter class
    PyObject* noargs = PyTuple_New(0);
    PyObject* the_null_pointer = PyObject_CallObject(NULL_pointer_class, noargs);
    Py_DECREF(noargs);
    // Return the NULLPointer instance
    return the_null_pointer;
  } else {
    // Insert C instance into Python proxy (static type)
    PyObject* py_instance = PyObject_CallMethod((PyObject*)&ProxyBaseObject_Type, "__new__", "O", the_class);
    ((ProxyObject*)py_instance)->__the_instance__ = arg;
    if (downcast) {
      dynamicDownCast(NULL, py_instance);
    }
    return py_instance;
  }
  return 0; // Should never reach here.
}

//PyObject* EnumInserter(PyObject* the_enum_type, void* arg) {
//  PyObject* py_instance = 0;
//  py_instance = PyObject_CallFunction(the_enum_type, "i", (...)arg);
//}


int CommonExtractor(PyObject* arg, void*& address) {
  address = ((ProxyObject*)arg)->__the_instance__;
  if (address) { return 1; }
  PyErr_SetString(PyExc_Exception, "Proxy extraction failed");
  return 0;}


