# $Id: godel.py,v 1.7 2005/03/03 09:24:25 jacek Exp $
import itertools as itt
import sys

def indent(code, N=1):
    addition = ('\n'+(N*indent.size)*' ')
    split = code.split('\n')
    return addition[1:] + addition.join(split)
indent.size = 2

separator = '/%s/\n' % ('*' * 70)
separator = '%s/*        %%-60s*/\n%s' % (separator, separator)
########################################################################
class Inserter(object):

    #@staticmethod #(2.4 feature)
    def name():
        return "CommonInserter"
    name = staticmethod(name)
##############################
class MethodsStruct_class(object):

    methods_name = 'methods'

    def method_pointers(self, the_type):
        methods = getattr(the_type, self.methods_name)
        lines = ['{"%s", %s, METH_VARARGS, ""},' % (m.name, m.long_name) for m in methods]
        return '\n'.join(lines)

    def name(self, the_type):
        return "MethodsStructFor_%s" % the_type.__name__

    def generate(self, the_type):
        return """
static PyMethodDef %s[] = {
%s
  { NULL } /* Sentinel */
};
""" % (self.name(the_type), indent(self.method_pointers(the_type)))

MethodsStruct = MethodsStruct_class()
##############################
class StaticMethodsStruct_class(MethodsStruct_class):

    methods_name = 'static_methods'

    def name(self, the_type):
        return "StaticMethodsStructFor_%s" % the_type.__name__

StaticMethodsStruct = StaticMethodsStruct_class()
##############################
class builtins(object):
    class builtin_type(object):

        def __init__(self):
            self.__name__ = self.__class__.__name__

        def insert(self, variable_name):
            return "return %s(%s);" % (self.inserter_name, variable_name)

    class int(builtin_type):
        fully_qualified_name = 'int'
        inserter_name = 'PyInt_FromLong'
        extractor_name = 'INT_should_never_appear'
        _format_code = 'i'
    int = int()

    class double(builtin_type):
        fully_qualified_name = 'double'
        inserter_name = 'PyFloat_FromDouble'
        extractor_name = 'DOUBLE_should_never_appear'
        _format_code = 'd'
    double = double()

    class void(builtin_type):
        fully_qualified_name = 'VOID_should_never_appear'
        inserter_name = 'VOID_should_never_appear'
        extractor_name = 'VOID_should_never_appear'
    void = void()

    class voidstar(builtin_type):
        fully_qualified_name = 'void*'
        inserter_name = 'PyCObject_FromVoidstar'
        extractor_name = 'VOIDSTAR_should_never_appear'
        inserter = """PyObject* %s(void* arg) {
      return PyCObject_FromVoidPtr(arg, 0);
    }
    """ % (inserter_name)
    voidstar = voidstar()

    class string(builtin_type):
        fully_qualified_name = 'std::string'
        inserter_name = 'PyStr_FromStdString'
        extractor_name = 'Extract_str_As_stdString'

        extractor = """int %s(PyObject* arg, void*& address) {
      char* Cstr = PyString_AsString(arg);
      std::string* stdStrP = new std::string(Cstr);
      address = stdStrP;
      if (address) { return 1; }
      PyErr_SetString(PyExc_Exception, "Extraction of str as std::string failed.");
      return 0;
    }
    """ % (extractor_name)

        inserter = """PyObject* %s(std::string arg) {
      return PyString_FromString(arg.c_str());
    }
    """ % (inserter_name)
    string = string()

    class bool(builtin_type):
        fully_qualified_name = 'bool'
        inserter_name = 'PyInt_FromLong'
        extractor_name = 'PyInt_AsLong'
        _format_code = 'i'
    bool = bool()

    #class opaque(builtin_type):
    #    fully_qualified_name = '/*opaque*/void*'
    #    inserter_name = 'Opaque_FromPointer'
    #    inserter = """PyObject* %s(void* pointer) {
    #      return PyCObject
    #    }
    #    """ % (inserter_name)
