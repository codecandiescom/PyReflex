#!/usr/bin/env python
# $Id: test_pyreflex.py,v 1.32 2005/03/03 10:16:26 jacek Exp $
import pyreflex
import unittest
import warnings
import sys
warnings.filterwarnings("ignore")

import sys
def commandarg(string):
    if string in sys.argv:
        sys.argv.remove(string)
        return True

fail = commandarg('fail')
seg  = commandarg('seg')
noexit = commandarg('noexit')
names = commandarg('names')
STL = commandarg('stl')
bench = commandarg('bench')
locateseg = commandarg('locateseg')


# Hide a whole suite at once
def hideSuite(suite):
    setattr(suite, 'setUp', lambda self: None)
    #for name in (name for name in dir(suite) if name.startswith('test')): #(2.4 feature)
    for name in [name for name in dir(suite) if name.startswith('test')]:
        setattr(suite, 'block'+name, getattr(suite, name))
        setattr(suite, name, lambda self: None)



if STL:
    pyreflex.loadDictionary('SealSTLDict')

if sys.platform == 'win32' : testdict = 'test_pyreflexDict'
else :                       testdict = 'libtest_pyreflex'                      
g = pyreflex.loadDictionary(testdict)

#g = pyreflex.BuildScope()
# helper for testIdentityOfNamespaces
#_original_foo = g.foo._C_metanamespace

class Pass(unittest.TestCase):
    pass

class Types(unittest.TestCase):

    def test_double(self):
        "The double type is not equivalent to float."
        double(2.3) == 2.3


# Abstract converter test. Inherit from this, and override the class
# variables to specify the actual types and values to be used
class ConverterTest(unittest.TestCase):

 #   aux_class = g.test_int
 #   initial_value = 7
 #   new_value = 9
 #   type = int

    def setUp(self):
        self.a = self.aux_class(self.initial_value)
        
    def testReturn(self):
        self.assertEqual(type(self.a.get()), self.type)
        self.assertEqual(self.a.get(), self.initial_value)

    def testReturnReference(self):
        self.assertEqual(type(self.a.getreference()), self.type)
        self.assertEqual(self.a.getreference(), self.initial_value)

    def testReturnPointer(self):
        self.assertEqual(type(self.a.getpointer()), self.type)
        self.assertEqual(self.a.getpointer(), self.initial_value)

    def testReturnReferenceToPointer(self):
        self.assertEqual(type(self.a.getpointerreference()), self.type)
        self.assertEqual(self.a.getpointerreference(), self.initial_value)

    def testGetField(self):
        self.assertEqual(type(self.a.a), self.type)
        self.assertEqual(self.a.a, self.initial_value)

    def testGetPointerField(self):
        self.assertEqual(type(self.a.ap), self.type)
        self.assertEqual(self.a.ap, self.initial_value)

    def nevertestSetPointerField(self): # A similar test passes in "class Other"
        self.assertEqual(self.a.ap, self.initial_value)
        self.a.ap = self.new_value
        self.assertEqual(self.a.ap, self.new_value)

    def testSetField(self):
        self.assertNotEqual(self.a.a, self.new_value)
        self.a.a = self.new_value
        self.assertEqual(self.a.a, self.new_value)

    def testSetter(self):
        self.assertNotEqual(self.a.a, self.new_value)
        self.a.set(self.new_value)
        self.assertEqual(self.a.a, self.new_value)

    def testArgument(self):
        self.assertEqual(self.a.take(self.new_value), self.new_value)

    def testPointerToArgument(self):
        self.assertEqual(self.a.takepointer(self.new_value), self.new_value)

    def testReferenceToArgument(self):
        self.assertEqual(self.a.takereference(self.new_value), self.new_value)

    def nevertestReferenceToPointerToArgument(self):
        self.assertEqual(self.a.takepointerreference(self.new_value), self.new_value)
    
class Bool(ConverterTest):

    aux_class = g.test_bool
    initial_value = True
    new_value = False
    type = bool


class Int(ConverterTest):

    aux_class = g.test_int
    initial_value = 7
    new_value = 9
    type = int


class Short_int(Int):
    aux_class = g.test_short_int

class Long_int(Int):
    aux_class = g.test_long_int


class Short(Int):
    aux_class = g.test_short

class Long(Short):
    aux_class = g.test_long

class Unsigned_int(Int):
    aux_class = g.test_unsigned_int

class Unsigned_short(Int):
    aux_class = g.test_unsigned_short
hideSuite(Unsigned_short)


class Double(ConverterTest):

    aux_class = g.test_double
    initial_value = 7.0
    new_value = 9.0
    type = float # Pedantically speaking, this should be "double"


class Float(ConverterTest):

    aux_class = g.test_float # This should be "test_float" !
    initial_value = 7.0
    new_value = 9.0
    type = float


class Std_string(ConverterTest):
    aux_class = g.test_std_string
    initial_value = 'fubar'
    new_value = 'spong'
    type = str

    segtestGetPointerField = 1
    segtestReturnPointer = 1

    failtestSetField = 1

############################################################
# See [3.9.1] in the C++ standard: in summary, char is a character,
# while unsigned char and signed char are small integer types.
class Char(ConverterTest):

    aux_class = g.test_char
    initial_value = 'p'
    new_value = 'y'
    type = str

    initial_value_long = 'more than one character'
    new_value_long = 'some other string'

    def testReturnPointerLong(self):
        a = self.aux_class(self.initial_value_long)
        self.assertEqual(type(a.getpointer()), self.type)
        self.assertEqual(a.getpointer(), self.initial_value_long)

    def testGetPointerFieldLong(self):
        a = self.aux_class(self.initial_value_long)
        self.assertEqual(type(a.ap), self.type)
        self.assertEqual(a.ap, self.initial_value_long)

    def testPointerToArgumentLong(self):
        a = self.aux_class(self.initial_value_long)
        self.assertEqual(a.takepointer(self.new_value_long), self.new_value_long)



class Unsigned_char(ConverterTest):

    aux_class = g.test_unsigned_char
    initial_value = 4
    new_value = 5
    type = int

class Signed_char(Unsigned_char):

    aux_class = g.test_signed_char
############################################################


class Class(unittest.TestCase):

    aux_class = g.test_user_defined
    initial_value = 7
    new_value = 9
    type = g.user_defined

    def setUp(self):
        self.a = self.aux_class(self.type(self.initial_value))
        
    def testReturn(self):
        self.assertEqual(type(self.a.get()), g.user_defined)
        self.assertEqual(self.a.get().i, 7)

    def testReturnReference(self):
        self.assertEqual(type(self.a.getreference()), g.user_defined)
        self.assertEqual(self.a.getreference().i, 7)

    def testReturnPointer(self):
        self.assertEqual(type(self.a.getpointer()), g.user_defined)
        self.a.getpointer()
        self.assertEqual(self.a.getpointer().i, 7)

    def testReturnReferenceToPointer(self):
        self.assertEqual(type(self.a.getpointerreference()), g.user_defined)
        self.assertEqual(self.a.getpointerreference().i, 7)

    def testGetField(self):
        self.assertEqual(type(self.a.a), g.user_defined)
        self.assertEqual(self.a.a.i, 7)

    def testGetPointerField(self):
        self.assertEqual(type(self.a.ap), g.user_defined)
        self.assertEqual(self.a.ap.i, 7)

    def testSetField(self):
        self.assertNotEqual(self.a.a.i, 9)
        self.a.a = g.user_defined(9)
        self.assertEqual(self.a.a.i, 9)

    def testSetter(self):
        self.assertNotEqual(self.a.a.i, 9)
        self.a.set(g.user_defined(9))
        self.assertEqual(self.a.a.i, 9)

    def testArgument(self):
        self.assertEqual(self.a.take(g.user_defined(3)).i, 3)

    def testPointerToArgument(self):
        self.assertEqual(self.a.takepointer(g.user_defined(3)).i, 3)

    def testReferenceToArgument(self):
        self.assertEqual(self.a.takereference(g.user_defined(3)).i, 3)
        
    def segtestReferenceToPointerToArgument(self):
        self.assertEqual(self.a.takepointerreference(g.user_defined(3)).i, 3)

