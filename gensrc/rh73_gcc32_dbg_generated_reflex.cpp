#define private public
#include <Python.h>
#include <string>
#include <cstdio>
#include <iostream> // for diagnostcs only: remove in production

PyObject* global_typeinfo_map_temporary_hack;
PyObject* dynamicDownCast(PyObject*, PyObject* Pinstance);

PyObject* MODULEPaux;
PyObject* NULLPointer;
PyObject* Reflex_Type_to_Python_proxy_map;
PyObject* auxP;
PyObject* auxC;
PyObject* extractor_map;
PyObject* EnumMetaType;

int Extract_str_As_stdString(PyObject* arg, void*& address) {
      char* Cstr = PyString_AsString(arg);
      std::string* stdStrP = new std::string(Cstr);
      address = stdStrP;
      if (address) { return 1; }
      PyErr_SetString(PyExc_Exception, "Extraction of str as std::string failed.");
      return 0;
    }
    
PyObject* PyStr_FromStdString(std::string arg) {
      return PyString_FromString(arg.c_str());
    }
    
PyObject* PyCObject_FromVoidstar(void* arg) {
      return PyCObject_FromVoidPtr(arg, 0);
    }
    
struct Dummy { virtual ~Dummy() {}; }; // Helps with dynamic type discovery

#include "Reflex/Member.h"
#include "Reflex/Scope.h"
#include "Reflex/Type.h"
#include "Reflex/MemberTemplate.h"
#include "typeinfo"
#include "Reflex/Base.h"
#include "Reflex/TypeTemplate.h"

#include "Caux.h"
PyObject* ClassObject_For_Member;
PyObject* ClassObject_For_Scope;
PyObject* ClassObject_For_Type;
PyObject* ClassObject_For_MemberTemplate;
PyObject* ClassObject_For_type_info;
PyObject* ClassObject_For_Field;
PyObject* ClassObject_For_Base;
PyObject* ClassObject_For_TypeTemplate;
PyObject* ClassObject_For_GenericFunction;
PyObject* seal__reflex__TYPE;
/**********************************************************************/
/*        Member                                                      */
/**********************************************************************/


static PyObject* seal__reflex__Member___parameterCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    bool bool_0 = 0;
    if (PyArg_ParseTuple(args, "i", &bool_0)) {
      try {
        int int_result = (int)((seal::reflex::Member*)((ProxyObject*)self)->__the_instance__)->parameterCount(bool_0);
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'parameterCount'");
  return 0;
}


static PyObject* seal__reflex__Member___name (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        std::string string_result = (std::string)((seal::reflex::Member*)((ProxyObject*)self)->__the_instance__)->name();
        return PyStr_FromStdString(string_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'name'");
  return 0;
}


static PyObject* seal__reflex__Member___templateArgument (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(((seal::reflex::Member*)((ProxyObject*)self)->__the_instance__)->templateArgument(int_0));
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'templateArgument'");
  return 0;
}


static PyObject* seal__reflex__Member___isTemplateInstance (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Member*)((ProxyObject*)self)->__the_instance__)->isTemplateInstance();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isTemplateInstance'");
  return 0;
}


static PyObject* seal__reflex__Member___isConstructor (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Member*)((ProxyObject*)self)->__the_instance__)->isConstructor();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isConstructor'");
  return 0;
}


static PyObject* seal__reflex__Member___type (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(((seal::reflex::Member*)((ProxyObject*)self)->__the_instance__)->type());
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'type'");
  return 0;
}


static PyObject* seal__reflex__Member___templateArgumentCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Member*)((ProxyObject*)self)->__the_instance__)->templateArgumentCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'templateArgumentCount'");
  return 0;
}


static PyMethodDef MethodsStructFor_Member[] = {
  {"parameterCount", seal__reflex__Member___parameterCount, METH_VARARGS, ""},
  {"name", seal__reflex__Member___name, METH_VARARGS, ""},
  {"templateArgument", seal__reflex__Member___templateArgument, METH_VARARGS, ""},
  {"isTemplateInstance", seal__reflex__Member___isTemplateInstance, METH_VARARGS, ""},
  {"isConstructor", seal__reflex__Member___isConstructor, METH_VARARGS, ""},
  {"type", seal__reflex__Member___type, METH_VARARGS, ""},
  {"templateArgumentCount", seal__reflex__Member___templateArgumentCount, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};

/**********************************************************************/
/*        Scope                                                       */
/**********************************************************************/


static PyObject* seal__reflex__Scope___dataMemberCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->dataMemberCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'dataMemberCount'");
  return 0;
}


static PyObject* seal__reflex__Scope___memberTemplateCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->memberTemplateCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'memberTemplateCount'");
  return 0;
}


static PyObject* seal__reflex__Scope___isUnion (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->isUnion();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isUnion'");
  return 0;
}


