#import parse_headers
#parse_headers.parse('testgodel.xml')
#
#import generate_code
#classes = [i for i in parse_headers.all_items.itervalues()
#           if type(i) == parse_headers.GCCXML.Class]
#
#generate_code.generate_module(classes, 'TG')
#generate_code.compile_module('TG')
import generate_code
generate_code.do_everything('TG', 'testgodel.h')



import TG
import unittest

class Class1(unittest.TestCase):

    def testDefaultConstruct(self):
        "Class with a default defalut constructor is correctly instantiated"
        self.assert_(type(TG.Class1()) is TG.Class1)




class Int(unittest.TestCase):

    arg = 4
    arg2 = 5
    TC = TG.test_int

    def setUp(self):
        self.o = self.TC(self.arg)

    def testReturnValue(self):
        self.assert_(self.o.return_v() == self.arg)

    def testReturnPointer(self):
        self.assert_(self.o.return_p() == self.arg)

    def testReturnReference(self):
        self.assert_(self.o.return_r() == self.arg)

    def testPassValue(self):
        self.assert_(self.o.pass_v(self.arg2) == self.arg+self.arg2)


class Double(Int):

    arg = 4.5
    arg2 = 5.6
    TC = TG.test_double



class UserClass(unittest.TestCase):
    
    arg = 4
    arg2 = 5
    TC = TG.test_UserClass

    def setUp(self):
        self.original = TG.UserClass(self.arg)
        self.holder = self.TC(self.original)

    def testReturnValue(self):
        """Return the same instance twice, by value. Modify one value,
        check that the other is not modified."""
        returned_one = self.holder.return_v()
        returned_two = self.holder.return_v()
        self.assert_(returned_one.get() == self.arg)
        self.assert_(returned_one.get() == returned_two.get())
        returned_one.set(self.arg2)
        self.assert_(returned_one.get() == self.arg2)
        self.assert_(returned_one.get() != returned_two.get())
        

    def testReturnPointer(self):
        """Return the same instance twice, by pointer. Modify one value,
        check that the other is not modified."""
        returned_one = self.holder.return_p()
        returned_two = self.holder.return_p()
        self.assert_(returned_one.get() == self.arg)
        self.assert_(returned_one.get() == returned_two.get())
        returned_one.set(self.arg2)
        self.assert_(returned_one.get() == self.arg2)
        self.assert_(returned_one.get() == returned_two.get())


    def testReturnReference(self):
        """Return the same instance twice, by reference. Modify one value,
        check that the other is not modified."""
        returned_one = self.holder.return_r()
        returned_two = self.holder.return_r()
        self.assert_(returned_one.get() == self.arg)
        self.assert_(returned_one.get() == returned_two.get())
        returned_one.set(self.arg2)
        self.assert_(returned_one.get() == self.arg2)
        self.assert_(returned_one.get() == returned_two.get())

    def testPassValue(self):
        """Pass an instance by value, modify it, check that the
        original is unchanged."""
        returned = self.holder.pass_v(self.original)
        self.assert_(returned.get() == self.arg * 2)
        self.assert_(self.original.get() == self.arg)

    def testPassPointer(self):
        """Pass an instance by pointer, modify it, check that the
        original also changes."""
        returned = self.holder.pass_p(self.original)
        self.assert_(returned.get() == self.arg * 2)
        self.assert_(self.original.get() == returned.get())

    def testPassReference(self):
        """Pass an instance by reference, modify it, check that the
        original also changes."""
        returned = self.holder.pass_r(self.original)
        self.assert_(returned.get() == self.arg * 2)
        self.assert_(self.original.get() == returned.get())

        
        


class OperatorEquals(unittest.TestCase):

    # No operator== definition exists, so Godel must implement the
    # comparison function, or trigger the compiler to generate it.
    def FAILtestImplicit(self):
        one = TG.Operator__eq__Implicit(1)
        two = TG.Operator__eq__Implicit(2)
        self.assert_(one == one)
        self.assert_(one != two)

    # A user-defined operator== exists, so Godel must bind it to __eq__
    def testExplicit(self):
        one = TG.Operator__eq__Explicit(1)
        two = TG.Operator__eq__Explicit(2)
        self.assert_(one == one)
        self.assert_(one != two)


unittest.main()