class Class_typedef(Class):

    aux_class = g.test_TYPEDEF_user_defined
    type = g.TYPEDEF_user_defined

    # reflex.Member.set uses the sizeof(typedef) to memcpy the
    # field. We have been promised a fix.
    failtestSetField = 1


class CrossExtraction(unittest.TestCase):

    def testIntToFloat(self):
        self.assertEqual(g.test_float(1.0).take(4), 4.0)

    def testIntToDouble(self):
        self.assertEqual(g.test_double(1.0).take(4), 4.0)

    def testFloatToFloat(self):
        self.assertEqual(g.test_float(1.0).take(4.0), 4.0)


class PassingSemantics(unittest.TestCase):

    aux_class = g.user_defined
    initial_value = 7
    new_value = 9

    def setUp(self):
        self.original = self.aux_class(self.initial_value)
        self.holder = g.test_user_defined(self.original)

    def testReturnValue(self):
        """Return the same instance twice, by value. Modify one value,
        check that the other is not modified."""
        returned_one = self.holder.get()
        returned_two = self.holder.get()
        self.assertEqual(returned_one.i, self.initial_value)
        self.assertEqual(returned_two.i, self.initial_value)
        returned_one.i = self.new_value
        self.assertEqual(returned_two.i, self.initial_value)
        self.assertEqual(returned_one.i, self.new_value)
        
    def testReturnPointer(self):
        """Return the same instance twice, by pointer. Modify one value,
        check that the other is also modified."""
        returned_one = self.holder.getpointer()
        returned_two = self.holder.getpointer()
        self.assertEqual(returned_one.i, self.initial_value)
        self.assertEqual(returned_two.i, self.initial_value)
        returned_one.i = self.new_value
        self.assertEqual(returned_two.i, self.new_value)
        self.assertEqual(returned_one.i, self.new_value)

    def testReturnReference(self):
        """Return the same instance twice, by reference. Modify one value,
        check that the other is also modified."""
        returned_one = self.holder.getreference()
        returned_two = self.holder.getreference()
        self.assertEqual(returned_one.i, self.initial_value)
        self.assertEqual(returned_two.i, self.initial_value)
        returned_one.i = self.new_value
        self.assertEqual(returned_two.i, self.new_value)
        self.assertEqual(returned_one.i, self.new_value)

    def testPassValue(self):
        """Pass an instance by value, modify it, check that the
        original is unchanged."""
        aux = g.test_PassSemantics(self.new_value)
        orig = g.user_defined(self.initial_value)
        aux.pass_v(orig)
        self.assertEqual(orig.i, self.initial_value)
        
    def testPassPointer(self):
        """Pass an instance by pointer, modify it, check that the
        original is also changed."""
        aux = g.test_PassSemantics(self.new_value)
        orig = g.user_defined(self.initial_value)
        aux.pass_p(orig)
        self.assertEqual(orig.i, self.new_value)

    def testPassReference(self):
        """Pass an instance by reference, modify it, check that the
        original is also changed."""
        aux = g.test_PassSemantics(self.new_value)
        orig = g.user_defined(self.initial_value)
        aux.pass_r(orig)
        self.assertEqual(orig.i, self.new_value)

class Pointerclass(unittest.TestCase):

    def test_not_create(self):
        Class = pyreflex.Reflect.Class
        klass, deref = pyreflex.findPythonClass(Class.forName('test_pointerclass*'))
        self.assertEqual(deref, 1)
        self.assertEqual(klass, pyreflex.findPythonClass(Class.forName('test_pointerclass'))[0])
hideSuite(Pointerclass)

class Abstract(unittest.TestCase):

    def failtestAbstractInstantiation(self):
        self.assertRaises(pyreflex.ProxyClass._AbstractInstantiation, g.Abstract)

    # Merge the below into the test_pABC class used above ?
    def failtest_pABCconstructorException(self):
        self.assertRaises(Exception, g.PurelyAbstract)

    def testIsinstance(self):
        self.assert_(isinstance(g.PADerived(), g.PurelyAbstract))

    def test_pABCsubclass(self):
        pad = g.PADerived()
        self.assertEqual(pad.foo(), 'PADerived')

#class test_pABCXXXXX(unittest.TestCase):
#
#    def test_pointer_to_pABCXXXXX_field(self):
#        x = g.test_pABCXXXXX()


class Overload(unittest.TestCase):

    def setUp(self):
        self.b = g.Basic()

    def failtestBuiltins(self):
        b = g.Basic(1)
        self.assertEqual(b.overload (), 0)
        self.assertEqual(b.overload (1), 1)
        self.assertEqual(b.overload (1.0), 2)
        self.assertEqual(b.overload (1, 1), 3)

        self.assertEqual(b.overload2(), 0)
        self.assertEqual(b.overload2(1), 1)
        self.assertEqual(b.overload2(1.0), 2)
        self.assertEqual(b.overload2(1, 1), 3)

    # User defined
    # User defined and builtins
    # Optional arguments
    def testOptionalArguments(self):
        # Improve this, once we can return collections
        b = g.Basic(1)
        self.assertEqual(b.optional(), 6)
        self.assertEqual(b.optional(10), 15)
        self.assertEqual(b.optional(10,100), 113)
        self.assertEqual(b.optional(10,100,1000), 1110)
        self.assertRaises(TypeError, b.optional, 1,2,3,'a')
                      
