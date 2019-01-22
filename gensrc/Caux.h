#ifndef Caux_h
#define Caux_h
#include "Python.h"
#include <typeinfo>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstddef>

#define private public
#include "Reflex/FunctionMember.h"
//#undef private
#include "Reflex/Function.h"
#include "Reflex/Scope.h"
#include "Reflex/Object.h"

// Pyreflex own's
#include "ProxyObject.h"
#include "DispatcherTuple.h"
#include "DyCast.h"
#include "Converters.h"
#include "GenericFunction.h"
#include "Field.h"

PyObject* cobjectid(PyObject*, PyObject* arg) {
  return PyInt_FromLong((long)PyCObject_AsVoidPtr(arg));
}

#ifdef __APPLE__
#include <dlfcn.h>
PyObject* loadDictApple(PyObject*, PyObject* arg) {
  std::string dict = PyString_AsString(arg);
  void* ret = dlopen((dict+".dylib").c_str(), RTLD_GLOBAL);
  if (ret) {
    std::cout << "loaded " << dict << std::endl;
    dlsym(ret, "SEAL_CAPABILITIES");
  } else {
    std::cout << "******** Failed to load dict " + dict << std::endl;
  }
  int iret = (int)ret;
  return PyInt_FromLong(iret);
}
#endif

PyObject* GetAddress(PyObject*, PyObject* arg) {
  long pointer = (long)((ProxyObject*)arg)->__the_instance__;
  if (!pointer) {
    std::cout << "GetAddress returning NULL poniter!" << std::endl;
  }
  return PyInt_FromLong(pointer);
}

PyObject* reflex_Type_hash(PyObject*, PyObject* self) {
  return PyInt_FromLong((long) (((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->id()));
}

PyObject* reflex_Type_eq(PyObject*, PyObject* args) {
  PyObject* self  = PyTuple_GetItem(args, 0);
  PyObject* other = PyTuple_GetItem(args, 1);
  bool eq = ((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->id() ==
    ((seal::reflex::Type*)((ProxyObject*)other)->__the_instance__)->id();
  return PyInt_FromLong((long)eq);
}

#endif