##############################
def find_in_some_namespace(namespaces, name):
    for namespace in namespaces.split():
        if namespace in globals():
            namespace = globals()[namespace]
            if hasattr(namespace, name):
                return getattr(namespace, name)
    raise Exception, "Could not identify type '%s'." % name
#################
class Parameter(object):

    special_types = 'int void string bool'.split()

    #@staticmethod #(2.4 feature)
    def spec_to_type(spec):
        decorators = Parameter.spec_to_decorators(spec)
        spec = spec.strip('&*')
        if decorators and decorators[-1] == '*' and spec == 'void': # The shortest shortcut available for now !
            return builtins.voidstar
        else:
            return find_in_some_namespace('builtins reflex tester', spec)
    spec_to_type = staticmethod(spec_to_type)
    
    #@staticmethod #(2.4 feature)
    def spec_to_decorators(spec):
        decorators = [] # Probably want to store them in a more structured way later
        while spec[-1] in '*&':
            decorators.append(spec[-1])
            spec = spec[:-1]
        return decorators
    spec_to_decorators = staticmethod(spec_to_decorators)

    #@staticmethod #(2.4 feature)
    def variable_name(spec_type, suffix):
        return '%s_%s' % (spec_type.__name__, suffix)
    variable_name = staticmethod(variable_name)
    
    #@staticmethod #(2.4 feature)
    def variable_is_pointer(variable_type, result=False): # Can we have a better name ?
        if variable_type is builtins.string:
            if result: return False
            else: return True
        return not isinstance(variable_type, builtins.builtin_type)
    variable_is_pointer = staticmethod(variable_is_pointer)

    #@staticmethod #(2.4 feature)
    def declaration(the_type, suffix='result', result=False, initialize=False):
        type_name = the_type.fully_qualified_name
        if Parameter.variable_is_pointer(the_type, result):
            deref = '*'
        else:
            deref = ''
        declaration = '%s%s %s' % (type_name, deref, Parameter.variable_name(the_type, suffix))
        if initialize:
            return declaration + ' = 0'
        return declaration
    declaration = staticmethod(declaration)

    #@staticmethod #(2.4 feature)
    def format_code(the_type):
        try:
            return the_type._format_code
        except AttributeError:
            return 'O&'
    format_code = staticmethod(format_code)
    
    #@staticmethod #(2.4 feature)
    def format_args(the_type, suffix):
        if Parameter.variable_is_pointer(the_type):
            return '%s, &%s' % (the_type.extractor_name, Parameter.variable_name(the_type, suffix))
        return "&%s" % Parameter.variable_name(the_type, suffix)
    format_args = staticmethod(format_args)

    #@staticmethod #(2.4 feature)
    def call_arg(the_type, decorators, suffix):
        if the_type is builtins.string:
            return "*"+ Parameter.variable_name(the_type, suffix)
        if not isinstance(the_type, builtins.builtin_type):
            if not decorators or decorators[0] in ['', '&']:
                return "*"+ Parameter.variable_name(the_type, suffix)
        return Parameter.variable_name(the_type, suffix)
    call_arg = staticmethod(call_arg)
    
    #@staticmethod #(2.4 feature)
    def type_name(spec):
        return Parameter.spec_to_type(spec).fully_qualified_name
    type_name = staticmethod(type_name)
    
    #@staticmethod #(2.4 feature)
    def insert(RTD, decorators): # RTD: return_type_description
        variable_name = Parameter.variable_name(RTD, 'result')
        if isinstance(RTD, builtins.builtin_type) or isinstance(RTD, Enum):
            return RTD.insert(variable_name)
        if RTD.__has_vtable__ and decorators:
            downcast_p = "true"
        else:
            downcast_p = "false"
        return """
return %s(%s, %s, %s);
    """ % (Inserter.name(), RTD.class_object_name, variable_name, downcast_p)
    insert = staticmethod(insert)