class Destructor(unittest.TestCase):

    def setUp(self):
        self.t = g.test_destructor
        self.s = self.t.count()
        
    def testConstruction(self):
        a = g.test_destructor()
        self.assertEqual(pyreflex.does_proxy_own(a), True)

    def testByValue(self):
        a = g.test_destructor.getByValue()
        self.assertEqual(pyreflex.does_proxy_own(a), True)

    def testByPointer(self):
        a = g.test_destructor.getByPointer()
        self.assertEqual(pyreflex.does_proxy_own(a), False)
        
    def testByReference(self):
        a = g.test_destructor.getByReference()
        self.assertEqual(pyreflex.does_proxy_own(a), False)

    def testSwitchOwnership(self):
        getters = (g.test_destructor.getByReference,
                   g.test_destructor,
                   g.test_destructor.getByValue,
                   g.test_destructor.getByPointer,)

        for getit in getters:
            a = getit()
            pyreflex.drop_ownership(a)
            self.assertEqual(pyreflex.does_proxy_own(a), False)
            pyreflex.take_ownership(a)
            self.assertEqual(pyreflex.does_proxy_own(a), True)

        for getit in getters:
            a = getit()
            pyreflex.take_ownership(a)
            self.assertEqual(pyreflex.does_proxy_own(a), True)
            pyreflex.drop_ownership(a)
            self.assertEqual(pyreflex.does_proxy_own(a), False)

    def testOwnership(self):
        self.assertEqual(self.t.count() - self.s, 0)
        a = g.test_destructor(); pyreflex.take_ownership(a)
        self.assertEqual(self.t.count() - self.s, 1)
        b = g.test_destructor(); pyreflex.take_ownership(b)
        self.assertEqual(self.t.count() - self.s, 2)
        c = g.test_destructor(); pyreflex.take_ownership(c)
        self.assertEqual(self.t.count() - self.s, 3)
        del a
        self.assertEqual(self.t.count() - self.s, 2)
        del c
        self.assertEqual(self.t.count() - self.s, 1)
        del b
        self.assertEqual(self.t.count() - self.s, 0)

        a = g.test_destructor(); pyreflex.drop_ownership(a)
        self.assertEqual(self.t.count() - self.s, 1)
        b = g.test_destructor(); pyreflex.drop_ownership(b)
        self.assertEqual(self.t.count() - self.s, 2)
        c = g.test_destructor(); pyreflex.drop_ownership(c)
        self.assertEqual(self.t.count() - self.s, 3)
        del a
        self.assertEqual(self.t.count() - self.s, 3)
        del c
        self.assertEqual(self.t.count() - self.s, 3)
        del b
        self.assertEqual(self.t.count() - self.s, 3)

        a = g.test_destructor(); pyreflex.drop_ownership(a)
        self.assertEqual(self.t.count() - self.s, 4)
        b = g.test_destructor(); pyreflex.take_ownership(b)
        self.assertEqual(self.t.count() - self.s, 5)
        c = g.test_destructor(); pyreflex.drop_ownership(c)
        self.assertEqual(self.t.count() - self.s, 6)
        d = g.test_destructor(); pyreflex.take_ownership(d)
        self.assertEqual(self.t.count() - self.s, 7)
        del a
        self.assertEqual(self.t.count() - self.s, 7)
        del b
        self.assertEqual(self.t.count() - self.s, 6)
        del c
        self.assertEqual(self.t.count() - self.s, 6)
        del d
        self.assertEqual(self.t.count() - self.s, 5)

        
class SequenceProtocol(unittest.TestCase):

    def setUp(self):
        #self.v = g.std.vector(int)()
        self.v = g.VectorInt()
        self.l = range(10,20)
        for i in self.l: self.v.push_back(i)

    def testlen(self):
        self.assertEqual(len(self.v), len(self.l))

    def testIndex(self):
        self.assertEqual(self.v[3], self.l[3])

    def testNegativeIndex(self):
        self.assertEqual(self.v[-2], self.l[-2])

    def ICG(self, it, arg):
        "Implicitly call __getattr__ via []."
        return it[arg]

    def testOutOfRange(self):
        self.assertRaises(IndexError, self.ICG, self.v, len(self.l))
        self.assertRaises(IndexError, self.ICG, self.v, -1-len(self.l))

    def testForLoop(self):
        self.assertEqual([i for i in self.v], [i for i in self.l])

    def testSlice(self):
        vsl = self.v[1:4]
        lsl = self.l[1:4]
        self.assertEqual(len(vsl), len(lsl))
        #self.assertEqual(type(vsl), type(self.v))
        self.assertEqual((vsl[0], vsl[1], vsl[2]), (lsl[0], lsl[1], lsl[2]))
        
    def testNoneSlice(self):
        vsl = self.v[:2]
        lsl = self.l[:2]
        self.assertEqual(len(vsl), len(lsl))
        #self.assertEqual(type(vsl), type(self.v))
        self.assertEqual((vsl[0], vsl[1]), (lsl[0], lsl[1]))

    def testSliceNone(self):
        vsl = self.v[len(self.l)-2:]
        lsl = self.l[len(self.l)-2:]
        self.assertEqual(len(vsl), len(lsl))
        #self.assertEqual(type(vsl), type(self.v))
        self.assertEqual((vsl[0], vsl[1]), (lsl[0], lsl[1]))

    def testStepSlice(self):
        vsl = self.v[1:4:2]
        self.assertEqual(len(vsl), 2)
        #self.assertEqual(type(vsl), type(self.v))
        self.assertEqual((vsl[0], vsl[1]), (self.v[1], self.v[3]))

    def testSliceReverse(self):
        vsl = self.v[::-1]
        lsl = self.l[::-1]
        self.assertEqual(len(vsl), len(self.l))
        #self.assertEqual(type(vsl), type(self.v))
        self.assertEqual([i for i in vsl], [i for i in lsl])
        self.l.reverse()
        self.assertEqual([i for i in vsl], self.l)


class Operator(unittest.TestCase):

    def test_add(self):
        self.assertEqual(g.Number(20) + g.Number(10), g.Number(30) )

    def test_sub(self):
        self.assertEqual(g.Number(20) - g.Number(10), g.Number(10) )

    def test_div(self):
        self.assertEqual(g.Number(20) / g.Number(10), g.Number(2) )

    def test_mul(self):
        self.assertEqual(g.Number(20) * g.Number(10), g.Number(200) )

    def test_and(self):
        self.assertEqual(g.Number(5)  & g.Number(14), g.Number(4) )

    def test_or(self):
        self.assertEqual(g.Number(5)  | g.Number(14), g.Number(15) )

    def test_xor(self):
        self.assertEqual(g.Number(5)  ^ g.Number(14), g.Number(11) )

    def test_lshift(self):
        self.assertEqual(g.Number(5)  << 2, g.Number(20) )

    def test_rshift(self):
        self.assertEqual(g.Number(20) >> 2, g.Number(5) )

    def test_iadd(self):
        n = g.Number(20)
        n += g.Number(10)
        self.assertEqual(n, g.Number(30))

    def test_isub(self):
        n = g.Number(20)
        n -= g.Number(10)
        self.assertEqual(n, g.Number(10))

    def test_idiv(self):
        n = g.Number(20)
        n /= g.Number(2)
        self.assertEqual(n, g.Number(10))

    def test_imul(self):
        n = g.Number(20)
        n *= g.Number(10)
        self.assertEqual(n, g.Number(200))

    def test_iand(self):
        n = g.Number(5)
        n &= g.Number(14)
        self.assertEqual(n, g.Number(4) )

    def test_ior(self):
        n = g.Number(5)
        n |= g.Number(14)
        self.assertEqual(n, g.Number(15) )

    def test_ixor(self):
        n = g.Number(5)
        n ^= g.Number(14)
        self.assertEqual(n, g.Number(11) )

    def test_ilshift(self):
        n = g.Number(5)
        n <<= 2
        self.assertEqual(n, g.Number(20) )

    def test_irshift(self):
        n = g.Number(20)
        n >>= 2
        self.assertEqual(n, g.Number(5) )

