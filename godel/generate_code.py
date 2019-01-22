import sys, os
import build_structure
import parse_headers
from parse_headers import GCCXML as XX


def do_everything(modulename, headername, libfilename=None):
    xmlfilename = headername.split('.')[0] + 'xml'
    os.system('gccxml %s -fxml=%s' % (headername, xmlfilename))
    parse_headers.parse(xmlfilename)
    headerfile = parse_headers.file_name_to_object[headername]
    classes = [c for c in parse_headers.all_items.itervalues()
               if type(c) == parse_headers.GCCXML.Class
               and c.file == headerfile]
    generate_module(classes, modulename)

    if libfilename:
        compile_module(modulename, libraries=[libfilename])
    else: 
        compile_module(modulename)




def generate_module(objects, modulename, filename = None):
    # If filename is not specified, create a name based on the module name
    if not filename:
        filename = module_filename(modulename)
    stream = open(filename, 'w')

    print >> stream, "#include <Python.h>"
    print >> stream, '#include "godelstuff.h"'
    print >> stream
    classes = select_and_sort_classes(objects)
    print >> stream, include_headers(classes)
    for cls in classes:
        print >> stream, declare_class(cls)
    for cls in classes:
        print >> stream, generate_class(cls)

    print >> stream, """// Module methods
static PyMethodDef module_methods_struct_automate_the_name[] = {
  {NULL} /* Sentinel */
};
"""

    ############################################################
    # initialization function
    ############################################################
    print >> stream, "PyMODINIT_FUNC \ninit%s(void) {" % modulename

    print >> stream, indent("""
PyObject* this_module = Py_InitModule3("%s",
                                       module_methods_struct_automate_the_name,
                                       "Module docstring not done yet");
""" % (modulename,))
    print >> stream, indent("PyObject* module_dict = PyModule_GetDict(this_module);")

    print >> stream, indent(import_module('godelstuff'))
    print >> stream, indent(from_module_import('godelstuff', 'NULLPointer'))
    print >> stream, indent(from_module_import('godelstuff', 'global_typeinfo_map_temporary_hack'))
    print >> stream

    for cls in classes:
        print >> stream, indent(initialize_class(cls))


    print >> stream, "  // Py_DECREF(module_dict) // ??"
    print >> stream, "}"
        
def select_and_sort_classes(objects):
    classes = [o for o in objects if type(o) == XX.Class]
    for o in classes:
        build_structure.build_generic_functions(o)
    # Need to sort classes, so that bases are initialized before their
    # subclasses (not done yet)
    return classes

def include_headers(objects):
    headers = set()
    for o in objects:
        headers.add(o.file.name)
    includes = ['#include "%s"' % h for h in headers]
    return '\n'.join(includes)

def declare_class(cls):
    return "PyObject* %s;" % identifier(cls)

def generate_class(cls):
    stuff_to_do = [separator % fully_qualified_name(cls)]
    stuff_to_do.append(generate_class_methods(cls))
    stuff_to_do.append(generate_class_methods_struct(cls))
    stuff_to_do.append(generate_class_methods_struct(cls, static=True))
    return '\n'.join(stuff_to_do)

def generate_class_methods(cls):
    methods = [generic_method(cls.constructor)]
    for m in cls.dynamicmethods:
        methods.append(generic_method(m))
    for m in cls.staticmethods:
        methods.append(generic_method(m))
    return '\n'.join(methods)

def generate_class_methods_struct(cls, static=False):
    if static:
        methods = cls.staticmethods
        methodstyle = 'METH_STATIC'
    else:
        methods = [cls.constructor] + cls.dynamicmethods
        methodstyle = 'METH_VARARGS'
    struct_name = class_methods_struct_name(cls, static)
        
    lines = ['{"%s", %s, %s, ""},' % (method_python_name(m), identifier(m), methodstyle)
             for m in methods]

    method_pointers = '\n'.join(lines)
    return """
static PyMethodDef %s[] = {
%s
    { NULL } /* Sentinel */
};
""" % (struct_name, indent(method_pointers))

def method_python_name(meth):
    # Will have to take care of operator names etc. eventually
    operator_map = {'operator==': '__eq__',
                    }
    if meth.name in operator_map:
        return operator_map[meth.name]
    return meth.name

def class_methods_struct_name(cls, static=False):
    if static:
        static = 'Static'
    else:
        static = ''
    return "%sMethodsStructFor_%s" % (static, identifier(cls))

def initialize_class(cls):
    return """
godel_initialize_class(module_dict,
                       "%s",
                       %s,
                       typeid(%s),
                       %s,
                       %s);
""" % (cls.name, identifier(cls), fully_qualified_name(cls),
       class_methods_struct_name(cls), class_methods_struct_name(cls,True))
    return '// Initialize class %s: not implemented yet' % cls.name