##############################
class SingleCallable(object):

    def __init__(self, owning_type, name, parameter_types):
        self.result = parameter_types[0]
        self.parameter_types = parameter_types
        self.name = name
        self.owning_type = owning_type

    def parameters(self, parameter_types):
        return parameter_types[1:]

    def declarations(self, parameters):
        return ';\n'.join([Parameter.declaration(Parameter.spec_to_type(p), str(n), initialize=True) for n,p in enumerate(parameters)])+';'

    def format_string(self, parameters):
        return ''.join([Parameter.format_code(Parameter.spec_to_type(p)) for p in parameters])

    def format_args(self, parameters):
        return ', '.join([Parameter.format_args(Parameter.spec_to_type(p), n) for n,p in enumerate(parameters)])

    def invoke_and_return(self, owning_type, parameters):
        raise NotImplementedError, "This is an abstract method, subclasses implement it."

    def call_args(self, parameters):
        return ', '.join([Parameter.call_arg(Parameter.spec_to_type(p), Parameter.spec_to_decorators(p), n) for n,p in enumerate(parameters)])

    def generate(self):
        parameters = self.parameters(self.parameter_types)
        if parameters:
            return self.generate_with_arguments(self.owning_type, self.name, parameters)
        return self.generate_without_arguments(self.owning_type, self.name)

    # Temporary hack
    def cast_away_const(self):
        return "(%s)" % ' '.join(Parameter.declaration(Parameter.spec_to_type(self.result), result=True).split()[:-1])

    def generate_with_arguments(self, owning_type, name, parameters):
        return """
{
%s
  if (PyArg_ParseTuple(args, "%s", %s)) {
    try {
%s
    } catch (...) {
      PyErr_SetString(PyExc_Exception, "C++ exception");
      return 0;
    }
  } else {
    PyErr_Clear();
  }
}""" % (indent(self.declarations(parameters)),
        self.format_string(parameters), self.format_args(parameters),
        indent(self.invoke_and_return(owning_type, name, parameters),3))

    def generate_without_arguments(self, owning_type, name):
        return """
{
  if (PyTuple_Size(args) == 0) {
    try {
%s
    } catch (...) {
      PyErr_SetString(PyExc_Exception, "C++ exception");
      return 0;
    }
  }
}""" % (indent(self.invoke_and_return(owning_type, name, ()),3))
##############################
class SingleDestructor(SingleCallable): 

    def generate(self):
        return self.generate_without_arguments(self.owning_type, self.name)

    def invoke_and_return(self, owning_type, name, parameters):
        return """delete ((%s*)((ProxyObject*)self)->__the_instance__);
Py_INCREF(Py_None);
return Py_None;""" % (owning_type.fully_qualified_name)
##############################
class SingleConstructor(SingleCallable):

    def invoke_and_return(self, owning_type, name, parameters):
        return """((ProxyObject*)self)->__the_instance__ = new %s(%s);
Py_INCREF(Py_None);
return Py_None;""" % (owning_type.fully_qualified_name, self.call_args(parameters))
##############################
class SingleMethod(SingleCallable):

    def invoke_and_return(self, owning_type, name, parameters):
        result_type = Parameter.spec_to_type(self.result)
        result_decorators = Parameter.spec_to_decorators(self.result)
        method_call = "%s(%s)" % (self.access_method(owning_type, name), self.call_args(parameters))
        # Nothing returned
        if result_type is builtins.void:
            return """%s;
Py_INCREF(Py_None);
return Py_None;""" % (method_call,)            
        # Returning reflected types ...
        if not isinstance(result_type, builtins.builtin_type):

            convert_to_pointer = None
            # ... by value
            if not result_decorators:
                convert_to_pointer = "new %s" % result_type.fully_qualified_name
            # ... by reference
            if result_decorators == ['&']:
                convert_to_pointer = "%s&" % self.cast_away_const()

            if convert_to_pointer:
                return """%s = %s(%s);
%s """ % (Parameter.declaration(result_type, result=True), convert_to_pointer, method_call,
          Parameter.insert(result_type, result_decorators))

        # Return builtins or pointers to reflected types
        return """%s = %s%s;
%s """ % (Parameter.declaration(result_type, result=True), self.cast_away_const(), method_call,
          Parameter.insert(result_type, result_decorators))

    #@staticmethod #(2.4 feature)
    def access_method(owning_type, name):
        return '((%s*)((ProxyObject*)self)->__the_instance__)->%s' % (owning_type.fully_qualified_name, name)
    access_method = staticmethod(access_method)