#"<<=" : "__ilshift__"
#">>=" : "__irshift__"

    def test_gt(self):
        self.assert_(    g.Number(2) >  g.Number(1) )
        self.assert_(not g.Number(2) >  g.Number(2) )
        self.assert_(not g.Number(2) >  g.Number(3) )
                                        
    def test_lt(self):                  
        self.assert_(not g.Number(2) <  g.Number(1) )
        self.assert_(not g.Number(2) <  g.Number(2) )
        self.assert_(    g.Number(2) <  g.Number(3) )

    def test_ge(self):                  
        self.assert_(    g.Number(2) >= g.Number(1) )
        self.assert_(    g.Number(2) >= g.Number(2) )
        self.assert_(not g.Number(2) >= g.Number(3) )

    def test_le(self):                  
        self.assert_(not g.Number(2) <= g.Number(1) )
        self.assert_(    g.Number(2) <= g.Number(2) )
        self.assert_(    g.Number(2) <= g.Number(3) )

    def test_ne(self):                  
        self.assertNotEqual(g.Number(2), g.Number(1) )
        self.assertEqual   (g.Number(2), g.Number(2) )
        self.assertNotEqual(g.Number(2), g.Number(3) )

    def test_eq(self):                  
        self.assertNotEqual(g.Number(2), g.Number(1) )
        self.assertEqual   (g.Number(2), g.Number(2) )
        self.assertNotEqual(g.Number(2), g.Number(3) )

    def test_mod(self):
        self.assertEqual(g.Number(2) % g.Number(3), g.Number(2))
        self.assertEqual(g.Number(3) % g.Number(3), g.Number(0))
        self.assertEqual(g.Number(4) % g.Number(3), g.Number(1))

    def test_imod(self):
        n = g.Number(2)
        n %= g.Number(3)
        self.assertEqual(n, g.Number(2))
        n = g.Number(3)
        n %= g.Number(3)
        self.assertEqual(n, g.Number(0))
        n = g.Number(4)
        n %= g.Number(3)
        self.assertEqual(n, g.Number(1))

#"**"  : "__pow__" There is no such operator in C
#"**=" : "__ipow__"


class Other(unittest.TestCase):

    def testDefaultConstructor(self):
        b = g.Basic()
        self.assertEqual(type(b), g.Basic)
        self.assertEqual(b.i, 1) 
        self.assertEqual(b.d, 3.1)

    def testOneArgumentConstructor(self):
        b = g.Basic(2)
        self.assertEqual(type(b), g.Basic)

    def testStaticMethod(self):
        self.assertEqual(g.Basic.staticmethod(8), 14)
        self.assertEqual(g.Basic(1).staticmethod(8), 14)

    def testGlobalFunction(self):
        self.assertEqual(g.global_function(6), 13)
        
    def testGetPointerField(self):
        b = g.Basic(7)
        self.assertEqual(type(b.ip), int)
        self.assertEqual(b.ip, 7)

    def testSetPointerField(self):
        b = g.Basic(10)
        self.assertEqual(b.ip, 10)
        b.i = 9
        self.assertEqual(b.ip, 9)


# This looks to be testing for an implementation-specific problem in
# PyLCGDict ... remove when confirmed.

#class Identity(unittest.TestCase):
#
#    def testMethodArgumentTypes(self):
#        it = g.Identity.ints.dispatcher.methods[0]
#        p = [t._pointer() for t in it.argumentTypesCached]
#        self.assert_(p[0] == p[1] == p[2] == p[3])
#
#        it = g.Identity.classes.dispatcher.methods[0]
#        p = [t._pointer() for t in it.argumentTypesCached]
#        self.assert_(p[0] == p[1] == p[2] == p[3])
#
#    def testMethodArgumentTypesPointers(self):
#        it = g.Identity.intsPtr.dispatcher.methods[0]
#        p = [t._pointer() for t in it.argumentTypesCached]
#        self.assert_(p[0] == p[1] == p[2] == p[3])
#        
#        it = g.Identity.classesPtr.dispatcher.methods[0]
#        p = [t._pointer() for t in it.argumentTypesCached]
#        self.assert_(p[0] == p[1] == p[2] == p[3])
#
#        # The following was passing even when the ROOT dictionary was
#        # failing something very similar
#        
#        it = g.Identity.charPtr.dispatcher.methods[0]
#        p = [t._pointer() for t in it.argumentTypesCached]
#        self.assert_(p[0] == p[1] == p[2] == p[3])
#hideSuite(Identity)


class Inheritance(unittest.TestCase):

    derived = g.Derived
    base = g.Base
    pass_base = g.test_pass_Base
    
    def testIsinstance(self):
        self.assert_(isinstance(self.derived(), self.base))

    def test__bases__(self):
        self.assert_(self.base in self.derived.__bases__)

    def testInheritField(self):
        d = self.derived()
        self.assertEqual(d.i, 123456789)
        
    def testInheritMethod(self):
        d = self.derived()
        self.assertEqual(d.geti(), 123456789)

    def testReturnReflectedPointerCorrectDynamicType(self):
        d = self.derived()
        self.assertEqual(type(d.baseDp()), self.derived)

    def testReturnReflectedReferenceCorrectDynamicType(self):
        # Waiting for Reflection to provide a Class::dynamic_type()
        d = self.derived()
        self.assertEqual(type(d.baseDr()), self.derived)

    def testPassSubclassPointer(self):
        bt = self.pass_base()
        d = self.derived(7)
        self.assertEqual(bt.takepointerm(d), 7)
        self.assertEqual(bt.takepointerf(d), 7)

    def testPassSubclassReference(self):
        bt = self.pass_base()
        d = self.derived(7)
        self.assertEqual(bt.takereferencem(d), 7)
        self.assertEqual(bt.takereferencef(d), 7)

    def testPassSubclassValue(self):
        bt = self.pass_base()
        d = self.derived(7)
        self.assertEqual(bt.takevaluem(d), 7)
        self.assertEqual(bt.takevaluef(d), 7)

    ##########

    def failtestP_InheritField(self):
        dp = pyreflex.Pointer(self.derived)(self.derived())
        self.assertEqual(dp.i, 123456789)
        
    def failtestP_InheritMethod(self):
        dp = pyreflex.Pointer(self.derived)(self.derived())
        self.assertEqual(dp.geti(), 123456789)
    
    #def testP_ReturnReflectedPointerCorrectDynamicType(self):
    #    d = self.derived()
    #    self.assertEqual(type(d.baseDp()), self.derived)
    #
    #def testP_ReturnReflectedReferenceCorrectDynamicType(self):
    #    # Waiting for Reflection to provide a Class::dynamic_type()
    #    d = self.derived()
    #    self.assertEqual(type(d.baseDr()), self.derived)

    def failtestP_PassSubclassPointer(self):
        bt = self.pass_base()
        dp = pyreflex.Pointer(self.derived)(self.derived(7))
        self.assertEqual(bt.takepointerm(dp), 7)
        self.assertEqual(bt.takepointerf(dp), 7)

    def failtestP_PassSubclassReference(self):
        bt = self.pass_base()
        dp = pyreflex.Pointer(self.derived)(self.derived(7))
        self.assertEqual(bt.takereferencem(dp), 7)
        self.assertEqual(bt.takereferencef(dp), 7)

    def failtestP_PassSubclassValue(self):
        bt = self.pass_base()
        dp = pyreflex.Pointer(self.derived)(self.derived(7))
        self.assertEqual(bt.takevaluem(dp), 7)
        self.assertEqual(bt.takevaluef(dp), 7)


class VirtualInheritance(Inheritance):

    derived = g.DerivedVirtually