def identifier(obj):
    if isinstance(obj, build_structure.GenericMethod):
        return "%s__%s" % (fully_qualified_name(obj.context).replace(':','_'),
                           method_python_name(obj) )
        print obj.context, obj.name
    return fully_qualified_name(obj).replace(':','_')

def fully_qualified_name(obj):
    if type(obj) == XX.FundamentalType:
        return obj.name
    if hasattr(obj, 'context'):
        prefix = fully_qualified_name(obj.context)
    else: prefix = ''
    return '%s::%s' % (prefix, obj.name)

def strip_cvrp(TYPE, pointer_stripped=False):
    if type(TYPE) in (XX.CvQualifiedType, XX.ReferenceType):
        return strip_cvrp(TYPE.type)
    if type(TYPE) == XX.PointerType:
        if pointer_stripped:
            return TYPE
        else:
            return strip_cvrp(TYPE.type, True)
    return TYPE


def generic_method(meth):
    methods = "".join([single_method(m) for m in meth.methods])
    if type(meth) == build_structure.GenericStaticMethod:
        separate_self = ""
    else:
        separate_self = indent("""PyObject* self = PyTuple_GetItem(args, 0);
args = PyTuple_GetSlice(args, 1, 999999);
""")


    return """
PyObject*
%s(PyObject*, PyObject* args) {
%s%s
    PyObject_Print(args, stdout, 0);
    PyErr_SetString(PyExc_TypeError, "No method with matching arguments found for %s");
    return 0;
}
""" % (identifier(meth), separate_self, indent(methods), meth.name)


def single_method(meth):
    invoke = indent(invoke_method_and_return(meth), 2)
    def method_without_arguments(meth):
        return """
{
    if (PyTuple_Size(args) == 0) {
%s
    }
}""" % (invoke,)
    
    def method_with_arguments(meth):
        declarations = indent(method_local_declarations(meth))
        format_string = method_format_string(meth)
        format_args =  method_format_args(meth)
        return """
{
%s
    if (PyArg_ParseTuple(args, "%s", %s)) {
%s
    } else {
        PyErr_Clear();
    }
}""" % (declarations, format_string, format_args, invoke)

    if method_parameters(meth):
        return method_with_arguments(meth)
    else:
        return method_without_arguments(meth)

def invoke_method_and_return(meth):
    if type(meth) == XX.Constructor:
        return "%s;\n%s" % (method_call(meth), insert_and_return(None))

    invoke = implement_return_semantics(meth.returns, method_call(meth))
    insert = insert_and_return(meth.returns)
    if type(meth.returns) == XX.FundamentalType and meth.returns.name == 'void':
        return "%s;\n%s" % (invoke, insert)
    else:
        store_retval = "%s = " % method_local_declaration(meth.returns)
        return "%s%s;\n%s" % (store_retval, invoke, insert)

def method_call_args(meth):
    def implement_argument_passing_semantics(TYPE, n):
        if type(TYPE) == XX.PointerType or type(strip_cvrp(TYPE)) == XX.FundamentalType:
            # Pointers to fundamental types are a problem: pointing to temporary !
            # (Not implemented)
            deref = ''
        else:
            deref = '*'
        return deref + variable_name(strip_cvrp(TYPE), n)
        
    return ', '.join([implement_argument_passing_semantics(p,n)
                      for n,p in enumerate(method_parameters(meth))])

def insert_and_return(TYPE):
    if TYPE == None or type(TYPE) == XX.FundamentalType and TYPE.name == 'void':
        return "Py_INCREF(Py_None);\nreturn Py_None;"
    strippedTYPE = strip_cvrp(TYPE)
    if type(strippedTYPE) == XX.FundamentalType:
        return "return %s(%s);" % (find_inserter(strippedTYPE), variable_name(strippedTYPE))

    classpointer = identifier(strippedTYPE)
    downcast = 'false'#'true'
    return "return %s(%s, %s, %s);" % (find_inserter(strippedTYPE), classpointer,
                                       variable_name(strippedTYPE), downcast)

def find_inserter(TYPE):
    if type(strip_cvrp(TYPE)) == XX.Class:
        return "CommonInserter"
    try:
        typename = strip_cvrp(TYPE).name
        return {'int': 'PyInt_FromLong',
                'double': 'PyFloat_FromDouble',
                'float': 'PyFloat_FromDouble',
                'bool': 'PyInt_FromLong',
                }[typename]
    except KeyError:
        raise "Can't find inserter for %s." % TYPE

def method_call(meth):
    if type(meth) == XX.Constructor:
        return "((%s*)((ProxyObject*)self)->__the_instance__) = new %s(%s)" % \
               (fully_qualified_name(meth.context),
                fully_qualified_name(meth.context), method_call_args(meth))
    if meth.isStatic:
        return "%s(%s)" % (fully_qualified_name(meth), method_call_args(meth))
    else:
        return "((%s*)((ProxyObject*)self)->__the_instance__)->%s(%s)" % \
               (fully_qualified_name(meth.context), meth.name, method_call_args(meth))