##############################
class SingleStaticMethod(SingleMethod):

    def access_method(self, owning_type, name):
        return '%s::%s' % (owning_type.fully_qualified_name, name)
##############################
class Callable(object):

    def __init__(self, *signatures):
        self.signatures = [s.split() for s in signatures]
        # NB self.name and self.type are set externally by the
        # enclosing type, later. This cannot be done at construction
        # time, as the type has not been defined yet.
        self.type = "Not known yet"
        self.name = "Not known yet"

    #@property #(2.4 feature)
    def long_name(self):
        return "%s___%s" % (self.type.fully_qualified_name.replace(':','_'), self.name.replace(' ', '_'))
    long_name = property(long_name)
    
    def signature(self, long_name):
        return 'PyObject* %s (PyObject*, PyObject* args)' % long_name

    def separate_self(self):
        return """PyObject* self = PyTuple_GetItem(args, 0);
args = PyTuple_GetSlice(args, 1, 999999);"""

    def generate(self):
        method_calls = '\n'.join(self.method_calls())
        return """
static %s {
%s%s
  PyObject_Print(args, stdout, 0);
  PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for call to '%s'");
  return 0;
}
""" % (self.signature(self.long_name), indent(self.separate_self()), indent(method_calls), self.name)

    def method_calls(self):
        return [self.single(self.type, self.name, sig).generate() for sig in self.signatures]
##############################
class Destructor(Callable):
    single = SingleDestructor
##############################
class Constructor(Callable):
    single = SingleConstructor
##############################
class Method(Callable):
    single = SingleMethod
##############################
class StaticMethod(Callable):

    single = SingleStaticMethod

    def separate_self(self):
        return "// Static method: no self."
##############################
class PythonFunction(Callable):

    def generate(self):
        return """
static %s {
%s
  return %s(args);
}
""" % (self.signature(self.long_name), indent(self.separate_self()),
       SingleMethod.access_method(self.type, self.name,))
##############################
# Quick hack for binding GenericFunction.__get__. Should work out what
# this actually means; it could well be a static function.
class FunnyPythonFunction(PythonFunction):

    def separate_self(self):
        return """ // "Funny" method: need self in 2 places
PyObject* self = PyTuple_GetItem(args, 0);"""
##############################
class Misc(object):

    #@staticmethod #(2.4 feature)
    def typeinfo_to_class_map_name():
        return "typeinfo_to_class_dict"
    typeinfo_to_class_map_name = staticmethod(typeinfo_to_class_map_name)
    
    #@staticmethod #(2.4 feature)
    def type_identifier_name(the_type):
        return the_type.fully_qualified_name.replace("::", '__')
    type_identifier_name = staticmethod(type_identifier_name)

Misc = Misc()
################ The metaclass which coordinates it all #############
class ExtensionType(type):

    """Metaclass for creating Python extension types based on existing
    C++ types."""

    inserter = Inserter

    def __init__(self, name, bases, dictn):
        dictn['__del__'] = Destructor('void')
        self.set_names()
        self.collect_methods(dictn)
        if not hasattr(self, '__has_vtable__'):
            self.__has_vtable__ = True

    def set_names(self):
        name = self.__name__

        if hasattr(self, '__namespace__'):
            self.fully_qualified_name = self.__namespace__ + '::' + name
        else:
            self.fully_qualified_name = name

        self.extractor_name = "CommonExtractor"
        self.class_object_name = "ClassObject_For_%s" % name

    def collect_methods(self, dictn):
        self.methods = []
        self.static_methods = []
        for name, method in dictn.items():
            if isinstance(method, Callable):
                method.type = self
                method.name = name
                if isinstance(method, StaticMethod):
                    self.static_methods.append(method)
                else:
                    self.methods.append(method)
        
    def register_headers_in(self, module):
        # Collect necessary headers, without duplication
        for header in self.__headers__:
            if not header in module.headers:
                module.headers.append(header)

    def generate_code(self):
        print separator % self.__name__
        for method in itt.chain(self.methods, self.static_methods):
            print method.generate()
        print MethodsStruct.generate(self)
        if self.static_methods or self.__has_vtable__:
            print StaticMethodsStruct.generate(self)