class Weird(Inheritance):

    derived = g.DerivedVirtually

    def Diamond(self):
        D = g.Diamond()
        D.i
        
    def Self(self):
        d = self.derived()
        self.assertEqual(d.i, 123456789)

    # These tests were due to a weird Reflex bug where changing the
    # order of class use made for either segfaulting or just regular
    # operations. Fixed now.
        
    def testDS(self): # seg
        self.Diamond()
        self.Self()

    def testSD(self): # fine
        self.Self()
        self.Diamond()

    def testDSD(self): # seg
        self.Diamond()
        self.Self()
        self.Diamond()

    def testSDS(self): # fine
        self.Self()
        self.Diamond()
        self.Self()

        

class VVirtualInheritance(Inheritance):

    derived = g.VDerivedVirtually
    base = g.VBase
    pass_base = g.test_pass_VBase
    
    #failtestReturnReflectedPointerCorrectDynamicType = 1
    #failtestReturnReflectedReferenceCorrectDynamicType = 1
    #segtestPassSubclassPointer = 1
    #segtestPassSubclassReference = 1 
    #segtestPassSubclassValue = 1

class Diamond(unittest.TestCase): 

    def testIsinstance(self):
        self.assert_(isinstance(g.Diamond(), g.Base))
        self.assert_(isinstance(g.Diamond(), g.DerivedVirtually))
        self.assert_(isinstance(g.Diamond(), g.DerivedVirtually2))

    def test__bases__(self):
        self.assert_(g.Base in g.Diamond.__mro__)

    def testInheritField(self):
        d = g.Diamond()
        self.assertEqual(d.i, 123456789)
        
    def testInheritMethod(self):
        d = g.Diamond()
        self.assertEqual(d.geti(), 123456789)

    def testPassSubclassPointer(self):
        bt = g.test_pass_Base()
        d = g.Diamond(7)
        self.assertEqual(bt.takepointerm(d), 7)
        self.assertEqual(bt.takepointerf(d), 7)

    def testPassSubclassReference(self):
        bt = g.test_pass_Base()
        d = g.Diamond(7)
        self.assertEqual(bt.takereferencem(d), 7)
        self.assertEqual(bt.takereferencef(d), 7)

    def testPassSubclassValue(self):
        bt = g.test_pass_Base()
        d = g.Diamond(7)
        self.assertEqual(bt.takevaluem(d), 7)
        self.assertEqual(bt.takevaluef(d), 7)
    ###
    def failtestP_PassSubclassPointer(self):
        bt = g.test_pass_Base()
        dp = pyreflex.Pointer(g.Diamond)(g.Diamond(7))
        self.assertEqual(bt.takepointerm(dp), 7)
        self.assertEqual(bt.takepointerf(dp), 7)

    def failtestP_PassSubclassReference(self):
        bt = g.test_pass_Base()
        dp = pyreflex.Pointer(g.Diamond)(g.Diamond(7))
        self.assertEqual(bt.takereferencem(dp), 7)
        self.assertEqual(bt.takereferencef(dp), 7)

    def failtestP_PassSubclassValue(self):
        bt = g.test_pass_Base()
        dp = pyreflex.Pointer(g.Diamond)(g.Diamond(7))
        self.assertEqual(bt.takevaluem(dp), 7)
        self.assertEqual(bt.takevaluef(dp), 7)

class Namespace(unittest.TestCase):

    # Lazy filling of namespaces is not implemented in Pyreflex ... yet.
    def failtestProxy(self):
        "A NamespaceAttributeProxy is evaluated too early or too late."
        self.assert_('bar' in g.foo.__class__.__dict__)
        self.assert_('bar' not in g.foo.__dict__)
        g.foo.bar
        self.assert_('bar' in g.foo.__dict__)
        self.assertEqual(type(g.foo.bar.baz), pyreflex.ProxyClass)

    # This relies of having a second dictionary, and scram makes
    # having two dictionaries too painful to bother with this test for
    # now.
    def failtestUpdate(self):
        "Loading a new dictionary has failed to update an existing namespace."
        self.assert_(not hasattr(g.foo.bar, 'zot'))
        self.assert_(not hasattr(g.foo.bar, 'quux'))
        pyreflex.loadDictionary('pyreflex_seconddict')
        self.assertEqual(type(g.foo.bar.zot), pyreflex.ProxyClass)
        self.assertEqual(isinstance(g.foo.bar.quux, pyreflex.NamespaceClassBase))
        

    # This one relies on two dictionaries ... see comment for previous test.

    # Thrown a 'z' into the name to force execution _after_ the other test.
    # Is the alphabetic ordering guaranteed ? (yes, the current unittest sorts the names)
    def failtestzIdentityOfNamespaces(self):
        "The identity of a reflecion::Namespace instance changes."
        self.assertEqual(_original_foo._pointer(), g.foo._C_metanamespace._pointer())
        # remember to remove the following, when this bug is fixed:
        #  _original_foo
        #  NampspaceClassBase._selfupdate (and calls to it)

# In Pylcgdict, template arguments were specified with
# parentheses. Now that function templates are available, this seems
# to be a bad choice, as parentheses following a template object could
# mean one of 2 things:

# 1) specification of template arguments (<...> in C++),

# 2) passing of function arguments.

# Therefore it is probably a better idea to use brackets [], rather
# than parentheses (), to specify template arguments. We keep the
# tests for both versions until we are sure which we prefer.

# Testing the parenthesis-based syntax.
class ClassTemplates(unittest.TestCase):

    def testOneTemplateArgument(self):
        foo = g.T.foo
        fi = foo(int)(9)
        self.assert_(type(fi) is foo(int))
        self.assertEqual(fi.it, 9)
        self.assertEqual(fi.get(), 9)
        fb = foo(g.Basic)(g.Basic(8))
        self.assert_(type(fb) is foo(g.Basic))
        self.assertEqual(fb.it.i, 8)
        self.assertEqual(fb.get().i, 8)

    def testDistinguishFloatAndDouble(self):
        "Float and dobule as template arguments are not distinguished correctly."
        foo = g.T.foo
        self.assert_(foo(float) is foo(float))
        self.assert_(foo(double) is foo(double))
        self.assert_(foo(double) is not foo(float))
        self.assertEqual(foo(double)(2.3).it, 2.3)
        self.assertNotEqual(foo(float )(2.3).it,  2.3) # Loss of precision

    def testTwoTemplateArguments(self):
        "Instantiation of a two-template argument class, fails."
        bar = g.T.bar
        bii = bar(int, int)(2,3)
        self.assert_(type(bii) is bar(int, int))
        self.assert_(bii.t == 2 and bii.u == 3)

    def testTemplatedClassesInNamespaces(self):
        "Templated class is not correctly installed in its namespace."
        self.assertEqual(type(g.T.foo(int)(9)), g.T.foo(int))
        self.assertEqual(g.T.foo(int)(9).it, 9)

    def testNested(self): # the argument type is templated itself
        "Tripped over a template argument which itself is templated. (user-defined arg conversion)"
        foo = g.T.foo
        #fii = foo(foo(int))(7)
        fii = foo(foo(int))(foo(int)(7))
        self.assertEqual(type(fii), foo(foo(int)))
        self.assertEqual(fii.it.it, 7)

