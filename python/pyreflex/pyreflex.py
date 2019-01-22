import reflex
import adapt_reflex
from Paux import memoize

def loadDictionary(name):
    try:
        __import__(name)
    except ImportError, e:
        if "init%s" % (name) not in str(e):
            print e
            try:
                reflex.loadDictApple(name)
            except Exception, f:
                print f
                raise e

    return BuildScope()

# hack around lack of Scope -> Type conversion in Reflex
def asType(self):
    #if not self.isClass():
    #    raise "This scope (%s) is not a class, so can't convert it to a Type." % self.name()
    name = self.name(4)
    Type = reflex.Type.byName(name)
    if Type.typeType() != reflex.TYPE.UNRESOLVED:
        return Type
    raise "This scope (%s) is not a class, so can't convert it to a Type." % self.name()

reflex.Scope.asType = asType
del asType

# hack around lack of Type -> Scope conversion in Reflex
def asScope(self):
    if not self.isClass():
        raise "Cannot convert Type to Scope: %s is not a class, it's %s." % (self.name(4), self.typeTypeAsString())
    name = self.name(4)
    return reflex.Scope.byName(name)

reflex.Type.asScope = asScope
del asScope

def __hash__(self):
    return reflex.cobjectid(self.id())
def __eq__(self, other):
    return reflex.cobjectid(self.id()) == reflex.cobjectid(other.id())
reflex.Scope.__hash__ = __hash__
reflex.Scope.__eq__ = __eq__
del __hash__, __eq__

def scopedict(Rscope):
    scopedict = {}
    # Templates
    for t in Rscope.typeTemplates():
        scopedict[t.name(0)] = ClassTemplate(t)
    for t in Rscope.memberTemplates():
        scopedict[t.name(0)] = FunctionTemplate(t)
    # Inner scopes
    for innerscope in Rscope.subScopes():
        if not innerscope.isEnum() and not innerscope.isUnion(): # Union untested: not implemented in Reflex yet
            scopedict[innerscope.name(0)] = BuildScope(innerscope)
    # Methods
    GenericMethods, destructor = collect_methods(Rscope)
    for name, methods in GenericMethods.items():
        scopedict[name] = make_generic_function(name, methods)
    if destructor:
        scopedict['__del__'] = destructor
    # Data
    for datum in Rscope.dataMembers():
        #scopedict[datum.name()] = "a datum of type %s" % datum.type().fullName(False)
        scopedict[datum.name()] = reflex.Field(datum, 0)
    # Typedefs
    for td in Rscope.subTypes():
        if td.typeType() == reflex.TYPE.TYPEDEF:
            if td.toType().typeType() in (reflex.TYPE.CLASS, reflex.TYPE.STRUCT):
                scopedict[td.name(2)] = BuildClass(td.toType().asScope())
            else:
                scopedict[td.name(2)] = NotImplementedError("typedef %s %s" % (td.toType().name(2), td.name(2)))
    return scopedict
    
def BuildScope(Rscope=None):
    if Rscope is None:
        Rscope = reflex.Scope.byName('')
    if not isinstance(Rscope, reflex.Scope):
        raise "BuildScope expects a reflex.Scope, but got %s." % Rscope
    # This scope
    if Rscope.scopeType() == reflex.TYPE.NAMESPACE:
        return Namespace(Rscope)
    return BuildClass(Rscope)

class PyreflexClass(type):
    # Check the performance implications of using this as the
    # metaclass, BEFORE building too much functionality into it.
    def __repr__(self):
        return "<Pyreflex class '%s'>" % self.__name__

#@memoize #(2.4 feature)
def BuildClass(Rscope):
    classdict = scopedict(Rscope)
    name = Rscope.name(4)
    Rclass = Rscope.asType()
    bases = []
    for b in Rclass.bases():
        try:
            b = b.toType().asScope()
            bases.append(BuildClass(b))
        except: # Catching string raised in asScope()
            # Throwing away unresolved classes from the list of bases, for now.
            pass

    bases = tuple(bases +[reflex.proxyobject])
    the_class = PyreflexClass(name, bases, classdict)
    
    __new__ = reflex.proxyobject_newOBJECT
    the_class.__new__ = staticmethod(__new__)
    
    reflex.Paux.Reflex_Type_to_Python_proxy_map[Rclass.ID()] = the_class
    Rclass.__Pyreflex_Type__ = the_class # Alternative to the above. Remove the former.
    auxP[Rscope.name(0)] = Rclass.ID()
    the_class.__Reflex_Type__ = Rclass
    the_class.__typeinfo__ = Rclass.typeInfo()
    auxC[the_class.__typeinfo__.name()] = the_class

    # Making the class iterable
    for len, get in (('size', 'operator[]'),
                     ('size', 'at'),
                     ('Size', 'At')):
        try:
            make_iterable(the_class, len, get)
            break
        except AttributeError:
            pass
    return the_class
BuildClass = memoize(BuildClass)