def implement_return_semantics(return_type, method_call):
    if not return_type:
        # We have a constructor
            return method_call
    if type(strip_cvrp(return_type)) == XX.FundamentalType:
        if type(return_type) == XX.PointerType:
            return "*(%s)" % method_call
        else:
            return method_call
    if type(strip_cvrp(return_type)) == XX.Class:
        if type(return_type) == XX.ReferenceType:
            return "&(%s)" % method_call
        if type(return_type) == XX.PointerType:
            return method_call
        # by value: copy construct to avoid taking address of temporary
        return "new %s::%s(%s)" % (fully_qualified_name(return_type), return_type.name, method_call)
    raise "Haven't implemented return semantics for %s (%s) yet." % (type(strip_cvrp(return_type)), return_type)

    

def variable_name(TYPE, suffix=None):
    if suffix == None:
        suffix = 'result'
    return "%s_%s" % (TYPE.name, suffix)

def method_parameters(meth):
    return [a.type for a in meth.arguments]

def method_local_declarations(meth):
    decls = ['%s;' % method_local_declaration(p,n)
             for n, p in enumerate(method_parameters(meth))]
    return '\n'.join(decls)
        
def method_local_declaration(TYPE, suffix=None):
    if suffix == None and type(TYPE) in (XX.ReferenceType,) and type(TYPE.type) == XX.CvQualifiedType and TYPE.type.const:
        const = "const "
    else:
        const = ""

    TYPE = strip_cvrp(TYPE)
    fqn = fully_qualified_name(TYPE)
    if type(TYPE) == XX.FundamentalType:
        deref = ''
    else:
        deref = '*'
    # std:string ignored for now !
    return "%s%s%s %s" % (const, fqn, deref, variable_name(TYPE, suffix))

def method_format_string(meth):

    def format_code(TYPE):
        if type(TYPE) == XX.Class:
            return "O&"
        return {'int': 'i',
                'double': 'd'}[TYPE.name]

    return ''.join([format_code(strip_cvrp(p)) for p in method_parameters(meth)])
    

def method_format_args(meth):

    def method_format_arg(TYPE, suffix):
        if type(TYPE) == XX.Class:
            return "CommonExtractor, &%s" % variable_name(TYPE, suffix)
        return "&%s" % variable_name(TYPE, suffix)

    return ", ".join([method_format_arg(strip_cvrp(p),n)
                      for n,p in enumerate(method_parameters(meth))])

    





def compile_module(modulename, filename = None,
                   include_dirs = [], library_dirs = ['.'], libraries = [],
                   clean = False):
    if not filename:
        filename = module_filename(modulename)

    from distutils.core import setup, Extension
    the_module = Extension(modulename,
                           [filename],
                           include_dirs = include_dirs,
                           library_dirs = library_dirs,
                           libraries = libraries,
                           extra_compile_args = ['-ansi -pedantic -g']
                           )

    originalsysargv = sys.argv[:]
    try:
        sys.argv += ['install', '--install-lib', '.']
        setup( name = modulename,
        #version = '0.00001',
        #description = 'test functionality of automatic wrapping helper',
        #author = "Jacek Generowicz",
        #author_email = "jacek.generowicz@cern.ch",
        ext_modules = [the_module])
    finally:
        sys.argv = originalsysargv
    
def module_filename(modulename):
    return 'tmp_godel_%s.cpp' % modulename




def import_module(modulename):
    return """PyObject* MODULE%s = PyImport_ImportModule("%s");
if (!MODULE%s) {
    return;
}
""" % (modulename, modulename, modulename)

def from_module_import(modulename, objectname, localname=None):
    if not localname:
        localname = objectname
    return """PyDict_SetItemString(module_dict, "%s", MODULE%s);
%s = PyObject_GetAttrString(MODULE%s, "%s");
""" % (modulename, modulename, localname, modulename, objectname)




def indent(code, N=1):
    addition = ('\n'+(N*indent.size)*' ')
    split = code.split('\n')
    return addition[1:] + addition.join(split)
indent.size = 4

separator = '/%s/\n' % ('*' * 70)
separator = '%s/*        %%-60s*/\n%s' % (separator, separator)



if __name__ == '__main__':

    try: 
        modulename  = sys.argv[1]; del sys.argv[1]
        headername  = sys.argv[1]; del sys.argv[1]
        if len(sys.argv) > 1 and sys.argv[1][:2] == '-l':
            libfilename = sys.argv[1][2:]; del sys.argv[1]
        else:
            libfilename = None
        do_everything(modulename, headername, libfilename)
    except IndexError:
        print "Usage:  %s <module name> <header file>" % sys.argv[0]
