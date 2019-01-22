// Pyreflex currently only supports one level of indirection

// By default, Pyreflex objects are created according to the following:
// 1) If Pyreflex type is T, then T is not a pointer type
// 2) It contains a reflex::Object whose type() is T
// 3) The reflex::Object contains a void* pointing to a T 

// Stub functions for methods returning pointers automatically
// dereference the pointer. This results in Reflex returning a
// reflex::Object in an inconsistent state: The Object's type() does
// not match the object pointed to by its address() .

// No such inconsistency is present in the case of Field that are
// pointers. Therefore the insertion of pointer types must be treated
// differently in the two cases.

// Currently T and T*, for any non pointer type T, are essentially
// equivalent. This conflation of different levels of indirections has
// implications for the support of higher levels of indirection (all
// level appear to be conflatable). Therefore supporting higher levels
// of indirections would require re-thinking the situation. The
// conflation of the first two levels is imposed on Pyreflex by the
// behavior of the Reflex stub functions as mentioned above.

struct Field {
  seal::reflex::Member member;
  ptrdiff_t offset;
  Inserter* inserter;
  seal::reflex::Type Reflex_type;

  Field(seal::reflex::Member m, ptrdiff_t class_offset) :
    member(m), offset(m.offset() + class_offset), Reflex_type(m.type()) {
    this->inserter  = ::find_inserter (Reflex_type);
  }

  PyObject* __get__(PyObject* args) {
    PyObject* obj = PyTuple_GetItem(args, 1);
    if (obj == Py_None) {
      // Not implemented yet!  

      // For static members: should return the value.  For dynamic
      // ones, how about returning the value's type? Alternatively
      // self.
      Py_INCREF(Py_None);
      return Py_None;
    } else {
      seal::reflex::Object object = ((ProxyObject*)obj)->__Object__;
      // But pointer objects should not be stored in the first place:
      // so this is ensured at insertion time ... for now. Once we
      // admit pointers to pointers, this can of worms will need
      // reconsidering.

      seal::reflex::Object value = this->member.get(object);

      // XXXXXXXXX hacking around trying to understand the nasty
      // interaction of pointers, typedefs, and the stub functions'
      // lies about pointers.
      if (value.type().isTypedef()) {
        value = seal::reflex::Object(value.type().toType(), value.address());
      }

      // Promote T* to T in the proxy object.
      if (value.type().isPointer()) {
      // Note that the type is dereferenced as well as the address:
      // contrast to the equivalent code in GenericFunction.h for
      // methods returning pointers where stub functions had
      // dereferenced already the address.
        value = seal::reflex::Object(value.type().toType(), *(void**)value.address());
      }
      return (*(this->inserter))(value);
    }
  }


  PyObject* __set__(PyObject* args) {
    PyObject* obj    = PyTuple_GetItem(args,1);
    PyObject* Pvalue = PyTuple_GetItem(args,2);
    Extractor* extractor = ::find_extractor(Pvalue, Reflex_type);
    void* Cvalue = (*extractor)(Pvalue);

    seal::reflex::Object object = ((ProxyObject*)obj)->__Object__;
    // This could only work as well as the underlying implementation
    // of set in Reflex because of the necessity to discover the
    // availability of operator=()
    // Therefore this might fail for some classes.


    // Actually, member::set currently uses the size of the time of
    // the formal type of the member ... ie the size of the typedef,
    // in this case, so we must wait for a promised change, for this
    // ugly hack to work.
    //while (object.type().isTypedef()) {
    //  // member::set works by memcopy(sizeof(type), ...)
    //  // sizeof(typedef) == 0, so let's give set a helping hand by
    //  // changing the type, in the case of a typedef.
    //  // ... If set ever gets an implementation based on operator=, then
    //  // we can and must get rid of all these hideuous hacks.
    //  std::cout << "Field.__set__ changing type from " << object.type().name(2) << " to ";
    //  object = seal::reflex::Object(object.type().toType(), object.address());
    //  std::cout << object.type().name(2) << std::endl;
    //}
    this->member.set(object, Cvalue);
    Py_INCREF(Py_None);
    return Py_None;
  }
};

