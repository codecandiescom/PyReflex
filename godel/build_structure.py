from parse_headers import GCCXML as XX

def build_generic_functions(scope):
    __init__, dynamicmethods, staticmethods, functions = group_methods(scope)
    if type(scope) == XX.Class:
        scope.constructor = __init__
        scope.dynamicmethods = dynamicmethods
        scope.staticmethods = staticmethods
    elif type(scope) == XX.Namespace:
        scope.functions = functions
    else:
        raise "Unexpected scope type"

def group_methods(scope):
    generics = collecting_dict()
    for m in scope.members:
        if type(m) in (XX.Constructor, XX.Method, XX.Function, XX.OperatorMethod):
            generics.add(m.name, m)
            
    __init__ = {}
    dynamicmethods = {}
    staticmethods = {}
    functions = {}

    for name, meths in generics.iteritems():
        tpe = type(meths[0])
        if tpe == XX.Constructor:
            __init__[name] = meths
        elif tpe == XX.Function:
            functions[name] = meths
        elif tpe == XX.Method or tpe == XX.OperatorMethod:
            if meths[0].isStatic:
                staticmethods[name] = meths
            else:
                dynamicmethods[name] = meths
        else:
            raise "Unexpected method type %s for method %s in scope %s" % (tpe, name, scope)

    
    return (GenericConstructor(*(__init__.items()[0]+(scope,))),
            [GenericMethod(n,ms,scope) for n,ms in dynamicmethods.items()],
            [GenericStaticMethod(n,ms,scope) for n,ms in staticmethods.items()],
            [GenericFunction(n,ms,scope) for n,ms in functions.items()])



class collecting_dict(dict):

    def add(self, key, value):
        if key in self:
            self[key].append(value)
        else:
            self[key] = [value]
    

class GenericMethod(object):
    def __init__(self, name, methods, context):
        self.name = name
        self.methods = methods
        self.context = context

class GenericStaticMethod(GenericMethod):
    pass

class GenericConstructor(GenericMethod):
    pass

class GenericFunction(GenericMethod):
    pass
