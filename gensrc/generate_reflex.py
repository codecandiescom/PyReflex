# $Id: generate_reflex.py,v 1.18 2005/03/03 10:16:26 jacek Exp $

#(2.4 feature)
#from godel import (Constructor, Method, StaticMethod, PythonFunction, FunnyPythonFunction,
#                   ModuleMetaclass, Import, BuiltinFunction, IncludeHeader,
#                   ExtensionType as __metaclass__)

from godel import Constructor, Method, StaticMethod, PythonFunction, FunnyPythonFunction, \
                   ModuleMetaclass, Import, BuiltinFunction, IncludeHeader, \
                   ExtensionType as __metaclass__


import godel
import sys

class reflex:
    __metaclass__ = ModuleMetaclass

    reflex_framework = Import('Paux')
    reflex_framework.from_module_import_as('NULLPointer', 'NULLPointer')
    reflex_framework.from_module_import_as('Reflex_Type_to_Python_proxy_map', 'Reflex_Type_to_Python_proxy_map')
    reflex_framework.from_module_import_as('auxP', 'auxP')
    reflex_framework.from_module_import_as('auxC', 'auxC')
    reflex_framework.from_module_import_as('extractor_map', 'extractor_map')
    reflex_framework.from_module_import_as('Enum', 'EnumMetaType')
    
    cobjectid = BuiltinFunction('cobjectid', "O", "PyCObject_AsVoidPtr ... as Python int")
    funkize = BuiltinFunction('funkize', 'O', "funkizes a tuple")

    take_ownership = BuiltinFunction('take_ownership', "O", "Make __del__ delete the C++ object held by the proxy.")
    drop_ownership = BuiltinFunction('drop_ownership', "O", "Make __del__ NOT delete the C++ object held by the proxy.")
    does_proxy_own = BuiltinFunction('does_proxy_own', "O", "Shows whether __del__ will delete the C++ object held by the proxy.")

    proxyobject_newOBJECT = BuiltinFunction('proxyobject_newOBJECT', 'VARARGS', "")

    Type__hash__ = BuiltinFunction('reflex_Type_hash', "O", "")
    Type__eq__ = BuiltinFunction('reflex_Type_eq', "VARARGS", "")

    _ = IncludeHeader('Caux.h')
    if sys.platform == 'darwin':
        _dl = IncludeHeader('dlfcn.h')
        loadDictApple = BuiltinFunction('loadDictApple', "O", "wrapped dlopen for tmp OS X hack")
    ########################################################################
    TYPE = godel.Enum("TYPE",
                      ("CLASS", "STRUCT", "ENUM", "FUNCTION", "ARRAY", "FUNDAMENTAL",
                       "POINTER", "POINTERTOMEMBER", "TYPEDEF", "UNION",
                       "INSTANTIATEDTEMPLATECLASS",
                       "INSTANTIATEDTEMPLATEFUNCTION", "NAMESPACE",
                       "DATAMEMBER", "FUNCTIONMEMBER", "UNRESOLVED"),
                      __namespace__ = 'seal::reflex',
                      __headers__ = ['Reflex/Kernel.h'])
    ########################################################################
    class type_info:

        "C++'s std::type_info"

        __namespace__ = 'std'
        __headers__ = ['typeinfo']
        __has_vtable__ = False

        name = Method('string')
    ########################################################################
    class Type:

        "reflex::Type"

        __namespace__ = 'seal::reflex'
        __headers__ = ['Reflex/Type.h']
        __has_vtable__ = False


        byName = StaticMethod('Type string')
        type = StaticMethod('Type int')
        typeCount = StaticMethod('int')

        base = Method('Base int')
        baseCount = Method('int')
        
        name = Method('string int')

        typeTypeAsString = Method('string')

        id = Method('voidstar')
        typeType = Method('TYPE')

        typeInfo = Method('type_info&')

        isClass = Method('bool')

        member = Method('Member int')
        memberCount = Method('int')

        toType = Method('Type')
        sizeOf = Method('int')

        isReference = Method('bool')
        isPointer = Method('bool')
        isTypedef = Method('bool')
        isClass = Method('bool')

        parameter = Method('Type int')
        parameterCount = Method('int')
        returnType = Method('Type')

        templateArgument = Method('Type int')
        templateArgumentCount = Method('int')
    ########################################################################
    class Base:

        __namespace__ = 'seal::reflex'
        __headers__ = ['Reflex/Base.h']
        __has_vtable__ = False

        toType = Method('Type')
        offset = Method('int')
    ########################################################################
    class Scope:

        __namespace__ = 'seal::reflex'
        __headers__ = ['Reflex/Scope.h']
        __has_vtable__ = False

        byName = StaticMethod('Scope string')
        id = Method('voidstar')
        scopeType = Method('TYPE')

        member = Method('Member int')
        memberCount = Method('int')

        functionMember = Method('Member int')
        functionMemberCount = Method('int')

        dataMember = Method('Member int')
        dataMemberCount = Method('int')

        subScope = Method('Scope int')
        subScopeCount = Method('int')

        subType = Method('Type int')
        subTypeCount = Method('int')

        memberTemplate = Method('MemberTemplate int')
        memberTemplateCount = Method('int')

        typeTemplate = Method('TypeTemplate int')
        typeTemplateCount = Method('int')

        name = Method('string int')

        isClass = Method('bool')
        isNamespace = Method('bool')
        isEnum = Method('bool')
        isUnion = Method('bool')
    ########################################################################
    class Member:

        __namespace__ = 'seal::reflex'
        __headers__ = ['Reflex/Member.h']
        __has_vtable__ = False

        name = Method('string')
        type = Method('Type')
        isConstructor = Method('bool')
        isDestructor = Method('bool')

        parameterCount = Method('int bool')

        isTemplateInstance = Method('bool')
        templateArgument = Method('Type int')
        templateArgumentCount = Method('int')
    ########################################################################    
    class MemberTemplate:

        __namespace__ = 'seal::reflex'
        __headers__ = ['Reflex/MemberTemplate.h']

        name = Method('string int')

        instantiation = Method('Member int')
        instantiationCount = Method('int')
    ########################################################################    
    class TypeTemplate:

        __namespace__ = 'seal::reflex'
        __headers__ = ['Reflex/TypeTemplate.h']

        name = Method('string int')

        instantiation = Method('Type int')
        instantiationCount = Method('int')
    ########################################################################

    ########################################################################    
    class GenericFunction:

        __namespace__ = ''
        __headers__ = []#['Caux.h']

        __init__ = Constructor('GenericFunction* string')
        add_method = Method('void Member')
        __call__ = PythonFunction()
        __get__ = FunnyPythonFunction()
    ########################################################################    
    class DEL:

        __namespace__ = ''
        __headers__ = ['ProxyObject.h']#['Caux.h']

        __init__ = Constructor('DEL* Member')
        __call__ = PythonFunction()
        __get__ = FunnyPythonFunction()
    ########################################################################    
    class Field:

        __namespace__ = ''
        __headers__ = []

        __init__ = Constructor('Field* Member int')
        __get__ = FunnyPythonFunction()
        __set__ = FunnyPythonFunction()
    ########################################################################    

# Hack to get around problem of evaling the type names
# Need to replace that whole mechanism. FIXME
import godel
godel.__dict__.update(globals())



build = True
if 'nobuild' in sys.argv:
    sys.argv.remove('nobuild')
    build = False

if 'where' in sys.argv:
    where = sys.argv[sys.argv.index('where')+1]
    sys.argv.remove('where')
else:
    where = None

if build:
    # Temporary solution for adding extra information to the module.
    import os, commands
    SEAL = commands.getoutput('scram b echo_LOCALTOP').split()[-1]
    ARCH = os.getenv('SCRAM_ARCH')

    reflex.include_dirs = [SEAL+'/src/Dictionary/Reflex']
    reflex.library_dirs = [SEAL+ARCH+'/lib/']


reflex.build(build=build, generated_source_name=where)
