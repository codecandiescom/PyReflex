import xml.parsers.expat

#filenames = set(['smallheader.h'])
#xmlfilename = 'smallheader.xml'
#
#
#file_ids = set()
#def find_files(name, attrs):
#    if name == "File" and attrs['name'] in filenames:
#        file_ids.add(attrs['id'])
#
#p = xml.parsers.expat.ParserCreate()
#p.StartElementHandler = find_files
#
#for line in file(xmlfilename):
#    p.Parse(line)

########################################################################
stack = []
all_items = {}
file_name_to_object = {}
g = None

def find_item(id):
    if id in all_items:
        return all_items[id]
    new = GCCXML.Unknown(id)
    all_items[id] = new
    return new

def gensym():
    count = [0]
    def gensym():
        count[0] += 1
        return count[0]
    return gensym
gensym = gensym()

class GCCXML(object):

    class Unknown(object):

        name = 'Unknown'

        def __init__(self, id):
            self.id = id

        def keep(self):
            return False

        def __repr__(self):
            return 'Unknown(%s)' % self.id
        
    class Base(object):

        def __new__(cls, attrs):
            if 'id' in attrs:
                id = attrs['id']
            else:
                id = gensym()
            self = find_item(id)
            self.__class__ = cls
            return self

        def __init__(self, attrs):
            stack.append(self)
            self.attrs = attrs
            self.__INIT__(attrs)

        def __INIT__(self, attrs):
            pass
        
        def __repr__(self):
            return self.__class__.__name__

        def bind(self, attname, bindname=None):
            if not bindname:
                bindname = attname
            # Get rid of unicode
            bindname = str(bindname)
            try:
                setattr(self, bindname, str(self.attrs[attname]))
            except KeyError:
                setattr(self, bindname, None)

        def bind_item(self, attname, bindname=None):
            if not bindname:
                bindname = attname
            try:
                setattr(self, bindname, find_item(self.attrs[attname]))
            except KeyError:
                setattr(self, bindname, None)

    class Class(Base):

        def __INIT__(self, attrs):
            self.bind_item('file')
            self.bind('name')
            try:
                self.members = [find_item(it) for it in attrs['members'].split()]
            except KeyError:
                self.members = []

        def keep(self):
            try:
                return self.file in file_ids
            except AttributeError:
                return False

        def __repr__(self):
            return self.name
        
        def show(self):
            return "(%s %s: %s)" % (self.__class__.__name__, self.name, self.members)

    #class Struct(Class):
    #    pass
    Struct = Class

    class Field(Base):

        def __INIT__(self, attrs):
            self.bind_item('type')
            self.bind('name')

        def __repr__(self):
            return "%s %s" % (self.type, self.name)

    class Constructor(Base):
        def __INIT__(self, attrs):
            self.bind_item('context')
            self.name = '__init__'
            self.returns = None
            if not hasattr(self, 'arguments'):
                self.arguments = []
        def __repr__(self):
            return "%s::%s(%s)" % (self.context.name, self.context.name, ', '.join(map(str,self.arguments)))
        def isCopyConstructor(self):
            if len(self.arguments) != 1:
                return
            tp = self.arguments[0].type # Throw away ArgumentType
            if type(tp) != GCCXML.ReferenceType:
                return
            tp = tp.type
            if type(tp) != GCCXML.CvQualifiedType:
                return
            if 'c' not in tp.id:
                return
            return True


    class Method(Base):

        def __INIT__(self, attrs):
            self.bind('name')
            self.bind_item('returns')
            self.bind_item('context')
            if not hasattr(self, 'arguments'):
                self.arguments = []
            if 'static' in attrs and attrs['static'] == "1":
                self.isStatic = True
            else:
                self.isStatic = False
            self.returns = find_item(attrs['returns'])

        def __repr__(self):
            return '%s %s(%s)' % (self.returns, self.name, ', '.join(map(str,self.arguments)))

    class Function(Method):
        pass

    class Typedef(Base):
        pass
                
    class Destructor(Base):
        pass
    class Variable(Base):
        pass
    class OperatorMethod(Method):
        def __INIT__(self, attrs):
            GCCXML.Method.__INIT__(self, attrs)
            self.name = 'operator%s' % self.name
                

    class FundamentalType(Base):

        def __INIT__(self, attrs):
            self.bind('name')

        def __repr__(self):
            return self.name
    class Converter(Base):
        pass
    class OperatorFunction(Base):
        pass

    class CvQualifiedType(Base):

        def __INIT__(self, attrs):
            self.bind_item('type')
            qual = []
            id = attrs['id']
            if 'c' in id:
                qual.append('const')
                self.const = True
            else:
                self.const = False
            if 'v' in id:
                qual.append('volatile')
            self.qual = ' '.join(qual)

        def __repr__(self):
            return "%s %s" % (self.qual, self.type)

        @property
        def name(self):
            return self.type.name
            #return "%s %s" % (self.qual, self.type.name)

    class Namespace(Base):
        def __INIT__(self, attrs):
            self.bind('name')
            if self.name == '::':
                global g
                g = self
            try:
                self.members = [find_item(it) for it in attrs['members'].split()]
            except KeyError:
                pass

        def __repr__(self):
            return "namespace %s" % self.name

    class Argument(Base):
        def __INIT__(self, attrs):
            self.bind_item('type')
            fn = stack[-2]
            if not hasattr(fn, 'arguments'):
                fn.arguments = []
            fn.arguments.append(self)
            ##self.type = attrs['type']
            #self.type = find_item(attrs['type'])
            pass
        def __repr__(self):
            return "%s" % self.type

    class Enumeration(Base):
        pass
    class EnumValue(Base):
        pass
    class GCC_XML(Base):
        id = None
    class Union(Base):
        pass
    class ArrayType(Base):
        pass

    class File(Base):
        def __INIT__(self, attrs):
            self.bind('name')
            file_name_to_object[self.name] = self
        
    class PointerType(Base):

        def __INIT__(self, attrs):
            self.attrs = attrs
            self.bind_item('type')
            #self.type = find_item(attrs['type'])

        def __repr__(self):
            return "%s*" % self.type

    class ReferenceType(PointerType):
        def __repr__(self):
            return "%s&" % self.type

    class Ellipsis(Base):
        pass
    class FunctionType(Base):
        pass


def collect_data(name, attrs):
    getattr(GCCXML, name)(attrs)

p = xml.parsers.expat.ParserCreate()
p.StartElementHandler = collect_data
def end(name):
    stack.pop()
p.EndElementHandler = end

def parse(xmlfilename):
    for line in file(xmlfilename):
        p.Parse(line)

if __name__ == '__main__':

    parse(xmlfilename)

    for c in all_items.itervalues():
        print c

    print len(all_items)