# Testing the bracked-based syntax.
class ClassTemplates(unittest.TestCase):

    def testOneTemplateArgument(self):
        foo = g.T.foo
        fi = foo[int](9)
        self.assert_(type(fi) is foo[int])
        self.assertEqual(fi.it, 9)
        self.assertEqual(fi.get(), 9)
        fb = foo[g.Basic](g.Basic(8))
        self.assert_(type(fb) is foo[g.Basic])
        self.assertEqual(fb.it.i, 8)
        self.assertEqual(fb.get().i, 8)

    def testDistinguishFloatAndDouble(self):
        "Float and dobule as template arguments are not distinguished correctly."
        foo = g.T.foo
        self.assert_(foo[float] is foo[float])
        self.assert_(foo[double] is foo[double])
        self.assert_(foo[double] is not foo[float])
        self.assertEqual(foo[double](2.3).it, 2.3)
        self.assertNotEqual(foo[float ](2.3).it,  2.3) # Loss of precision

    def testTwoTemplateArguments(self):
        "Instantiation of a two-template argument class, fails."
        bar = g.T.bar
        bii = bar[int, int](2,3)
        self.assert_(type(bii) is bar[int, int])
        self.assert_(bii.t == 2 and bii.u == 3)

    def testTemplatedClassesInNamespaces(self):
        "Templated class is not correctly installed in its namespace."
        self.assertEqual(type(g.T.foo[int](9)), g.T.foo[int])
        self.assertEqual(g.T.foo[int](9).it, 9)

    def testNested(self): # the argument type is templated itself
        "Tripped over a template argument which itself is templated. (user-defined arg conversion)"
        foo = g.T.foo
        #fii = foo(foo(int))(7)
        fii = foo[foo[int]](foo[int](7))
        self.assertEqual(type(fii), foo[foo[int]])
        self.assertEqual(fii.it.it, 7)

class NonMemberFunctionTemplates(unittest.TestCase):

    SCOPE = g.T

    def testOneArg(self):
        self.assertEqual(self.SCOPE.abs[int](-2), 2)
        self.assertEqual(self.SCOPE.abs[double](-2.4), 2.4)
        a = g.T.aux[int](9)
        self.assertEqual(self.SCOPE.abs[g.T.aux[int]](a).it, 9)

    # Looks like dictionary generation bugs in scope resolution
    def failtestOneNestedTemplateArg(self):
        a = g.T.aux[int](9)
        aa = g.T.aux[g.T.aux[int]](a)
        self.assertEqual(self.SCOPE.abs[g.T.aux[g.T.aux[int]]](aa).it.it, 9)
        
    def testTwoArg(self):
        self.assertEqual(self.SCOPE.max[int, int](2,3), 3)
        self.assertEqual(self.SCOPE.max[double, double](2.4, 2.5), 2.5)

class MemberFunctionTemplates(NonMemberFunctionTemplates):

    SCOPE = g.T.S()

class NestedClasses(unittest.TestCase):

    def testType(self):
        self.assertEqual(type(g.Outer.Inner(2.3)), g.Outer.Inner)

    def testGetter(self):
        self.assertEqual(g.Outer.Inner(2.3).getd(), 2.3)

    def testSomething(self):
        o = g.Outer(g.Outer.Inner(7.1), 3.6)
        self.assertEqual(type(o), g.Outer)

    def testKitchenSink(self):
        "Something fails in nested class support." # Probably want to split this into separate tests
        self.assertEqual(g.Outer.Inner(2.3).d, 2.3)
        o = g.Outer(g.Outer.Inner(7.1), 3.6)
        self.assertEqual(o.i.d, 7.1)

class SelfReferential(unittest.TestCase):

    def hidetestIt(self):
        g.self_referential_field

class AbsOnConc(unittest.TestCase):

    def hidetestIt(self):
        g.AbstractInheritsConcrete

class DynamicType(unittest.TestCase):

    def testBasic(self):
        derived = g.dynamicTypeDerived(2)
        base = derived.returnBase(3)
        self.assert_(not hasattr(base, 'addedMethod'))
        derived2 = derived.returnDerived(4)
        self.assert_(type(derived2) is g.dynamicTypeDerived)
        self.assertEqual(derived2.addedMethod(), 5)

    def testWithoutDefaultConstructor(self):
        derived = g.dynamicTypeDerivedWithoutDefaultConstructor(2)
        base = derived.returnBase(3)
        self.assert_(not hasattr(base, 'addedMethod'))
        derived2 = derived.returnDerived(4)
        self.assert_(type(derived2) is g.dynamicTypeDerivedWithoutDefaultConstructor)
        self.assertEqual(derived2.addedMethod(), 5)

    def testVirtually(self):
        derived = g.dynamicTypeDerivedVirtually(5)
        base = derived.returnBase(6)
        self.assert_(not hasattr(base, 'addedMethodVirtually'))
        derived2 = derived.returnDerivedVirtually(7)
        self.assert_(type(derived2) is g.dynamicTypeDerivedVirtually)
        self.assertEqual(derived2.addedMethodVirtually(), 8)

    def segtestVectorOfPointers(self):
        vector = g.dynamicTypeDerivedVirtually(5).returnVectorOfPointers()
        self.assert_(type(vector[0]) is g.dynamicTypeBase)
        self.assert_(not hasattr(vector[0], 'addedMethod'))
        self.assert_(not hasattr(vector[0], 'addedMethodVirtually'))
        self.assert_(type(vector[1]) is g.dynamicTypeDerived)
        self.assertEqual(hasattr(vector[1], 'addedMethod'))
        self.assertEqual(vector[1].addedMethod(), 12)
        self.assert_(type(vector[2]) is g.dynamicTypeDerivedVirtually)
        self.assertEqual(hasattr(vector[2], 'addedMethodVirtually'))
        self.assertEqual(vector[2].addedMethodVirtually(), 13)

    def testDynamicTypeNotInDictionary(self):
        result = g.dynamicTypeDerived().returnNotInDictionary()
        self.assert_(type(result) is g.dynamicTypeBase)
                     
class getDynamicType(unittest.TestCase):

    # Not sure how to induce getDynamicType to return a null dynamic
    # type ... so this test remains unfinished.
    def DO_NOT_testReturnsNullPointersCorrectly(self):
        input = pyreflex.Pointer(g.Base()) # Incompatible with the type to which we will cast
        NULLout = pyreflex.converters.getDynamicType(input, g.dynamicTypeBase._C_metaclass)
        self.assert_(type(NULLout) is pyreflex.NULLPointer(g.dynamicTypeBase))

class StaticMethods(unittest.TestCase):
    Class = g.test_static_methods
    def setUp(self):
        self.instance = self.Class(5)

    def testNoargs(self):
        self.assertEqual(self.instance.noargs(), 19)
        self.assertEqual(self.Class.noargs(), 19)

    def testOneArg(self):
        self.assertEqual(self.instance.onearg(3), 9)
        self.assertEqual(self.Class.onearg(3), 9)

    def testTwoArg(self):
        self.assertEqual(self.instance.twoarg(14,15), 29)
        self.assertEqual(self.Class.twoarg(14,15), 29)

    def testOverloaded(self):
        self.assertEqual(self.instance.overloaded(), 19)
        self.assertEqual(self.instance.overloaded(3), 9)
        self.assertEqual(self.instance.overloaded(14,15), 29)
        self.assertEqual(self.Class.overloaded(), 19)
        self.assertEqual(self.Class.overloaded(3), 9)
        self.assertEqual(self.Class.overloaded(14,15), 29)

    def nevertestEvil(self):
        self.assertEqual(self.instance.evil(), 5)
        self.assertEqual(self.Class.evil(3), 675)
        self.assertEqual(self.Class.more_evil(), 668)
        self.assertEqual(self.instance.more_evil(3), 683)