static PyObject* seal__reflex__Scope___subType (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->subType(int_0));
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'subType'");
  return 0;
}


static PyObject* seal__reflex__Scope___isNamespace (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->isNamespace();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isNamespace'");
  return 0;
}


static PyObject* seal__reflex__Scope___subScope (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Scope* Scope_result = new seal::reflex::Scope(((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->subScope(int_0));
        
        return CommonInserter(ClassObject_For_Scope, Scope_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'subScope'");
  return 0;
}


static PyObject* seal__reflex__Scope___scopeType (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        seal::reflex::TYPE* TYPE_result = new seal::reflex::TYPE(((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->scopeType());
          return PyObject_CallFunction(seal__reflex__TYPE, "i", *TYPE_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'scopeType'");
  return 0;
}


static PyObject* seal__reflex__Scope___member (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Member* Member_result = new seal::reflex::Member(((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->member(int_0));
        
        return CommonInserter(ClassObject_For_Member, Member_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'member'");
  return 0;
}


static PyObject* seal__reflex__Scope___isClass (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->isClass();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isClass'");
  return 0;
}


static PyObject* seal__reflex__Scope___memberCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->memberCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'memberCount'");
  return 0;
}


static PyObject* seal__reflex__Scope___id (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        void* voidstar_result = (void*)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->id();
        return PyCObject_FromVoidstar(voidstar_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'id'");
  return 0;
}


static PyObject* seal__reflex__Scope___functionMember (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Member* Member_result = new seal::reflex::Member(((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->functionMember(int_0));
        
        return CommonInserter(ClassObject_For_Member, Member_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'functionMember'");
  return 0;
}


static PyObject* seal__reflex__Scope___isEnum (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->isEnum();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isEnum'");
  return 0;
}


static PyObject* seal__reflex__Scope___typeTemplateCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->typeTemplateCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'typeTemplateCount'");
  return 0;
}


static PyObject* seal__reflex__Scope___typeTemplate (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::TypeTemplate* TypeTemplate_result = new seal::reflex::TypeTemplate(((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->typeTemplate(int_0));
        
        return CommonInserter(ClassObject_For_TypeTemplate, TypeTemplate_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'typeTemplate'");
  return 0;
}


static PyObject* seal__reflex__Scope___subTypeCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->subTypeCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'subTypeCount'");
  return 0;
}


static PyObject* seal__reflex__Scope___memberTemplate (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::MemberTemplate* MemberTemplate_result = new seal::reflex::MemberTemplate(((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->memberTemplate(int_0));
        
        return CommonInserter(ClassObject_For_MemberTemplate, MemberTemplate_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'memberTemplate'");
  return 0;
}


static PyObject* seal__reflex__Scope___dataMember (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Member* Member_result = new seal::reflex::Member(((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->dataMember(int_0));
        
        return CommonInserter(ClassObject_For_Member, Member_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'dataMember'");
  return 0;
}


static PyObject* seal__reflex__Scope___name (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        std::string string_result = (std::string)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->name(int_0);
        return PyStr_FromStdString(string_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'name'");
  return 0;
}


static PyObject* seal__reflex__Scope___functionMemberCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->functionMemberCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'functionMemberCount'");
  return 0;
}


static PyObject* seal__reflex__Scope___subScopeCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Scope*)((ProxyObject*)self)->__the_instance__)->subScopeCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'subScopeCount'");
  return 0;
}


static PyObject* seal__reflex__Scope___byName (PyObject*, PyObject* args) {
  // Static method: no self.  
  {
    std::string* string_0 = 0;
    if (PyArg_ParseTuple(args, "O&", Extract_str_As_stdString, &string_0)) {
      try {
        seal::reflex::Scope* Scope_result = new seal::reflex::Scope(seal::reflex::Scope::byName(*string_0));
        
        return CommonInserter(ClassObject_For_Scope, Scope_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'byName'");
  return 0;
}


static PyMethodDef MethodsStructFor_Scope[] = {
  {"dataMemberCount", seal__reflex__Scope___dataMemberCount, METH_VARARGS, ""},
  {"memberTemplateCount", seal__reflex__Scope___memberTemplateCount, METH_VARARGS, ""},
  {"isUnion", seal__reflex__Scope___isUnion, METH_VARARGS, ""},
  {"subType", seal__reflex__Scope___subType, METH_VARARGS, ""},
  {"isNamespace", seal__reflex__Scope___isNamespace, METH_VARARGS, ""},
  {"subScope", seal__reflex__Scope___subScope, METH_VARARGS, ""},
  {"scopeType", seal__reflex__Scope___scopeType, METH_VARARGS, ""},
  {"member", seal__reflex__Scope___member, METH_VARARGS, ""},
  {"isClass", seal__reflex__Scope___isClass, METH_VARARGS, ""},
  {"memberCount", seal__reflex__Scope___memberCount, METH_VARARGS, ""},
  {"id", seal__reflex__Scope___id, METH_VARARGS, ""},
  {"functionMember", seal__reflex__Scope___functionMember, METH_VARARGS, ""},
  {"isEnum", seal__reflex__Scope___isEnum, METH_VARARGS, ""},
  {"typeTemplateCount", seal__reflex__Scope___typeTemplateCount, METH_VARARGS, ""},
  {"typeTemplate", seal__reflex__Scope___typeTemplate, METH_VARARGS, ""},
  {"subTypeCount", seal__reflex__Scope___subTypeCount, METH_VARARGS, ""},
  {"memberTemplate", seal__reflex__Scope___memberTemplate, METH_VARARGS, ""},
  {"dataMember", seal__reflex__Scope___dataMember, METH_VARARGS, ""},
  {"name", seal__reflex__Scope___name, METH_VARARGS, ""},
  {"functionMemberCount", seal__reflex__Scope___functionMemberCount, METH_VARARGS, ""},
  {"subScopeCount", seal__reflex__Scope___subScopeCount, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};


static PyMethodDef StaticMethodsStructFor_Scope[] = {
  {"byName", seal__reflex__Scope___byName, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};

/**********************************************************************/
/*        Type                                                        */
/**********************************************************************/


static PyObject* seal__reflex__Type___typeTypeAsString (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        std::string string_result = (std::string)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->typeTypeAsString();
        return PyStr_FromStdString(string_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'typeTypeAsString'");
  return 0;
}


static PyObject* seal__reflex__Type___baseCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->baseCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'baseCount'");
  return 0;
}


static PyObject* seal__reflex__Type___id (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        void* voidstar_result = (void*)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->id();
        return PyCObject_FromVoidstar(voidstar_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'id'");
  return 0;
}


static PyObject* seal__reflex__Type___typeInfo (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        std::type_info* type_info_result = (std::type_info*)&(((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->typeInfo());
        
        return CommonInserter(ClassObject_For_type_info, type_info_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'typeInfo'");
  return 0;
}


static PyObject* seal__reflex__Type___isClass (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->isClass();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isClass'");
  return 0;
}


static PyObject* seal__reflex__Type___memberCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->memberCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'memberCount'");
  return 0;
}


static PyObject* seal__reflex__Type___toType (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->toType());
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'toType'");
  return 0;
}


static PyObject* seal__reflex__Type___member (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Member* Member_result = new seal::reflex::Member(((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->member(int_0));
        
        return CommonInserter(ClassObject_For_Member, Member_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'member'");
  return 0;
}


static PyObject* seal__reflex__Type___returnType (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->returnType());
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'returnType'");
  return 0;
}


static PyObject* seal__reflex__Type___parameterCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->parameterCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'parameterCount'");
  return 0;
}


static PyObject* seal__reflex__Type___templateArgumentCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->templateArgumentCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'templateArgumentCount'");
  return 0;
}


static PyObject* seal__reflex__Type___base (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Base* Base_result = new seal::reflex::Base(((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->base(int_0));
        
        return CommonInserter(ClassObject_For_Base, Base_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'base'");
  return 0;
}


static PyObject* seal__reflex__Type___templateArgument (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->templateArgument(int_0));
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'templateArgument'");
  return 0;
}


static PyObject* seal__reflex__Type___isReference (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->isReference();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isReference'");
  return 0;
}


static PyObject* seal__reflex__Type___isTypedef (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->isTypedef();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isTypedef'");
  return 0;
}


static PyObject* seal__reflex__Type___name (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        std::string string_result = (std::string)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->name(int_0);
        return PyStr_FromStdString(string_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'name'");
  return 0;
}


static PyObject* seal__reflex__Type___typeType (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        seal::reflex::TYPE* TYPE_result = new seal::reflex::TYPE(((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->typeType());
          return PyObject_CallFunction(seal__reflex__TYPE, "i", *TYPE_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'typeType'");
  return 0;
}


static PyObject* seal__reflex__Type___parameter (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->parameter(int_0));
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'parameter'");
  return 0;
}


static PyObject* seal__reflex__Type___isPointer (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        bool bool_result = (bool)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->isPointer();
        return PyInt_FromLong(bool_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'isPointer'");
  return 0;
}


static PyObject* seal__reflex__Type___sizeOf (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Type*)((ProxyObject*)self)->__the_instance__)->sizeOf();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'sizeOf'");
  return 0;
}


static PyObject* seal__reflex__Type___typeCount (PyObject*, PyObject* args) {
  // Static method: no self.  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)seal::reflex::Type::typeCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'typeCount'");
  return 0;
}


static PyObject* seal__reflex__Type___type (PyObject*, PyObject* args) {
  // Static method: no self.  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(seal::reflex::Type::type(int_0));
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'type'");
  return 0;
}


static PyObject* seal__reflex__Type___byName (PyObject*, PyObject* args) {
  // Static method: no self.  
  {
    std::string* string_0 = 0;
    if (PyArg_ParseTuple(args, "O&", Extract_str_As_stdString, &string_0)) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(seal::reflex::Type::byName(*string_0));
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'byName'");
  return 0;
}


static PyMethodDef MethodsStructFor_Type[] = {
  {"typeTypeAsString", seal__reflex__Type___typeTypeAsString, METH_VARARGS, ""},
  {"baseCount", seal__reflex__Type___baseCount, METH_VARARGS, ""},
  {"id", seal__reflex__Type___id, METH_VARARGS, ""},
  {"typeInfo", seal__reflex__Type___typeInfo, METH_VARARGS, ""},
  {"isClass", seal__reflex__Type___isClass, METH_VARARGS, ""},
  {"memberCount", seal__reflex__Type___memberCount, METH_VARARGS, ""},
  {"toType", seal__reflex__Type___toType, METH_VARARGS, ""},
  {"member", seal__reflex__Type___member, METH_VARARGS, ""},
  {"returnType", seal__reflex__Type___returnType, METH_VARARGS, ""},
  {"parameterCount", seal__reflex__Type___parameterCount, METH_VARARGS, ""},
  {"templateArgumentCount", seal__reflex__Type___templateArgumentCount, METH_VARARGS, ""},
  {"base", seal__reflex__Type___base, METH_VARARGS, ""},
  {"templateArgument", seal__reflex__Type___templateArgument, METH_VARARGS, ""},
  {"isReference", seal__reflex__Type___isReference, METH_VARARGS, ""},
  {"isTypedef", seal__reflex__Type___isTypedef, METH_VARARGS, ""},
  {"name", seal__reflex__Type___name, METH_VARARGS, ""},
  {"typeType", seal__reflex__Type___typeType, METH_VARARGS, ""},
  {"parameter", seal__reflex__Type___parameter, METH_VARARGS, ""},
  {"isPointer", seal__reflex__Type___isPointer, METH_VARARGS, ""},
  {"sizeOf", seal__reflex__Type___sizeOf, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};


static PyMethodDef StaticMethodsStructFor_Type[] = {
  {"typeCount", seal__reflex__Type___typeCount, METH_VARARGS, ""},
  {"type", seal__reflex__Type___type, METH_VARARGS, ""},
  {"byName", seal__reflex__Type___byName, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};

/**********************************************************************/
/*        MemberTemplate                                              */
/**********************************************************************/


static PyObject* seal__reflex__MemberTemplate___name (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        std::string string_result = (std::string)((seal::reflex::MemberTemplate*)((ProxyObject*)self)->__the_instance__)->name(int_0);
        return PyStr_FromStdString(string_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'name'");
  return 0;
}


static PyObject* seal__reflex__MemberTemplate___instantiationCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::MemberTemplate*)((ProxyObject*)self)->__the_instance__)->instantiationCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'instantiationCount'");
  return 0;
}


static PyObject* seal__reflex__MemberTemplate___instantiation (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Member* Member_result = new seal::reflex::Member(((seal::reflex::MemberTemplate*)((ProxyObject*)self)->__the_instance__)->instantiation(int_0));
        
        return CommonInserter(ClassObject_For_Member, Member_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'instantiation'");
  return 0;
}


static PyMethodDef MethodsStructFor_MemberTemplate[] = {
  {"name", seal__reflex__MemberTemplate___name, METH_VARARGS, ""},
  {"instantiationCount", seal__reflex__MemberTemplate___instantiationCount, METH_VARARGS, ""},
  {"instantiation", seal__reflex__MemberTemplate___instantiation, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};


static PyMethodDef StaticMethodsStructFor_MemberTemplate[] = {
  
  { NULL } /* Sentinel */
};

/**********************************************************************/
/*        type_info                                                   */
/**********************************************************************/


static PyObject* std__type_info___name (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        std::string string_result = (std::string)((std::type_info*)((ProxyObject*)self)->__the_instance__)->name();
        return PyStr_FromStdString(string_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'name'");
  return 0;
}


static PyMethodDef MethodsStructFor_type_info[] = {
  {"name", std__type_info___name, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};

/**********************************************************************/
/*        Field                                                       */
/**********************************************************************/


static PyObject* __Field_____set__ (PyObject*, PyObject* args) {
   // "Funny" method: need self in 2 places
  PyObject* self = PyTuple_GetItem(args, 0);
  return ((::Field*)((ProxyObject*)self)->__the_instance__)->__set__(args);
}


static PyObject* __Field_____init__ (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    seal::reflex::Member* Member_0 = 0;
    int int_1 = 0;
    if (PyArg_ParseTuple(args, "O&i", CommonExtractor, &Member_0, &int_1)) {
      try {
        ((ProxyObject*)self)->__the_instance__ = new ::Field(*Member_0, int_1);
        Py_INCREF(Py_None);
        return Py_None;
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to '__init__'");
  return 0;
}


static PyObject* __Field_____get__ (PyObject*, PyObject* args) {
   // "Funny" method: need self in 2 places
  PyObject* self = PyTuple_GetItem(args, 0);
  return ((::Field*)((ProxyObject*)self)->__the_instance__)->__get__(args);
}


static PyMethodDef MethodsStructFor_Field[] = {
  {"__set__", __Field_____set__, METH_VARARGS, ""},
  {"__init__", __Field_____init__, METH_VARARGS, ""},
  {"__get__", __Field_____get__, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};


static PyMethodDef StaticMethodsStructFor_Field[] = {
  
  { NULL } /* Sentinel */
};

/**********************************************************************/
/*        Base                                                        */
/**********************************************************************/


static PyObject* seal__reflex__Base___offset (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::Base*)((ProxyObject*)self)->__the_instance__)->offset();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'offset'");
  return 0;
}


static PyObject* seal__reflex__Base___toType (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(((seal::reflex::Base*)((ProxyObject*)self)->__the_instance__)->toType());
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'toType'");
  return 0;
}


static PyMethodDef MethodsStructFor_Base[] = {
  {"offset", seal__reflex__Base___offset, METH_VARARGS, ""},
  {"toType", seal__reflex__Base___toType, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};

/**********************************************************************/
/*        TypeTemplate                                                */
/**********************************************************************/


static PyObject* seal__reflex__TypeTemplate___name (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        std::string string_result = (std::string)((seal::reflex::TypeTemplate*)((ProxyObject*)self)->__the_instance__)->name(int_0);
        return PyStr_FromStdString(string_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'name'");
  return 0;
}


static PyObject* seal__reflex__TypeTemplate___instantiationCount (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    if (PyTuple_Size(args) == 0) {
      try {
        int int_result = (int)((seal::reflex::TypeTemplate*)((ProxyObject*)self)->__the_instance__)->instantiationCount();
        return PyInt_FromLong(int_result); 
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'instantiationCount'");
  return 0;
}


static PyObject* seal__reflex__TypeTemplate___instantiation (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    int int_0 = 0;
    if (PyArg_ParseTuple(args, "i", &int_0)) {
      try {
        seal::reflex::Type* Type_result = new seal::reflex::Type(((seal::reflex::TypeTemplate*)((ProxyObject*)self)->__the_instance__)->instantiation(int_0));
        
        return CommonInserter(ClassObject_For_Type, Type_result, false);
             
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'instantiation'");
  return 0;
}


static PyMethodDef MethodsStructFor_TypeTemplate[] = {
  {"name", seal__reflex__TypeTemplate___name, METH_VARARGS, ""},
  {"instantiationCount", seal__reflex__TypeTemplate___instantiationCount, METH_VARARGS, ""},
  {"instantiation", seal__reflex__TypeTemplate___instantiation, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};


static PyMethodDef StaticMethodsStructFor_TypeTemplate[] = {
  
  { NULL } /* Sentinel */
};

/**********************************************************************/
/*        GenericFunction                                             */
/**********************************************************************/


static PyObject* __GenericFunction___add_method (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    seal::reflex::Member* Member_0 = 0;
    if (PyArg_ParseTuple(args, "O&", CommonExtractor, &Member_0)) {
      try {
        ((::GenericFunction*)((ProxyObject*)self)->__the_instance__)->add_method(*Member_0);
        Py_INCREF(Py_None);
        return Py_None;
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to 'add_method'");
  return 0;
}


static PyObject* __GenericFunction_____call__ (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);
  return ((::GenericFunction*)((ProxyObject*)self)->__the_instance__)->__call__(args);
}


static PyObject* __GenericFunction_____init__ (PyObject*, PyObject* args) {
  PyObject* self = PyTuple_GetItem(args, 0);
  args = PyTuple_GetSlice(args, 1, 999999);  
  {
    std::string* string_0 = 0;
    if (PyArg_ParseTuple(args, "O&", Extract_str_As_stdString, &string_0)) {
      try {
        ((ProxyObject*)self)->__the_instance__ = new ::GenericFunction(*string_0);
        Py_INCREF(Py_None);
        return Py_None;
      } catch (...) {
        PyErr_SetString(PyExc_Exception, "C++ exception");
        return 0;
      }
    } else {
      PyErr_Clear();
    }
  }
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to '__init__'");
  return 0;
}


static PyObject* __GenericFunction_____get__ (PyObject*, PyObject* args) {
   // "Funny" method: need self in 2 places
  PyObject* self = PyTuple_GetItem(args, 0);
  return ((::GenericFunction*)((ProxyObject*)self)->__the_instance__)->__get__(args);
}


static PyMethodDef MethodsStructFor_GenericFunction[] = {
  {"add_method", __GenericFunction___add_method, METH_VARARGS, ""},
  {"__call__", __GenericFunction_____call__, METH_VARARGS, ""},
  {"__init__", __GenericFunction_____init__, METH_VARARGS, ""},
  {"__get__", __GenericFunction_____get__, METH_VARARGS, ""},
  { NULL } /* Sentinel */
};


static PyMethodDef StaticMethodsStructFor_GenericFunction[] = {
  
  { NULL } /* Sentinel */
};


static PyMethodDef module_methods_For_reflex[] = {
  {"funkize", funkize, METH_O, "funkizes a tuple"},
  {"cobjectid", cobjectid, METH_O, "PyCObject_AsVoidPtr ... as Python int"},
  {"Type__hash__", reflex_Type_hash, METH_O, ""},
  {"proxyobject_newOBJECT", proxyobject_newOBJECT, METH_VARARGS, ""},
  {"Type__eq__", reflex_Type_eq, METH_VARARGS, ""},
  {NULL} /* Sentinel */
};

/**********************************************************************/
/**********************************************************************/
/**********************************************************************/

#ifndef PyMODINT_FUNC /* declarations for DLL import/export */
#define PyMODINT_FUNC void
#endif


PyMODINIT_FUNC
initreflex(void) {

  PyObject* this_module = Py_InitModule3("reflex",
                                          module_methods_For_reflex,
                                          "None");
  PyObject* module_dict = PyModule_GetDict(this_module);
  PyObject* typeinfo_to_class_dict = PyDict_New();
  PyDict_SetItemString(module_dict, "typeinfo_to_class_dict", typeinfo_to_class_dict);
  global_typeinfo_map_temporary_hack = typeinfo_to_class_dict;


  PyObject* class_dict;
  PyObject* class_name;  
  PyObject* bases;
  PyObject* docstring;

  
  MODULEPaux = PyImport_ImportModule("Paux");
  if (!MODULEPaux) {
    return;
  }
  PyDict_SetItemString(module_dict, "Paux", MODULEPaux);
  NULLPointer = PyObject_GetAttrString(MODULEPaux, "NULLPointer");
  Reflex_Type_to_Python_proxy_map = PyObject_GetAttrString(MODULEPaux, "Reflex_Type_to_Python_proxy_map");
  auxP = PyObject_GetAttrString(MODULEPaux, "auxP");
  auxC = PyObject_GetAttrString(MODULEPaux, "auxC");
  extractor_map = PyObject_GetAttrString(MODULEPaux, "extractor_map");
  EnumMetaType = PyObject_GetAttrString(MODULEPaux, "Enum");
  
  /******************* seal::reflex::Member ****************/
  bases = PyTuple_New(1);
  PyTuple_SetItem(bases, 0, (PyObject*)&ProxyBaseObject_Type);

  class_dict = PyDict_New();
  docstring = PyString_FromString("None");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("Member");
  ClassObject_For_Member =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(ClassObject_For_Member, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(seal::reflex::Member), 0));

  //  Register class in module
  PyDict_SetItemString(module_dict, "Member", ClassObject_For_Member);
  //  Register methods in class
  for (PyMethodDef* method = MethodsStructFor_Member; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject_For_Member);
    PyObject_SetAttrString(ClassObject_For_Member, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }

  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);

  PyDict_SetItemString(typeinfo_to_class_dict, typeid(seal::reflex::Member).name(), ClassObject_For_Member);

  /******************* seal::reflex::Scope ****************/
  bases = PyTuple_New(1);
  PyTuple_SetItem(bases, 0, (PyObject*)&ProxyBaseObject_Type);

  class_dict = PyDict_New();
  docstring = PyString_FromString("None");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("Scope");
  ClassObject_For_Scope =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(ClassObject_For_Scope, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(seal::reflex::Scope), 0));

  //  Register class in module
  PyDict_SetItemString(module_dict, "Scope", ClassObject_For_Scope);
  //  Register methods in class
  for (PyMethodDef* method = MethodsStructFor_Scope; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject_For_Scope);
    PyObject_SetAttrString(ClassObject_For_Scope, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }
  // Register static methods in class
  for (PyMethodDef* method = StaticMethodsStructFor_Scope; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject_SetAttrString(ClassObject_For_Scope, method->ml_name, function);
    Py_DECREF(function);
  }

  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);

  PyDict_SetItemString(typeinfo_to_class_dict, typeid(seal::reflex::Scope).name(), ClassObject_For_Scope);

  /******************* seal::reflex::Type ****************/
  bases = PyTuple_New(1);
  PyTuple_SetItem(bases, 0, (PyObject*)&ProxyBaseObject_Type);

  class_dict = PyDict_New();
  docstring = PyString_FromString("reflex::Type");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("Type");
  ClassObject_For_Type =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(ClassObject_For_Type, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(seal::reflex::Type), 0));

  //  Register class in module
  PyDict_SetItemString(module_dict, "Type", ClassObject_For_Type);
  //  Register methods in class
  for (PyMethodDef* method = MethodsStructFor_Type; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject_For_Type);
    PyObject_SetAttrString(ClassObject_For_Type, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }
  // Register static methods in class
  for (PyMethodDef* method = StaticMethodsStructFor_Type; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject_SetAttrString(ClassObject_For_Type, method->ml_name, function);
    Py_DECREF(function);
  }

  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);

  PyDict_SetItemString(typeinfo_to_class_dict, typeid(seal::reflex::Type).name(), ClassObject_For_Type);

  /******************* seal::reflex::MemberTemplate ****************/
  bases = PyTuple_New(1);
  PyTuple_SetItem(bases, 0, (PyObject*)&ProxyBaseObject_Type);

  class_dict = PyDict_New();
  docstring = PyString_FromString("None");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("MemberTemplate");
  ClassObject_For_MemberTemplate =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(ClassObject_For_MemberTemplate, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(seal::reflex::MemberTemplate), 0));

  //  Register class in module
  PyDict_SetItemString(module_dict, "MemberTemplate", ClassObject_For_MemberTemplate);
  //  Register methods in class
  for (PyMethodDef* method = MethodsStructFor_MemberTemplate; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject_For_MemberTemplate);
    PyObject_SetAttrString(ClassObject_For_MemberTemplate, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }
  // Register static methods in class
  for (PyMethodDef* method = StaticMethodsStructFor_MemberTemplate; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject_SetAttrString(ClassObject_For_MemberTemplate, method->ml_name, function);
    Py_DECREF(function);
  }

  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);

  PyDict_SetItemString(typeinfo_to_class_dict, typeid(seal::reflex::MemberTemplate).name(), ClassObject_For_MemberTemplate);

  /******************* std::type_info ****************/
  bases = PyTuple_New(1);
  PyTuple_SetItem(bases, 0, (PyObject*)&ProxyBaseObject_Type);

  class_dict = PyDict_New();
  docstring = PyString_FromString("C++'s std::type_info");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("type_info");
  ClassObject_For_type_info =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(ClassObject_For_type_info, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(std::type_info), 0));

  //  Register class in module
  PyDict_SetItemString(module_dict, "type_info", ClassObject_For_type_info);
  //  Register methods in class
  for (PyMethodDef* method = MethodsStructFor_type_info; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject_For_type_info);
    PyObject_SetAttrString(ClassObject_For_type_info, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }

  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);

  PyDict_SetItemString(typeinfo_to_class_dict, typeid(std::type_info).name(), ClassObject_For_type_info);

  /******************* ::Field ****************/
  bases = PyTuple_New(1);
  PyTuple_SetItem(bases, 0, (PyObject*)&ProxyBaseObject_Type);

  class_dict = PyDict_New();
  docstring = PyString_FromString("None");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("Field");
  ClassObject_For_Field =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(ClassObject_For_Field, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(::Field), 0));

  //  Register class in module
  PyDict_SetItemString(module_dict, "Field", ClassObject_For_Field);
  //  Register methods in class
  for (PyMethodDef* method = MethodsStructFor_Field; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject_For_Field);
    PyObject_SetAttrString(ClassObject_For_Field, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }
  // Register static methods in class
  for (PyMethodDef* method = StaticMethodsStructFor_Field; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject_SetAttrString(ClassObject_For_Field, method->ml_name, function);
    Py_DECREF(function);
  }

  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);

  PyDict_SetItemString(typeinfo_to_class_dict, typeid(::Field).name(), ClassObject_For_Field);

  /******************* seal::reflex::Base ****************/
  bases = PyTuple_New(1);
  PyTuple_SetItem(bases, 0, (PyObject*)&ProxyBaseObject_Type);

  class_dict = PyDict_New();
  docstring = PyString_FromString("None");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("Base");
  ClassObject_For_Base =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(ClassObject_For_Base, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(seal::reflex::Base), 0));

  //  Register class in module
  PyDict_SetItemString(module_dict, "Base", ClassObject_For_Base);
  //  Register methods in class
  for (PyMethodDef* method = MethodsStructFor_Base; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject_For_Base);
    PyObject_SetAttrString(ClassObject_For_Base, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }

  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);

  PyDict_SetItemString(typeinfo_to_class_dict, typeid(seal::reflex::Base).name(), ClassObject_For_Base);

  /******************* seal::reflex::TypeTemplate ****************/
  bases = PyTuple_New(1);
  PyTuple_SetItem(bases, 0, (PyObject*)&ProxyBaseObject_Type);

  class_dict = PyDict_New();
  docstring = PyString_FromString("None");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("TypeTemplate");
  ClassObject_For_TypeTemplate =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(ClassObject_For_TypeTemplate, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(seal::reflex::TypeTemplate), 0));

  //  Register class in module
  PyDict_SetItemString(module_dict, "TypeTemplate", ClassObject_For_TypeTemplate);
  //  Register methods in class
  for (PyMethodDef* method = MethodsStructFor_TypeTemplate; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject_For_TypeTemplate);
    PyObject_SetAttrString(ClassObject_For_TypeTemplate, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }
  // Register static methods in class
  for (PyMethodDef* method = StaticMethodsStructFor_TypeTemplate; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject_SetAttrString(ClassObject_For_TypeTemplate, method->ml_name, function);
    Py_DECREF(function);
  }

  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);

  PyDict_SetItemString(typeinfo_to_class_dict, typeid(seal::reflex::TypeTemplate).name(), ClassObject_For_TypeTemplate);

  /******************* ::GenericFunction ****************/
  bases = PyTuple_New(1);
  PyTuple_SetItem(bases, 0, (PyObject*)&ProxyBaseObject_Type);

  class_dict = PyDict_New();
  docstring = PyString_FromString("None");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("GenericFunction");
  ClassObject_For_GenericFunction =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(ClassObject_For_GenericFunction, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(::GenericFunction), 0));

  //  Register class in module
  PyDict_SetItemString(module_dict, "GenericFunction", ClassObject_For_GenericFunction);
  //  Register methods in class
  for (PyMethodDef* method = MethodsStructFor_GenericFunction; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, ClassObject_For_GenericFunction);
    PyObject_SetAttrString(ClassObject_For_GenericFunction, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }
  // Register static methods in class
  for (PyMethodDef* method = StaticMethodsStructFor_GenericFunction; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject_SetAttrString(ClassObject_For_GenericFunction, method->ml_name, function);
    Py_DECREF(function);
  }

  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);

  PyDict_SetItemString(typeinfo_to_class_dict, typeid(::GenericFunction).name(), ClassObject_For_GenericFunction);

  PyObject* EnumDict; PyObject* EnumInt;
/**********************************************************************/
/*        seal::reflex::TYPE                                          */
/**********************************************************************/

  EnumDict = PyDict_New();
  EnumInt = PyInt_FromLong(3);
  PyDict_SetItemString(EnumDict, "FUNCTION", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(8);
  PyDict_SetItemString(EnumDict, "TYPEDEF", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(10);
  PyDict_SetItemString(EnumDict, "INSTANTIATEDTEMPLATECLASS", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(13);
  PyDict_SetItemString(EnumDict, "DATAMEMBER", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(1);
  PyDict_SetItemString(EnumDict, "STRUCT", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(9);
  PyDict_SetItemString(EnumDict, "UNION", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(2);
  PyDict_SetItemString(EnumDict, "ENUM", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(12);
  PyDict_SetItemString(EnumDict, "NAMESPACE", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(14);
  PyDict_SetItemString(EnumDict, "FUNCTIONMEMBER", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(11);
  PyDict_SetItemString(EnumDict, "INSTANTIATEDTEMPLATEFUNCTION", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(15);
  PyDict_SetItemString(EnumDict, "UNRESOLVED", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(5);
  PyDict_SetItemString(EnumDict, "FUNDAMENTAL", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(7);
  PyDict_SetItemString(EnumDict, "POINTERTOMEMBER", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(4);
  PyDict_SetItemString(EnumDict, "ARRAY", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(6);
  PyDict_SetItemString(EnumDict, "POINTER", EnumInt);
  Py_DECREF(EnumInt);
  EnumInt = PyInt_FromLong(0);
  PyDict_SetItemString(EnumDict, "CLASS", EnumInt);
  Py_DECREF(EnumInt);
  seal__reflex__TYPE = PyObject_CallFunction(EnumMetaType, "sO", "TYPE", EnumDict);
  PyDict_SetItemString(module_dict, "TYPE", seal__reflex__TYPE);
  Py_DECREF(EnumDict);

if (PyType_Ready(&FunkyTuple_Type) < 0) {std::cout << "FunkyTuple not Ready" << std::endl; }
//else {std::cout << "FunkyTuple seems to be OK." << std::endl;}
PyDict_SetItemString(module_dict, "Funkytuple", (PyObject*)&FunkyTuple_Type);
if (PyType_Ready(&ProxyBaseObject_Type) < 0) {std::cout << "ProxyBaseObject not Ready" << std::endl; }
//else {std::cout << "ProxyBaseObject seems to be OK." << std::endl;}
PyDict_SetItemString(module_dict, "proxyobject", (PyObject*)&ProxyBaseObject_Type);

}
