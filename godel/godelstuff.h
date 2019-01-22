#include <string>
#include <iostream>

typedef struct {
  PyObject_HEAD
  void* __the_instance__;
} ProxyObject;

PyObject* get_the_instance(PyObject*, PyObject* obj) {
  return PyInt_FromLong((long)((ProxyObject*)obj)->__the_instance__);
} 

PyObject* set_the_instance(PyObject*, PyObject* args) {
  PyObject* obj;
  long address;
  if (!PyArg_ParseTuple(args, "Oi", &obj, &address)) {
    return 0;
  }
  ((ProxyObject*)obj)->__the_instance__ = (void*)address;
  Py_INCREF(Py_None);
  return Py_None;
} 

PyTypeObject ProxyBaseObject_Type = {
	PyObject_HEAD_INIT(&PyType_Type)
 	0,					/* ob_size */
	"proxyobject",				/* tp_name */
	sizeof(ProxyObject),			/* tp_basicsize */
	0,					/* tp_itemsize */
	0,//(destructor)object_dealloc,		/* tp_dealloc */
	0,					/* tp_print */
	0,			 		/* tp_getattr */
	0,					/* tp_setattr */
	0,					/* tp_compare */
	0,//object_repr,			/* tp_repr */
	0,					/* tp_as_number */
	0,					/* tp_as_sequence */
	0,					/* tp_as_mapping */
	0,//object_hash,			/* tp_hash */
	0,					/* tp_call */
	0,//object_str,				/* tp_str */
	0,//PyObject_GenericGetAttr,		/* tp_getattro */
	0,//PyObject_GenericSetAttr,		/* tp_setattro */
	0,					/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	PyDoc_STR("Pyreflex Proxy base type"),	/* tp_doc */
	0,					/* tp_traverse */
	0,					/* tp_clear */
	0,					/* tp_richcompare */
	0,					/* tp_weaklistoffset */
	0,					/* tp_iter */
	0,					/* tp_iternext */
	0,//object_methods,			/* tp_methods */
	0,					/* tp_members */
	0,//object_getsets,			/* tp_getset */
	0,					/* tp_base */
	0,					/* tp_dict */
	0,					/* tp_descr_get */
	0,					/* tp_descr_set */
	0,					/* tp_dictoffset */
	0,//object_init,			/* tp_init */
	0,//PyType_GenericAlloc,		/* tp_alloc */
	PyBaseObject_Type.tp_new,//object_new,	/* tp_new */
	0,//PyObject_Del,           		/* tp_free */
};



// *********************************************************************


void godel_initialize_class(PyObject* module_dict,
			    char* class_name,
			    PyObject*& ClassObject,
			    const std::type_info& tid,
			    PyMethodDef dynamic_methods[],
			    PyMethodDef static_methods[]) {
  PyObject* PYbases = PyTuple_New(1); // no bases for now, only ProxyBaseObject
  PyTuple_SetItem(PYbases, 0, (PyObject*)&ProxyBaseObject_Type);
  PyObject* PYclass_dict = PyDict_New();
  PyObject* PYclass_name = PyString_FromString(class_name);
  PyObject* PYclass_docstring = PyString_FromString("No docstring for now");
  PyDict_SetItemString(PYclass_dict, "__doc__", PYclass_docstring);
  ClassObject =
    PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type,
				 PYclass_name, PYbases, PYclass_dict, NULL);
  // typeinfo stuff ... do we need this for Godel ?
  PyObject_SetAttrString(ClassObject, "__typeinfo__",
			 PyCObject_FromVoidPtr((void*)&tid, 0));
  //PyDict_SetItemString(typeinfo_to_class_dict, tid.name(), ClassObject);

  // Register class in module
  PyDict_SetItemString(module_dict, class_name, ClassObject);
  // Register dynamic methods in class
  for (PyMethodDef* method = dynamic_methods; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject);
    PyObject_SetAttrString(ClassObject, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }
  // Register static methods in class
  //for (PyMethodDef* method = static_methods; method->ml_name != NULL, method++) {
  //  PyObject* function = PyCFunction_New(method, NULL);
  //  PyObject_SetAttrString(ClassObject, method->ml_name, function);
  //  Py_DECREF(function);
  //}


  Py_DECREF(PYbases);
  Py_DECREF(PYclass_dict);
  Py_DECREF(PYclass_name);
  Py_DECREF(PYclass_docstring);
}




PyObject* NULLPointer;
PyObject* global_typeinfo_map_temporary_hack;





#if defined (__GNUC__) && ( __GNUC__ >=3)
#include <cxxabi.h>
#endif
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

struct Dummy { virtual ~Dummy() {}; }; // Helps with dynamic type discovery

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

  if (new_class) {
    PyObject_SetAttrString(Pinstance, "__class__", new_class);
  } else {
    std::cout << "Failed to downcast to " << target_type_info.name() << std::endl;
  }
  Py_INCREF(Py_None);
  return Py_None;
}









PyObject* CommonInserter(PyObject* the_class, const void* arg, bool downcast=false) {
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
    ((ProxyObject*)py_instance)->__the_instance__ = (void*)arg;
    if (downcast) {
      dynamicDownCast(NULL, py_instance);
    }
    return py_instance;
  }
  return 0; // Should never reach here.
}

int CommonExtractor(PyObject* arg, void*& address) {
  if (PyObject_IsInstance(arg, (PyObject*)&ProxyBaseObject_Type) <= 0) {
    return 0;
  }
  address = ((ProxyObject*)arg)->__the_instance__;
  if (address) { return 1; }
  PyErr_SetString(PyExc_Exception, "Proxy extraction failed");
  return 0;
}