class OutputParameter(unittest.TestCase):

    def setUp(self):
        self.t = g.test_output_parameter()

    def testRedirect(self):
        bp = pyreflex.Pointer(g.Basic)(0)
        self.t.redirect(bp)
        self.assertEqual(bp.i, 2)

    def testMutate(self):
        bp = pyreflex.Pointer(g.Basic)(g.Basic(4))
        self.assertEqual(bp.i, 4)
        self.t.mutate(bp)
        self.assertEqual(bp.i, 3)
hideSuite(OutputParameter)
    

class xDir(unittest.TestCase): # x to ensure it gets run after dict is loaded

    def testTemplatesInNamespaces(self):
        diroutput = dir(g.T)
        names = ['foo<>', 'bar<>']
        for name in names:
            self.assert_(name in diroutput, name+str(diroutput)) 

    def testTemplatesInTemplateItself(self):
        diroutput = dir(g.T.foo)
        names = ['foo<Basic>', 'foo<T::foo<int> >', 'foo<double>', 'foo<float>', 'foo<int>']
        for name in names:
            self.assert_(name in diroutput, name+str(diroutput)) 
hideSuite(xDir)


class UpcastExtractor(unittest.TestCase):

    def testByPointer(self):
        tue = g.test_upcast_extractor
        arg = tue.Derived(123)
        self.assertEqual(tue.by_pointer(arg), 123)

    def testByReference(self):
        tue = g.test_upcast_extractor
        arg = tue.Derived(123)
        self.assertEqual(tue.by_reference(arg), 123)

    if sys.platform == 'win32':
        def failtestViaNotInDict(self):
            bval, tval = 5, 9
            TNT = g.upcast_through_not_in_dict
            there = TNT.There(tval)
            base = TNT.Base(bval)
            self.assertEqual(base.doit(there), tval)
    else:
        def testViaNotInDict(self):
            bval, tval = 5, 9
            TNT = g.upcast_through_not_in_dict
            there = TNT.There(tval)
            base = TNT.Base(bval)
            self.assertEqual(base.doit(there), tval)

class NamespaceSeparation(unittest.TestCase):

    def testIt(self):
        self.assert_(g.namespace_one.the_class is not g.namespace_two.the_class)


class NullPointerPlain(unittest.TestCase):

    this_capsule = g.test_null_pointer
    other_capsule = g.test_null_pointerDynamic
    this_type = g.Basic

    def setUp(self):
        self.b = self.this_capsule.get_invalid()

    def testTruth(self):
        self.assert_(not self.b)

    def testType(self):
        self.assert_(type(self.b) is pyreflex.NULLPointer(self.this_type))

    def testGet(self):
        self.assertRaises(pyreflex.NullPointer, lambda :self.b.i)
        self.this_capsule.mutate(self.b)
        self.assertEqual(self.b.i, 1)

    def testActualPointer(self):
        self.assertEqual(self.this_capsule.report_pointer(self.b), 0)

    def testTypeError(self):
        self.assertRaises(TypeError, self.other_capsule.report_pointer, self.b)

    def testRejectNULLpointerByValue(self):
        self.assertRaises(TypeError, self.this_capsule.take_value, self.b)

    def testRejectNULLpointerByReference(self):
        self.assertRaises(TypeError, self.this_capsule.take_reference, self.b)

    def test_make_valid(self):
        self.assert_(not pyreflex.NULLPointer.make_valid(self.b))
        self.this_capsule.mutate(self.b)
        self.assert_(pyreflex.NULLPointer.make_valid(self.b))
hideSuite(NullPointerPlain)
        

class NullPointerDynamic(NullPointerPlain):

    this_capsule = g.test_null_pointerDynamic
    other_capsule = g.test_null_pointer
    this_type = g.Base

    def testGet(self):
        self.assertRaises(pyreflex.NullPointer, lambda :self.b.i)
        self.this_capsule.mutate(self.b)
        self.assertEqual(self.b.i, 123456789)
hideSuite(NullPointerDynamic)

class PassFunctionPointer(unittest.TestCase):

    def failtestViaPointer(self):
        address = g.test_pass_function.address_of_passme()
        pointer = pyreflex.voidstar(address)
        result = g.test_pass_function.take_function(pointer)
        self.assertEqual(result, 5.0)

    def failtestDirect(self):
        # This is probably too ambitious ... for now
        result = g.test_pass_function.take_function(g.test_pass_function.passme)
        self.assertEqual(result, 5.0)

    def DO_NOT_testViaPyInline(self):
        import sys
        import os
        # Work around scram's ignorance of python-packages. Skip if not SCRAM
        arch = os.getenv('SCRAM_ARCH')
        if arch:
            path = '/afs/cern.ch/sw/lcg/external/python-packages/23_LCG26/' + arch + '/site-packages'
            sys.path.insert(1,path)
            import PyInline
            m = PyInline.build(code = """
            double add(double a, double b) {
            return a + b;
            }
            int address_of_add() {
            return (int)&add;
            }
            """, language = 'C')
            result = g.test_pass_function.take_function(pyreflex.voidstar(m.address_of_add()))
            self.assertEqual(result, 5.0)
        else:
            self.assertEqual(0., 0.)
hideSuite(PassFunctionPointer)

# A function which automates finding and adding the correct wrapped
# operator<< as __repr__ to some class. Call this once on every class
# in which you want to do the mapping.
def make_insertable(klass):
    try:
        thingy = g.insertion_operator_to_repr(klass).__Repr__
    except AttributeError:
        raise Exception, "Can't find an inserter for %s" % klass
    def __repr__(self):
        return thingy(self)
    klass.__repr__ = __repr__

class Insertable(unittest.TestCase):

    def testInsert(self):
        test_message = "something or other"
        make_insertable(g.insertable)
        import sys
        import StringIO
        out = StringIO.StringIO()
        try:
            sys.stdout = out
            print g.insertable(test_message)
        finally:
            sys.stdout = sys.__stdout__
        result = out.getvalue().rstrip()
        self.assertEqual(result, test_message)
hideSuite(Insertable)