def make_iterable(the_class, len_name, getitem_name):

    LEN, GETITEM = getattr(the_class, len_name), getattr(the_class, getitem_name)

    def __getitem__(self, arg):
        if type(arg) == slice:
            
            # This is not possible to do correctly, in general. The
            # slice should be of the same type as the original object
            # but, in general, we cannot guess how to fill this object
            # with the relevant data. So we do the next best thing,
            # which is to return an equivalent Python
            # list. Alternatively we could assume that push_back is
            # always available ... at the cost of slices not working
            # at all for other sequences.
            size = LEN(self)
            indices = range(size)[arg]
            return [GETITEM(self,i) for i in indices]
            # The above is horribly inefficient. Try to find some
            # generator based solution.

        length = len(self)
        if arg < 0:
            arg += length
        if 0 <= arg < length:
            return GETITEM(self, arg)
        raise IndexError, "%s index out of range" % the_class.__name__

    the_class.__len__ = LEN
    the_class.__getitem__ = __getitem__


    # The following is probably more efficient than relying on the
    # sequence protocol implemented above. However, we might wish to
    # map this to the C++ iterator protocol (self.begin()++, !=
    # self.end()) directly.

    #def __iter__(self):
    #    count = 0
    #    while count < LEN(self):
    #        yield GETITEM(self, count)
    #        count += 1
    #
    #the_class.__iter__ = __iter__


def _TIP(cls):
    BEGIN, END = getattr(cls, 'begin'), getattr(cls, 'end')
    def __iter__(self):
        iterator = BEGIN(self)
        DEREF = getattr(cls, 'operator*')
        while iterator != END(self):
            yield DEREF(iterator)

    cls.__iter__ = __iter__

import types
class Namespace(types.ModuleType):

    #@memoize # 2.4 feature
    def __new__(cls, Rscope):
        return types.ModuleType.__new__(cls)
    __new__ = memoize(__new__)

    def __init__(self, Rscope):
        attributes = scopedict(Rscope)
        self.__name__ = Rscope.name(0)
        for name, attribute in attributes.items():
            if hasattr(self, name):
                # Do not clobber existing attributes on subsequent builds
                # NB, this is wrong in the case of new methods being
                # added to a generic function ... but we'll address
                # this point later.
                continue 
            setattr(self, name, attribute)

    def __repr__(self):
        return "<Namespace '%s'>" % self.__name__


class Template(object):

    def __init__(self, reflex_template):
        self._reflex_template = reflex_template
        self._instances = {}
# In Pylcgdict, template arguments were specified with
# parentheses. Now that function templates are available, this seems
# to be a bad choice, as parentheses following a template object could
# mean one of 2 things:

# 1) specification of template arguments (<...> in C++),

# 2) passing of function arguments.

# Therefore it is probably a better idea to use brackets [], rather
# than parentheses (), to specify template arguments. We keep the
# tests for both versions until we are sure which we prefer.

    # Parenthesis syntax version start  (((((
    #def __call__(self, *ptypes):
    # Parenthesis syntax version end    )))))
    # Bracked syntax version start      [[[[[

    # But, it turns out that GCCXML does not give us enough
    # information to deduce the template arguments from the function
    # arguments, so, either we remove __call__, or we go back to the
    # parenthesis-based syntax for template argument specification (in
    # which case we remove __getitem__ ... by moving its
    # implementation to __call__)
    
    #def __call__(self, *args):
    #    ptypes = tuple(map(type, args))
    #
    #    print "****************************************"
    #    print ptypes, args
    #    print "****************************************"
    #    return self[ptypes](*args)
    #    #return getattr(self, '__getitem__')(*ptypes)(*args)
        
    def __getitem__(self, ptypes):
        if type(ptypes) is not tuple:
            ptypes = (ptypes,)
    # Bracked syntax version end        ]]]]]
    # The rest is common to both
        try:
            return self._instances[ptypes]
        except KeyError:
            rtypes = map(find_reflex_type_corresponding_to_python_type, ptypes)
            for instance in self._reflex_template.instantiations():
                parmtypes = list(instance.templateArguments())
                if rtypes == parmtypes:
                    template_instance = self._reflex_to_python_instance(instance)
                    self._instances[ptypes] = template_instance
                    return template_instance

        members = [member for member in self._reflex_template.instantiations()]
        candidates = ['<%s>' % ', '.join([ta.name(4) for ta in member.templateArguments()]) for member in members]
        message = """
Cannot find template instance %s%s.

Candidates are
%s""" % (self._reflex_template.name(4), ptypes,
         '\n'.join(candidates))
        raise PyreflexException, message

    def __repr__(self):
        return "<%s template '%s'>" % (self.template_type_name, self._reflex_template.name(4))

    def dir(self):
        return [t.name(0) for t in self._reflex_template.instantiations()]