######################################################################
class BuiltinFunction(object):

    def __init__(self, name, argument_style, doc):
        self.name = name
        self.style = argument_style
        self.doc = doc

    def method_struct_line(self, name_in_module):
        return """{"%s", %s, METH_%s, "%s"},""" % (name_in_module, self.name, self.style, self.doc)
######################################################################
class IncludeHeader(object):

    def __init__(self, name):
        self.name = name

class IncludeImplementation(IncludeHeader):

    pass

######################################################################
class Import(object):

    def __init__(self, name):
        self.module_name = name
        self.module_handle = "MODULE%s" % name
        self.imported_attributes = []
        
    def declare_global_handles(self):
        declarations = ["PyObject* %s;" % l for a,l in self.imported_attributes]
        declarations.insert(0, "PyObject* %s;" % self.module_handle)
        return '\n'.join(declarations)

    def import_module(self):
        template = """%s = PyObject_GetAttrString(%s, "%s");"""
        attribute_imports = '\n'.join([template % (l, self.module_handle, a) for a,l in self.imported_attributes])

        return """
%s = PyImport_ImportModule("%s");
if (!%s) {
  return;
}
PyDict_SetItemString(module_dict, "%s", %s);
%s
""" % (self.module_handle, self.module_name,
       self.module_handle,
       self.module_name, self.module_handle,
       attribute_imports)

    def from_module_import_as(self, attribute_name, local_name=None):
        if not local_name:
            local_name = 'MODULE%s_dot_%s' % (self.name, attribute_name)
        self.imported_attributes.append((attribute_name, local_name))
######################################################################
class Enum(object):

    def __init__(self, name, keys, __namespace__ = '', __headers__ = []):
        self.map = {}
        self.__name__ = name
        self.__namespace__ = __namespace__
        self.fully_qualified_name = self.__namespace__ + '::' + name
        for number, key in enumerate(keys):
            self.map[key] = number

    #@staticmethod #(2.4 feature)
    def setup():
        print "  PyObject* EnumDict; PyObject* EnumInt;"
    setup = staticmethod(setup)

    def generate_code(self):
        print separator % self.fully_qualified_name
        print '  EnumDict = PyDict_New();'
        for key, value in self.map.items():
            print '  EnumInt = PyInt_FromLong(%s);' % value
            print '  PyDict_SetItemString(EnumDict, "%s", EnumInt);' % key
            print '  Py_DECREF(EnumInt);'
        print '  %s = PyObject_CallFunction(EnumMetaType, "sO", "%s", EnumDict);' % (Misc.type_identifier_name(self), self.__name__)
        print '  PyDict_SetItemString(module_dict, "%s", %s);' % (self.__name__, Misc.type_identifier_name(self))
        print '  Py_DECREF(EnumDict);'

    def insert(self, variable_name):
        return '  return PyObject_CallFunction(%s, "i", *%s);' % (Misc.type_identifier_name(self), variable_name)