class Typedef(unittest.TestCase):

    value = 4
    construct = 9
    cls = g.tdclass
    
    def setUp(self):
        self.a = g.test_typedef(self.construct)


    def testFundamentalOriginal2Typedef(self):
        self.assertEqual(self.a.TDint(self.value), self.value)

    def testFundamentalOriginal2TypedefStar(self):
        self.assertEqual(self.a.TDintstar(self.value), self.value)

    def testFundamentalOriginal2StarTypedef(self):
        self.assertEqual(self.a.starTDint(self.value), self.value)


    def testFundamentalGetTypedef(self):
        self.assertEqual(self.a.ti, self.construct)
        
    def testFundamentalGetStarTypedef(self):
        self.assertEqual(self.a.tip, self.construct)
        
    def testFundamentalGetTypedefStar(self):
        self.assertEqual(self.a.tis, self.construct)
        

    def testFundamentalReturnTypedef(self):
        ret = self.a.returnTDint(self.value)
        self.assertEqual(ret, self.value)
        #self.assertEqual(type(ret), ???)

    def testFundamentalReturnStarTypedef(self):
        ret = self.a.returnStarTDint()
        self.assertEqual(ret, self.construct)

    def testFundamentalReturnTypedefStar(self):
        ret = self.a.returnTDintstar()
        self.assertEqual(ret, self.construct)

    # Should typedefs to fundamentals be accessible on the Python side
    # at all? In the case of classes, particularly template classes,
    # we probably DO want the typedef. One approach would be to
    # install a name in the appropriate namespace, which is merely a
    # synonym for the typedefed class, but this might confuse the user
    # as the type's name will not match its __name__. Maybe a typedef
    # type should be introduced.
    
    #def testFundamentalTypedef2Original(self):
    #    self.assertEqual(self.a.INT(g.TYPEDEFint(self.value)), self.value)
    #
    #def testFundamentalTypedef2Typedef(self):
    #    self.assertEqual(self.a.INT(g.TYPEDEFint(self.value)), self.value)

    def testClassOriginal2Typedef(self):
        self.assertEqual(self.a.TDtdclass(self.cls(self.value)).i, self.value)

    def testClassOriginal2StarTypedef(self):
        self.assertEqual(self.a.starTDtdclass(self.cls(self.value)).i, self.value)

    def testClassOriginal2TypedefStar(self):
        self.assertEqual(self.a.TDtdclassstar(self.cls(self.value)).i, self.value)


    def testClassGetTypedef(self):
        self.assertEqual(self.a.tc.i, self.construct)
        
    def testClassGetStarTypedef(self):
        self.assertEqual(self.a.tcp.i, self.construct)
        
    def testClassGetTypedefStar(self):
        self.assertEqual(self.a.tcs.i, self.construct)
        

    def testClassReturnTypedef(self):
        ret = self.a.returnTDtdclass(self.cls(self.value))
        self.assertEqual(ret.i, self.value) # segfault on access of field of returned typedef
        #self.assertEqual(type(ret), ???)

    def testClassReturnStarTypedef(self):
        ret = self.a.returnStarTDtdclass()
        self.assertEqual(ret.i, self.construct)

    def testClassReturnTypedefStar(self):
        ret = self.a.returnTDtdclassstar()
        self.assertEqual(ret.i, self.construct)


class CException(unittest.TestCase):

    # Very unsophisticated for now, but at least it should avoid nasty aborts.
    def testSimple(self):
        self.assertRaises(Exception, g.test_exception.chuck)


# Remove the "abstract test" from the test suite, so that it does not get run.
del ConverterTest



# Hidden test management.

# Test may be hidden by prepending the test name with "fail" or
# "seg". This should ensure that the test is only run when the 'fail'
# or 'seg' option is passed on the command line. When the tests are
# hidden they should register themselves in the relevant (fail or seg)
# hidden tests collection.

# Inherited tests may be hidden by creating a non-function attribute
# of the appropriate name (failtestWhatever).

import types
def hidetest(case, fullname, prefix, hide):
    test_name = fullname[len(prefix):]
    if hide:
        # create empty test, which adds itself to the hidden test collecton, when it is called
        def hidden_test(self):
            hidetest.hidden[prefix].append(case.__name__+'.'+test_name)
        setattr(case, test_name, hidden_test)
    else:
        # hide is false, so we need to unhide the test ... only
        # necessary if the hidden test is a real test, ie if it's a
        # method.
        hider = getattr(case, fullname)
        if type(hider) == types.MethodType:
            setattr(case, test_name, hider)
# Collections of hidden tests        
hidetest.hidden = {}
hidetest.hidden['fail'] = []
hidetest.hidden['seg'] = []
hidetest.hidden['block'] = []
hidetest.hidden['never'] = []

# Find all test case classes
test_cases = [ c for c in globals().values()
               if isinstance(c, type(unittest.TestCase)) and issubclass(c, unittest.TestCase) ]
# Find all the hidden test indicators and process them
for case in test_cases:
    for name in dir(case):
        prefix = ''
        if name.startswith('nevertest'): prefix, hide = 'never', True
        if name.startswith('blocktest'): prefix, hide = 'block', True
        if name[:8] == 'failtest': prefix, hide = 'fail', not fail
        if name[:7] == 'segtest':  prefix, hide = 'seg',  not seg
        if prefix: hidetest(case, name, prefix, hide)

if __name__ == "__main__":

    # Hack unittest to ensure that it does not print anything to
    # stderr (unless some tests actually failed) This is necessary to
    # satisfy QMtest.

    # The hack consists of replacing the runTests method of the
    # unittest.main object which is responsible for running the
    # tests. We make it override the relevant stream variable.
    def runTests(self):
        if self.testRunner is None:
            # Here we add stream=sys.stdout as an extra parameter (jacek 05/03/2004)
            self.testRunner = unittest.TextTestRunner(stream=sys.stdout, verbosity=self.verbosity)
        global result
        result = self.testRunner.run(self.test)
        sys.exit(not result.wasSuccessful())

    unittest.main.runTests = runTests


    if locateseg:
        # Print out the test's identity as it is being run
        oldcall = unittest.TestCase.__call__
        def __call__(self, result=None):
            __name__ = getattr(self, self._TestCase__testMethodName).__name__
            print "        %s.%s  (%s)" % (self.__class__.__name__, self._TestCase__testMethodName, __name__)
            return oldcall(self, result)
        unittest.TestCase.__call__ = __call__
        
    if bench:
        import timeit
        print "static : ",
        timeit.main(['-s', 'from pyreflex import g,b', 'g.NMF.bench(2, b, 2.5)'])
        print "dynamic: ",
        timeit.main(['-s', 'from pyreflex import g,b; inst=g.NMF.Base(2)', 'inst.bench(2, b, 2.5)'])
        sys.exit()

#python2.4 -m timeit -s "import pyreflex" -s "pyreflex.loadDictionary('testdict')" -s "g=pyreflex.BuildScope()" -s "b = g.NMF.Boring(2)" "g.NMF.bench(2, b, 2.5)"
#python2.4 -m timeit -s "import pyreflex" -s "pyreflex.loadDictionary('testdict')" -s "g=pyreflex.BuildScope()" -s "b = g.NMF.Boring(2)" -s "inst = g.NMF.Base(2)" "inst.bench(2, b, 2.5)"

# Same again in pure Godel (no Reflex)
#python2.4 -m timeit -s "import reflex" -s "inst = reflex.Base(4)" -s "b = reflex.Boring(2)" "inst.bench(2,b,2.4)"
    try:
        unittest.main()
        #import pdb
        #pdb.run('unittest.main()')
    except SystemExit, unittest_exit_code:
        PD = pyreflex
        #Ref = PD.Reflect

        hblock = len(hidetest.hidden['block'])
        hfail  = len(hidetest.hidden['fail'])
        hseg   = len(hidetest.hidden['seg'])
        hnever = len(hidetest.hidden['never'])
        
        print hblock, "hidden blocked tests"
        if names:
            for t in hidetest.hidden['block']: print "  ", t
        print hfail, "hidden failing tests"
        if names:
            for t in hidetest.hidden['fail']: print "  ", t
        print hseg, "hidden segfaulting tests"
        if names:
            for t in hidetest.hidden['seg']: print "  ", t
        print hnever, "hidden tests we don't plan to pass in the foreseeable future"
        totpass = result.testsRun - len(result.failures) - len(result.errors) - hseg - hfail - hblock - hnever
        print "Total passed:", totpass
        print "Pyreflex is, therefore, exactly %.1f%% complete :-)" % (totpass*100.0/float(result.testsRun-hnever))
        if not noexit: raise unittest_exit_code
            
    b = g.NMF.Boring(2)