class FunctionTemplate(Template):

    template_type_name = 'Function'

    def __init__(self, reflex_template):
        super(FunctionTemplate, self).__init__(reflex_template)

        self.members_type_to_generic_function_map = {}
        # Collect methods
        GenericMethods = {}
        for method in reflex_template.instantiations():
            template_arguments = tuple(method.templateArguments())
            if template_arguments in GenericMethods:
                GenericMethods[template_arguments].append(method)
            else:
                GenericMethods[template_arguments] = [method]
        # ---------------
        for template_arguments, methods in GenericMethods.items():
            name = "%s<%s>" % (reflex_template.name(4),
                               ','.join([ta.name(4) for ta in template_arguments]))
            GF = make_generic_function(name, methods)
            for method in methods:
                self.members_type_to_generic_function_map[method.type().ID()] = GF

    def __getitem__(self, *args):
        template_instance = super(FunctionTemplate, self).__getitem__(*args)
        if hasattr(self, '_it_self'):
            return template_instance.__get__(self._it_self, self._it_class)
        else:
            return template_instance

    def __get__(self, obj, tpe):
        self._it_self = obj
        self._it_class = type
        return self

    def _reflex_to_python_instance(self, instance):
        return self.members_type_to_generic_function_map[instance.type().ID()]
        
class ClassTemplate(Template):

    template_type_name = 'Class'

    def _reflex_to_python_instance(self, instance):
        return reflex.Paux.Reflex_Type_to_Python_proxy_map[instance.ID()]


def make_generic_function(name, methods):
    generic = reflex.GenericFunction(name)
    for m in methods:
        generic.add_method(m)
    return generic

def collect_methods(Rscope):
    GenericMethods = {}
    destructor = None
    for method in Rscope.functionMembers():
        if method.isConstructor():
            name = '__init__'
        else:
            name = method.name()
        if method.isDestructor():
            # Destructor cannot be overloaded so don't make it a generic function
            destructor = reflex.DEL(method)
            continue 
        if name in GenericMethods:
            GenericMethods[name].append(method)
        else:
            GenericMethods[name] = [method]
    # Add operator mapping
    for name, function in GenericMethods.items():
        if name in _operator_map:
            GenericMethods[_operator_map[name]] = function
            # del GenericMethods[name] # if we wish not to keep the C++ name
    #if '[]' in GenericMethods:
    #    def __getitem__(self, arg):
    #        if type(arg) == slice:
    #            raise NotImplementedError
    #        return getattr(self, '[]')(arg)
    return GenericMethods, destructor

_operator_map = { "operator+"   : "__add__",     "operator-"   : "__sub__",
                  "operator*"   : "__mul__",     "operator/"   : "__div__",
                  "operator%"   : "__mod__",     "operator<<"  : "__lshift__",
                  "operator>>"  : "__rshift__",  "operator&"   : "__and__",
                  "operator|"   : "__or__",      "operator^"   : "__xor__",
                  "operator+="  : "__iadd__",    "operator-="  : "__isub__",
                  "operator*="  : "__imul__",    "operator/="  : "__idiv__",
                  "operator%="  : "__imod__",    "operator<<=" : "__ilshift__",
                  "operator>>=" : "__irshift__", "operator&="  : "__iand__",
                  "operator|="  : "__ior__",     "operator^="  : "__ixor__",
                  "operator=="  : "__eq__",      "operator!="  : "__ne__",
                  "operator>"   : "__gt__",      "operator<"   : "__lt__",
                  "operator>="  : "__ge__",      "operator<="  : "__le__",
                 #"operator**"  : "__pow__",     "operator**=" : "__ipow__",
                  "operator()"  : '__call__',
                  }


auxP = reflex.Paux.auxP
auxC = reflex.Paux.auxC


def __hash__(self):
    return reflex.Type__hash__(self)

def __eq__(self, other):
    return reflex.Type__eq__(self, other)

reflex.Type.__hash__ = __hash__
reflex.Type.__eq__ = __eq__
reflex.Type.ID = lambda self: reflex.cobjectid(self.id())


class PyreflexException(Exception):
    pass

########
class double(float):
    pass
try:
    __builtins__['double'] = double
except TypeError:
    __builtins__.double = double


########################################################################
python_to_reflex_map_for_builtins = {}
for python_type, reflex_type_name in ((int, 'int'),
                                      (float, 'float'),
                                      (double, 'double'),
                                      (str, 'std::string'),
                                      ):
    python_to_reflex_map_for_builtins[python_type] = reflex.Type.byName(reflex_type_name)

def find_reflex_type_corresponding_to_python_type(python_type):
    try:
        return python_type.__Reflex_Type__
    except AttributeError:
        return python_to_reflex_map_for_builtins[python_type]

########################################################################
#class Array(type):
#
#    def __new__(cls, element_type, size):
#        name = 'Array(%s)(%s)' % (element_type.__name__, size)
#
#        def __new__(size):
#            pass
#
#
#        arraydict = {}
#
#        return type(name, (), arraydict)


from reflex import drop_ownership, take_ownership, does_proxy_own