######################################################################
class ModuleMetaclass(type):

    "This is the module documentation string"

    def __init__(self, name, bases, dict):
        del dict['__module__']
        del dict['__metaclass__']
        self.name = name
        self.classes = []
        # Ensure that bases are created before their subclasses
        #for new in (c for c in dict.values() if type(c) is ExtensionType): #(2.4 feature)
        for new in [c for c in dict.values() if type(c) is ExtensionType]: 
            before_new, after_new = [], []
            for n,old in enumerate(self.classes):
                if new in old.__mro__:
                    self.classes.insert(n, new)
                    break
            else:
                self.classes.append(new)
        #self.classes = self.__order__

        self.enums = [e for e in dict.values() if type(e) is Enum]

        self.headers = []
        for cls in self.classes:
            cls.module = self
            cls.register_headers_in(self)

        self.extra_headers = [h for h in dict.values() if type(h) is IncludeHeader]
        self.Ccode = [c for c in dict.values() if type(c) is IncludeImplementation]
        # Import other modules
        self.imports = [i for i in dict.values() if type(i) is Import]
        self.module_methods_struct_name = 'module_methods_For_%s' % self.name

        # Temporary hack
        self.include_dirs = []
        self.library_dirs = []
        self.libraries = []

    def generate(self, stream=sys.stdout):
        print "#define private public"
        print "#include <Python.h>"
        print "#include <string>"
        print "#include <cstdio>"
        print "#include <iostream> // for diagnostcs only: remove in production"
        print
        print "PyObject* global_typeinfo_map_temporary_hack;"
        print "PyObject* dynamicDownCast(PyObject*, PyObject* Pinstance);"
        print
        for imp in self.imports:
            print imp.declare_global_handles()
        print 
        print builtins.string.extractor
        print builtins.string.inserter
        print builtins.voidstar.inserter
        print "struct Dummy { virtual ~Dummy() {}; }; // Helps with dynamic type discovery"
        print
        for header in self.headers:
            print '#include "%s"' % header
        print
        for h in self.extra_headers:
            print '#include "%s"' % h.name
        for cls in self.classes:
            print "PyObject* %s;" % cls.class_object_name
        for enum in self.enums:
            print "PyObject* %s;" % Misc.type_identifier_name(enum)
        for cls in self.classes:
            cls.generate_code()
        for code in self.Ccode:
            print '#include "%s"' % code.name
        self.generate_module_methods_struct()
        print '\n'.join([("/%s/" % ('*'*70))]*3)
        print """
#ifndef PyMODINT_FUNC /* declarations for DLL import/export */
#define PyMODINT_FUNC void
#endif
"""
        self.generate_init_function()

    def generate_module_methods_struct(self):
        lines = [b.method_struct_line(name)
                 for name,b in self.__dict__.items() if type(b) is BuiltinFunction]
        lines = indent('\n'.join(lines))

        print """
static PyMethodDef %s[] = {
%s
  {NULL} /* Sentinel */
};
""" % (self.module_methods_struct_name, lines)

    def generate_init_function(self):
        self.set_up_init()
        self.import_modules()
        for cls in self.classes:
            self.build_class(cls)
        Enum.setup()
        for enum in self.enums:
            enum.generate_code()
        self.xxxx()
        print "}"

    def set_up_init(self):
        ti2classname = Misc.typeinfo_to_class_map_name()
        print """
PyMODINIT_FUNC
init%s(void) {

  PyObject* this_module = Py_InitModule3("%s",
                                          %s,
                                          "%s");
  PyObject* module_dict = PyModule_GetDict(this_module);
  PyObject* %s = PyDict_New();
  PyDict_SetItemString(module_dict, "%s", %s);
  global_typeinfo_map_temporary_hack = %s;
""" % (self.name, self.name,
       self.module_methods_struct_name, self.__doc__,
       ti2classname, ti2classname, ti2classname, ti2classname)

        print """
  PyObject* class_dict;
  PyObject* class_name;  
  PyObject* bases;
  PyObject* docstring;
"""

    def import_modules(self):
        for imp in self.imports:
            print indent(imp.import_module())

    def build_class(self, cls):
        print "  /******************* %s ****************/" % cls.fully_qualified_name

        self.create_temporaries(cls)
        self.fill_class(cls)
        self.decref_temporaries()
        self.register_misc_class_information(cls)

    def create_temporaries(self, cls):
        bases = [b.class_object_name for b in cls.__bases__ if not b is object]
        if not bases:
            bases = ('(PyObject*)&ProxyBaseObject_Type',)
        print "  bases = PyTuple_New(%s);" % len(bases)
        if bases:
            print indent('\n'.join(["PyTuple_SetItem(bases, %s, %s);" % (n,b)
                                    for n,b in itt.izip(itt.count(), bases)]))

        print """
  class_dict = PyDict_New();
  docstring = PyString_FromString("%s");
  PyDict_SetItemString(class_dict, "__doc__", docstring); 
  class_name = PyString_FromString("%s");
  %s =
  PyObject_CallFunctionObjArgs((PyObject*)&PyType_Type, 
                               class_name, bases, class_dict, NULL); 
  PyObject_SetAttrString(%s, "__typeinfo__", PyCObject_FromVoidPtr((void*)&typeid(%s), 0));
""" % (cls.__doc__, cls.__name__, cls.class_object_name, cls.class_object_name, cls.fully_qualified_name)
        
    def fill_class(self, cls):
        print """  //  Register class in module
  PyDict_SetItemString(module_dict, "%s", %s);
  //  Register methods in class
  for (PyMethodDef* method = %s; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject* pymethod = PyMethod_New(function, NULL, %s);
    PyObject_SetAttrString(%s, method->ml_name, pymethod);
    Py_DECREF(function);
    Py_DECREF(pymethod);
  }
""" % (cls.__name__, cls.class_object_name, MethodsStruct.name(cls), cls.class_object_name, cls.class_object_name),

        if cls.static_methods or cls.__has_vtable__:
            print """  // Register static methods in class
  for (PyMethodDef* method = %s; method->ml_name != NULL; method++) {
    PyObject* function = PyCFunction_New(method, NULL);
    PyObject_SetAttrString(%s, method->ml_name, function);
    Py_DECREF(function);
  }
""" % (StaticMethodsStruct.name(cls), cls.class_object_name),

    def decref_temporaries(self):
        print """
  Py_DECREF(bases);
  Py_DECREF(class_dict);
  Py_DECREF(class_name);
  Py_DECREF(docstring);
"""

    def register_misc_class_information(self, cls):
        print """  PyDict_SetItemString(%s, typeid(%s).name(), %s);
""" % (Misc.typeinfo_to_class_map_name(), cls.fully_qualified_name, cls.class_object_name,)

    def build(self, generated_source_name = None, clean = False, build = True):
        if not generated_source_name:
            generated_source_name = 'generated_reflex.cpp'
        try:
            sys.stdout = open(generated_source_name, 'w')
            self.generate()
        finally:
            sys.stdout = sys.__stdout__

        if build:
            from distutils.core import setup, Extension

            if 'install' not in sys.argv:
                sys.argv += ['build', '--build-base', '/tmp', 'install', '--install-lib', '.']

            the_module = Extension(self.name,
                                   [generated_source_name],
                                   include_dirs = self.include_dirs,
                                   library_dirs = self.library_dirs,
                                   libraries = self.libraries,
                                   )

            setup( name = self.name,
                   #version = '0.00001',
                   #description = 'test functionality of automatic wrapping helper',
                   #author = "Jacek Generowicz",
                   #author_email = "jacek.generowicz@cern.ch",
                   ext_modules = [the_module])


    def xxxx(self):
        print """
if (PyType_Ready(&FunkyTuple_Type) < 0) {std::cout << "FunkyTuple not Ready" << std::endl; }
//else {std::cout << "FunkyTuple seems to be OK." << std::endl;}
PyDict_SetItemString(module_dict, "Funkytuple", (PyObject*)&FunkyTuple_Type);
if (PyType_Ready(&ProxyBaseObject_Type) < 0) {std::cout << "ProxyBaseObject not Ready" << std::endl; }
//else {std::cout << "ProxyBaseObject seems to be OK." << std::endl;}
PyDict_SetItemString(module_dict, "proxyobject", (PyObject*)&ProxyBaseObject_Type);
"""
