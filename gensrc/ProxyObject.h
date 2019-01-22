#ifndef ProxyObject_h
#define ProxyObject_h

// Base Class of all Proxy types in Pyreflex.  This class is used in
// two different contexts: 
// 1) The static binding of Reflex itself (uses the __the_instance__)
// 2) The dynamic binding created by Pyreflex (uses __Object__)
//  The two usages should ideally be given separate classes

typedef struct {
  PyObject_HEAD
  void* __the_instance__;
  seal::reflex::Object __Object__;
  bool python_owns;
} ProxyObject;

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

// Newing Proxies used in static binding (Reflex binding)

static PyObject*
proxyobject_new(PyObject*, PyObject* args) {
  PyObject* ClassProxy = PyTuple_GetItem(args, 0);
  // Ignoring all but the first argument
  PyObject* new_instance = PyObject_CallMethod((PyObject*)&ProxyBaseObject_Type, "__new__", "O", ClassProxy);
  PyObject* ReflexTypeProxy = PyObject_GetAttrString(ClassProxy, "__Reflex_Type__");
  void* allocation = ((seal::reflex::Type*)((ProxyObject*)ReflexTypeProxy)->__the_instance__)->allocate();
  ((ProxyObject*)new_instance)->__the_instance__ = allocation;
  return new_instance;
}

// Newing Proxies used in dynamic binding (Pyreflex)

static PyObject*
proxyobject_newOBJECT(PyObject*, PyObject* args) {
  PyObject* ClassProxy = PyTuple_GetItem(args, 0);
  // Ignoring all but the first argument
  PyObject* new_instance = PyObject_CallMethod((PyObject*)&ProxyBaseObject_Type, "__new__", "O", ClassProxy);
  PyObject* ReflexTypeProxy = PyObject_GetAttrString(ClassProxy, "__Reflex_Type__");
  seal::reflex::Type xxx = *((seal::reflex::Type*)((ProxyObject*)ReflexTypeProxy)->__the_instance__);
  void* allocation = xxx.allocate();
  ((ProxyObject*)new_instance)->__Object__ = seal::reflex::Object(xxx, allocation);


  // This is checked by __del__ to decide whether the destructor
  // should be called. It should be true only for instances
  // constructed in Python and for instances received BY VALUE from
  // methods or fields.  Set it to true here, and
  // ClassInserter sets it to false if the type is a pointer or a
  // reference.
  ((ProxyObject*)new_instance)->python_owns = true;
  // The User may inspect this field via pyrflex.does_python_own, and
  // may set it via pyreflex.take_ownership and
  // pyreflex.drop_ownership.

  return new_instance;
}


struct DEL {
  seal::reflex::Member destructor;

  DEL(seal::reflex::Member d) : destructor(d) {}

  PyObject* __call__(PyObject* args) {
    PyObject* self = PyTuple_GetItem(args, 0);
    if ( ((ProxyObject*)self)->python_owns) {
      seal::reflex::Object obj = ((ProxyObject*)self)->__Object__;
      this->destructor.invoke(obj, std::vector<void*>());
    }
    Py_INCREF(Py_None);
    return Py_None;
  }

  // Implement the descriptor protocol for turning this fn into a
  // bound/unbound method.
  PyObject* __get__(PyObject *args) {
        // The GenericFunction itself:
    PyObject* func = PyTuple_GetItem(args, 0);
    // The instance through which we were accessed.
    PyObject* obj  = PyTuple_GetItem(args, 1);
    // The class through which we were accessed.
    PyObject* type = PyTuple_GetItem(args, 2);
    // no instance  => unbound method (accessed through class).
    if (obj == Py_None)
      obj = NULL;
    return PyMethod_New(func, obj, type);
  }

};

static PyObject*
take_ownership(PyObject*, PyObject* obj) {
  ((ProxyObject*)obj)->python_owns = true;
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject*
drop_ownership(PyObject*, PyObject* obj) {
  ((ProxyObject*)obj)->python_owns = false;  
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject*
does_proxy_own(PyObject*, PyObject* obj) {
  if ( ((ProxyObject*)obj)->python_owns ) {
    Py_INCREF(Py_True);
    return Py_True;
  } else { 
    Py_INCREF(Py_False);
    return Py_False;
  }
}


#endif
