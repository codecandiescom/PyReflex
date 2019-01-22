#if defined (__GNUC__) && ( __GNUC__ >=3)
#define protected public
#include <cxxabi.h>
#undef protected
#endif

// Pyreflex would like to use dynamic casting operations as provided
// by Reflex itself. Currently Reflex is not providing sufficient
// support for Pyreflex needs, hence direct implementation in here.
// This code is actually also used in the static binding of Reflex so
// it has all chances to stay.


void* dynamicUpcast(const std::type_info* target, const std::type_info* source, void* object) {
  void* object_ptr = object; // Avoid modifying the original
#ifdef __CXXABI_H
  if (const abi::__class_type_info* sourceTI =
      dynamic_cast<const abi::__class_type_info*>(source)) {
    if (sourceTI->__do_upcast(dynamic_cast<const abi::__class_type_info*>(target), &object_ptr)) {
      return object_ptr;
    }
  }
#elif defined _WIN32
  void* target_pointer =  __RTDynamicCast
    (object_ptr,
     0,
     (void*)source,
     (void*)target,
     0);
  return target_pointer;
#endif

  return 0;
};


void* dynamicDownCast_core(const std::type_info& target_typeinfo,
			   const std::type_info& source_typeinfo,
			   void* source_pointer) {
#if defined (__CXXABI_H)
  abi::__class_type_info* sourceTI = (abi::__class_type_info *)&(source_typeinfo);
  abi::__class_type_info* targetTI = (abi::__class_type_info *)&(target_typeinfo);
  void* target_pointer = abi::__dynamic_cast(source_pointer, sourceTI, targetTI, -1);
#elif defined (_WIN32)
  // This branch not tested at all ...
  void* target_pointer =  __RTDynamicCast
    (source_pointer,
     0,
     (void*)&source_typeinfo,
     (void*)&target_typeinfo,
     0);
#else
#error "dynamic_cast runtime not available on this platform."
#endif
  if (! target_pointer) {
    PyErr_SetString(PyExc_Exception, "dynamic cast resulted in NULL pointer");
  }
  return target_pointer;
}

PyObject* dynamicDownCast(PyObject*, PyObject* Pinstance) {
  void* source_pointer = ((ProxyObject*)Pinstance)->__the_instance__;
  PyObject* __class__ = PyObject_GetAttrString(Pinstance, "__class__");
  PyObject* __typeinfo__ = (PyObject_GetAttrString(__class__, "__typeinfo__"));
  const std::type_info& source_type_info = *((std::type_info*)((ProxyObject*)__typeinfo__)->__the_instance__);
  const std::type_info& target_type_info = typeid(*(Dummy*)source_pointer);
  // A null-op cast seems to fail in Pyreflex (though not in Godel?)
  if (&source_type_info == &target_type_info) {
    //std::cout << "Not bothering to cast, the types are identical" << std::endl;
    Py_INCREF(Py_None);
    return Py_None;
  }
  void* target_pointer =  dynamicDownCast_core(target_type_info,
					       source_type_info,
					       source_pointer);
  if (! target_pointer) { 
    std::string s = "Dynamic cast failed: ";
    s += source_type_info.name();
    s += " -> ";
    s += target_type_info.name();
    PyErr_SetString(PyExc_Exception, s.c_str());
    return 0;
  }
  ((ProxyObject*)Pinstance)->__the_instance__ = target_pointer;
  // Here it gets interesting because Godel and Pyreflex share this function ...
  PyObject* new_class = PyDict_GetItemString(global_typeinfo_map_temporary_hack, target_type_info.name());
  if (!new_class) {
    new_class = PyDict_GetItemString(auxC, target_type_info.name());
  }
  if (new_class) {
    PyObject_SetAttrString(Pinstance, "__class__", new_class);
  } else {
    //std::cout << "Failed to downcast to " << target_type_info.name() << std::endl;
  }
  Py_INCREF(Py_None);
  return Py_None;
}


// Reflex can't cast reliably ... use this for now.

PyObject* dynamicDownCastOBJECT(PyObject*, PyObject* Pinstance) {
  void* source_pointer = ((ProxyObject*)Pinstance)->__Object__.address();
  PyObject* __class__ = PyObject_GetAttrString(Pinstance, "__class__");
  PyObject* __typeinfo__ = (PyObject_GetAttrString(__class__, "__typeinfo__"));
  const std::type_info& source_type_info = *((std::type_info*)((ProxyObject*)__typeinfo__)->__the_instance__);
  const std::type_info& target_type_info = typeid(*(Dummy*)source_pointer);
  if (&source_type_info == &target_type_info) {
    //std::cout << "Not bothering to cast, the types are identical" << std::endl;
    Py_INCREF(Py_None);
    return Py_None;
  }
  void* target_pointer =  dynamicDownCast_core(target_type_info,
					       source_type_info,
					       source_pointer);
  if (! target_pointer) { 
    std::string s = "Dynamic cast failed: ";
    s += source_type_info.name();
    s += " -> ";
    s += target_type_info.name();
    PyErr_SetString(PyExc_Exception, s.c_str());
    return 0;
  }
  PyObject* new_class = PyDict_GetItemString(global_typeinfo_map_temporary_hack, target_type_info.name());
  if (!new_class) {
    new_class = PyDict_GetItemString(auxC, target_type_info.name());
  }

  if (new_class) {
    PyObject_SetAttrString(Pinstance, "__class__", new_class);
    PyObject* PythonDynamicType = PyObject_GetAttrString(new_class, "__Reflex_Type__");
    seal::reflex::Type ReflexDynamicType = *(seal::reflex::Type*)((ProxyObject*)PythonDynamicType)->__the_instance__;
    seal::reflex::Object cast_object(ReflexDynamicType, target_pointer);
    ((ProxyObject*)Pinstance)->__Object__ = cast_object;
  } else {
    //std::cout << "Failed to downcast to " << target_type_info.name() << std::endl;
  }
  Py_INCREF(Py_None);
  return Py_None;
}

