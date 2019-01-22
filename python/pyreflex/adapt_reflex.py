import reflex

########################################################################
reflex.NULLPointer = reflex.Paux.NULLPointer
reflex.NULLPointer.__module__ = 'reflex'
########################################################################
def add_attribute_iteration(cls, name, class_method=False):
    if class_method:
        #@classmethod #(2.4 feature)
        def generator(cls):
            get = getattr(cls, name[:-1])
            size = getattr(cls, name[:-1]+'Count')
            #return (get(n) for n in xrange(size())) #(2.4 feature)
            return [get(n) for n in xrange(size())] 
        #generator.__name__ = name # Impossible for classmethods
        generator = classmethod(generator)
    else:
        def generator(self):
            get = getattr(self, name[:-1])
            size = getattr(self, name[:-1]+'Count')
            #return (get(n) for n in xrange(size())) #(2.4 feature)
            return [get(n) for n in xrange(size())] 
        #generator.__name__ = name #(2.4 feature)
        
    setattr(cls, name, generator)

for spec in ((reflex.Scope, 'subScopes'),
             (reflex.Scope, 'members'),
             (reflex.Scope, 'dataMembers'),
             (reflex.Scope, 'functionMembers'),
             (reflex.Scope, 'subTypes'),
             (reflex.Scope, 'memberTemplates'),
             (reflex.Scope, 'typeTemplates'),
             (reflex.Type, 'bases'),
             (reflex.Type, 'members'),
             (reflex.Type, 'parameters'),
             (reflex.Type, 'templateArguments'),
             (reflex.Member, 'templateArguments'),
             (reflex.TypeTemplate, 'instantiations'),
             (reflex.MemberTemplate, 'instantiations'),
             ##### Class does not exist any more(reflex.Class, 'members'),
             ##### Class does not exist any more(reflex.Class, 'dataMembers'),
             ##### Class does not exist any more(reflex.Class, 'functionMembers'),
             #(reflex.Type, 'types', True),
             #(reflex.Function, 'parameters'),
             ):
    add_attribute_iteration(*spec)
del add_attribute_iteration
########################################################################

__all__ = []
