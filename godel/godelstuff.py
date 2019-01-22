import types
########################################################################
def memoize(callable):
    cache = {}
    def proxy(*args):
        try:
            return cache[args]
        except KeyError:
            res = callable(*args)
            cache[args] = res
            return res
    proxy.__name__ = callable.__name__
    proxy.cache = cache
    return proxy

########################################################################
class NULLPointer(type):
    
    @memoize
    def __new__(cls, the_type):
        newdict = the_type.__dict__.copy()
        for name, attribute in newdict.items():
            #if type(attribute) is types.FunctionType:
            if type(attribute) is types.MethodType:
                newdict[name] = NULLPointer.make_safety_net(attribute, 'access a method', name)
            if type(attribute) is property:
                newdict[name] = property(NULLPointer.make_safety_net(getattr, 'access an attribute', name),
                                         NULLPointer.make_safety_net(setattr, 'set an attribute', name))

            def __nonzero__(self):
                return self._C_instance.get() != 0

        try:
            del newdict['__del__']
        except KeyError:
            pass
            
        newdict['__nonzero__'] = __nonzero__
        newdict['__the_type__'] = the_type
        return type.__new__(cls, "NULLPointer(%s)" % the_type.__name__, (), newdict)

    @staticmethod
    def make_safety_net(action, words, name):
        def safety_net(self, *args):
            if self._C_instance.get() == 0:
                # The pointer is still NULL
                raise NullPointer, "You are trying to %s via a NULL pointer." % (words)
            else:
                # Somehow the pointer was set to non-NULL, so switch to
                # normal pointer class, and try the operation again
                NULLPointer.make_valid(self)
                return action(self, *(name+args))
        safety_net.__name__ = name
        return safety_net
    
    @staticmethod
    def make_valid(the_pointer):
        if not isinstance(type(the_pointer), NULLPointer):
            raise "Eh? This branch should never be reached!"
        if the_pointer:
            the_pointer.__class__ = the_pointer.__the_type__
        return the_pointer
########################################################################

global_typeinfo_map_temporary_hack = {}
