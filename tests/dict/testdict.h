// $Id: testdict.h,v 1.21 2005/03/02 16:03:31 jacek Exp $
#include <iostream>
#include <string>
#include <vector>

#define CONVERTER_TEST(AUX_CLASS_NAME, AUX_TYPE) struct AUX_CLASS_NAME {\
  AUX_TYPE a;\
  AUX_TYPE* ap;\
  AUX_CLASS_NAME(AUX_TYPE b): a(b), ap(&a) {}\
  AUX_TYPE   get()                 { return  a; }\
  AUX_TYPE&  getreference()        { return  a; }\
  AUX_TYPE*  getpointer()          { return ap; }\
  AUX_TYPE*& getpointerreference() { return ap; }\
  void set(AUX_TYPE b) { a = b; }\
  AUX_TYPE take                (AUX_TYPE   b) { return  b; }\
  AUX_TYPE takereference       (AUX_TYPE&  b) { return  b; }\
  AUX_TYPE takepointer         (AUX_TYPE*  b) { return *b; }\
  AUX_TYPE takepointerreference(AUX_TYPE*& b) { return *b; }\
};

CONVERTER_TEST(test_int, int)
CONVERTER_TEST(test_short, short)
CONVERTER_TEST(test_short_int, short int)
CONVERTER_TEST(test_unsigned_int, unsigned int)
CONVERTER_TEST(test_float, float)
CONVERTER_TEST(test_double, double)
CONVERTER_TEST(test_signed_char, signed char)
CONVERTER_TEST(test_unsigned_char, unsigned char)
CONVERTER_TEST(test_unsigned_short, unsigned short)
CONVERTER_TEST(test_bool, bool)
CONVERTER_TEST(test_long, long)
CONVERTER_TEST(test_long_int, long int)

struct test_char {
  char* ap;
  char a;
  test_char(char* b) {
    ap = new char[100];
    strcpy(ap, b);
    a = ap[0];
  }
  char   get()                 { return  a; }
  char&  getreference()        { return  a; }
  char*  getpointer()          { return ap; }
  char*& getpointerreference() { return ap; }
  void set(char b) { a = b; }
  char  take                (char   b) { return  b; }
  char  takereference       (char&  b) { return  b; }
  // Returning char*s here, so that we see whether the whole string
  // was passed in, or just the first char.
  char* takepointer         (char*  b) { return  b; }
  char* takepointerreference(char*& b) { return  b; }
};

struct test_std_string {
  std::string a;
  std::string* ap;
  test_std_string(std::string b): a(b), ap(&a) {}
  std::string   get()                 { return  a; }
  std::string&  getreference()        { return  a; }
  std::string*  getpointer()          { return ap; }
  std::string*& getpointerreference() { return ap; }
  void set(std::string b) { a = b; }
  std::string take                (std::string  b)  { return  b; }
  std::string takereference       (std::string&  b) { return  b; }
  std::string takepointer         (std::string*  b) { return *b; }
  std::string takepointerreference(std::string*& b) { return *b; }
};

struct user_defined {
  int i;
  user_defined(int j):i(j) {}
};

struct test_user_defined {
  user_defined a;
  user_defined* ap;
  test_user_defined(user_defined b): a(b), ap(&a) {}
  user_defined   get()                 { return  a; }
  user_defined&  getreference()        { return  a; }
  user_defined*  getpointer()          { return ap; }
  user_defined*& getpointerreference() { return ap; }
  void set(user_defined b) { a = b; }
  user_defined take                (user_defined   b) { return  b; }
  user_defined takereference       (user_defined&  b) { return  b; }
  user_defined takepointer         (user_defined*  b) { return *b; }
  user_defined takepointerreference(user_defined*& b) { return *b; }
};

typedef user_defined TYPEDEF_user_defined;

struct test_TYPEDEF_user_defined {
  TYPEDEF_user_defined a;
  TYPEDEF_user_defined* ap;
  test_TYPEDEF_user_defined(TYPEDEF_user_defined b): a(b), ap(&a) {}
  TYPEDEF_user_defined   get()                 { return  a; }
  TYPEDEF_user_defined&  getreference()        { return  a; }
  TYPEDEF_user_defined*  getpointer()          { return ap; }
  TYPEDEF_user_defined*& getpointerreference() { return ap; }
  void set(TYPEDEF_user_defined b) { a = b; }
  TYPEDEF_user_defined take                (TYPEDEF_user_defined   b) { return  b; }
  TYPEDEF_user_defined takereference       (TYPEDEF_user_defined&  b) { return  b; }
  TYPEDEF_user_defined takepointer         (TYPEDEF_user_defined*  b) { return *b; }
  TYPEDEF_user_defined takepointerreference(TYPEDEF_user_defined*& b) { return *b; }
};



// This looks to be testing for an implementation-specific problem in
// PyLCGDict ... remove when confirmed.
//
//struct Identity {
//  int i;
//  int ints(int a, int b, int c, int d) {return a+b+c+d+i;};
//  int intsPtr(int*, int*, int*, int*) { return 0; };
//  void classes(user_defined, user_defined, user_defined, user_defined) {};
//  void classesPtr(user_defined*, user_defined*, user_defined*, user_defined*) {};
//  void charPtr(char*, char*, char*, char*) {};
//};

struct self_referential_field {
  self_referential_field* self;
};


struct test_pointerclass {};

struct Abstract {
  virtual int foo() = 0;
};
//
//struct concrete : public pABC {
//  int foo() { return 12; }
//};
//
//struct test_pABC {
//  pABC* it;
//};
//
//
//struct concreteXXXXX {
//  int foo() { return 12; }
//};
//
//struct test_pABCXXXXX {
//  concreteXXXXX it;
//  concreteXXXXX* itp;
//};

std::string global_function() {
  return std::string("global");
}

struct test_destructor {
  static int c;
  test_destructor() { c++; }
  test_destructor(const test_destructor& t) { c++; }
  ~test_destructor() { --c; }
  static int count() { return c; }
  static test_destructor  getByValue()     { return       test_destructor(); }
  static test_destructor* getByPointer()   { return   new test_destructor ; }
  static test_destructor& getByReference() { return *(new test_destructor); }
};
int test_destructor::c = 0;

struct Number  {
  Number() : N(0) {}
  Number(int i) : N(i) {}
  ~Number() {}
  int number() { return N; }
  operator int() const {return N;}
  Number  operator  +  (const Number& n) const { return Number(N  + n.N);} 
  Number  operator  -  (const Number& n) const { return Number(N  - n.N);} 
  Number  operator  *  (const Number& n) const { return Number(N  * n.N);} 
  Number  operator  /  (const Number& n) const { return Number(N  / n.N);} 
  Number  operator  %  (const Number& n) const { return Number(N  % n.N);} 
  Number& operator +=  (const Number& n) { N += n.N; return *this;} 
  Number& operator -=  (const Number& n) { N -= n.N; return *this;} 
  Number& operator *=  (const Number& n) { N *= n.N; return *this;} 
  Number& operator /=  (const Number& n) { N /= n.N; return *this;}
  Number& operator %=  (const Number& n) { N %= n.N; return *this;}
  bool    operator  <  (const Number& n) const { return N < n.N;}
  bool    operator  >  (const Number& n) const { return N > n.N;}
  bool    operator <=  (const Number& n) const { return N <= n.N;}
  bool    operator >=  (const Number& n) const { return N >= n.N;}
  bool    operator !=  (const Number& n) const { return N != n.N;}
  bool    operator ==  (const Number& n) const { return N == n.N;}
  Number  operator  &  (const Number& n) const { return Number(N & n.N);} 
  Number  operator  |  (const Number& n) const { return Number(N | n.N);} 
  Number  operator  ^  (const Number& n) const { return Number(N ^ n.N);} 
  Number& operator &=  (const Number& n) { N &= n.N; return *this;} 
  Number& operator |=  (const Number& n) { N |= n.N; return *this;} 
  Number& operator ^=  (const Number& n) { N ^= n.N; return *this;} 
  Number  operator <<  (int i) const { return Number(N << i);} 
  Number  operator >>  (int i) const { return Number(N >> i);} 
  //Number& operator >>= (int i) { N >>= i; return *this;}
  //Number& operator <<= (int i) { N <<= i; return *this;}
  int N;
};



struct Basic {
  int i;
  float f;
  double d;
  int* ip;
  char c;
  std::string s;
  Basic():      i(1),  f(2.0), d(3.1), ip(&i), c('c'), s("the string") {}
  Basic(int ii):i(ii), f(2.0), d(3.1), ip(&i), c('c'), s("the string") {}
  int overload() { return 0; }
  int overload(int) { return 1; }
  int overload(double) { return 2; }
  int overload(int, int) { return 3; }
  // Same again, reversing the order, to make sure that we weren't just lucky.
  int overload2(int, int) { return 3; }
  int overload2(double) { return 2; }
  int overload2(int) { return 1; }
  int overload2() { return 0; }

  int optional(int i=1, int j=2, int k=3) { return i + j + k; } // Improve this, once we can return collections
  static int staticmethod(int i) { return i + 6; }
};

struct class_without_assignment_op {

};

struct class_with_assignment_op {
  int i;
  class_with_assignment_op& operator = (const class_with_assignment_op&) {
    return *(new class_with_assignment_op);
  }
};

struct test_set_field {
  int i;
};




struct Outer {
  struct Inner {
    double d;
    Inner(double dd): d(dd) {}
    double getd() { return d; }
  };
  double d;
  Inner i;
  Outer(Inner ii, double dd) : d(dd), i(ii) {}
  Inner getI() { return i; }
  double getd() { return d; }
};

struct PurelyAbstract {
  virtual std::string foo() = 0;
  virtual ~PurelyAbstract() {}
};

struct PADerived : public PurelyAbstract {
  std::string foo() { return "PADerived"; }
};

int global_function(int i) { return i+7; }

struct Base {
  int i;
  Base(int j=123456789) : i(j) {}
  int geti() { return i; }
  virtual ~Base(){}
};

struct Derived : public Base {
  Derived() : Base(123456789) , Df(6666) {}
  Derived(int j) : Base(j) , Df(6666) {}
  Base* baseDp() {
    return new Derived();
  }
  Base& baseDr() {
    return *(new Derived());
  }
  int Df;
};

struct DerivedVirtually : public virtual Base {
  DerivedVirtually() : Base(123456789), DVf(5555) {}
  DerivedVirtually(int j) : Base(j), DVf(5555) {}
  Base* baseDp() {
    return new DerivedVirtually();
  }
  Base& baseDr() {
    return *(new DerivedVirtually());
  }
  int DVf;
};

struct DerivedVirtually2 : public virtual Base { 
  int DV2f;
  DerivedVirtually2() : Base(987654321), DV2f(7777) {}
  DerivedVirtually2(int j) : Base(j), DV2f(7777) {}
};
struct Diamond : public virtual DerivedVirtually,
		 public virtual DerivedVirtually2 {
  Diamond(int j) : Base(j) {}
  Diamond():Base(123456789) {}
};

struct test_pass_Base {
  int takepointerm  (Base* b) { return b->geti(); }
  int takereferencem(Base& b) { return b.geti(); }
  int takevaluem    (Base  b) { return b.geti(); }
  int takepointerf  (Base* b) { return b->i; }
  int takereferencef(Base& b) { return b.i; }
  int takevaluef    (Base  b) { return b.i; }
};

struct Base1 {};
struct DerivedVirtually1 : public virtual Base1 {};
struct test_pass_Base1 {
  void takevalue( Base1 ) {}
};




struct VBase {
  int i;
  VBase(int j=123456789) : i(j) {}
  virtual int geti() { return i; }
  virtual ~VBase() {}
};

struct VDerived : public VBase {
  VBase* baseDp() {
    return new VDerived();
  }
  VBase& baseDr() {
    return *(new VDerived());
  }
};

struct VDerivedVirtually : public virtual VBase {
  VDerivedVirtually(int j=123456789) : VBase(j) {}
  VBase* baseDp() {
    return new VDerivedVirtually();
  }
  VBase& baseDr() {
    return *(new VDerivedVirtually());
  }
};

struct test_pass_VBase {
  int takepointerm  (VBase* b) { return b->geti(); }
  int takereferencem(VBase& b) { return b.geti(); }
  int takevaluem    (VBase  b) { return b.geti(); }
  int takepointerf  (VBase* b) { return b->i; }
  int takereferencef(VBase& b) { return b.i; }
  int takevaluef    (VBase  b) { return b.i; }
};


struct test_PassSemantics {
  int value;
  test_PassSemantics(int i): value(i) {}
  void pass_v(user_defined  orig){ orig.i  = value; }
  void pass_p(user_defined* orig){ orig->i = value; }
  void pass_r(user_defined& orig){ orig.i  = value; }
};


namespace foo {
  namespace bar {
    struct baz {
      int i;
      baz(int j): i(j) {}
    };
  }
}


// Template testing
#include <complex>
namespace T {
  // Class templates
  template<class T> struct foo {
    T it;
    foo(T _) : it(_) {}
    T get() { return it; }
  };
  foo<int> fi(1);
  foo<float> ff(1);
  foo<double> fd(1);
  foo<Basic> fB(Basic(1));
  //foo<int*> fip;
  //foo<Basic*> fBp;
  foo<std::complex<double> > fcd(std::complex<double>(1,1));
  foo<foo<int> > ffi(7);
  template<class T, class U> struct bar {
    T t;
    U u;
    bar(T tt, U uu) : t(tt), u(uu) {}
    bar(): t(), u() {}
  };
  bar<int, int> bii;
  bar<std::complex<double>, std::complex<double> > bcdcd;

  // Non-member function templates
  template<class T>
    T abs(T n) {
    if (n<0) { return -n; }
    else { return n; }
  }

  template<class RETURN, class OTHER>
    RETURN max(RETURN a, OTHER b) {
    if (a < b) { return b; }
    else { return a; }
  }

  template<class T>
    struct aux {
      T it;
      aux(T i): it(i) {}
      bool operator < (const aux& other) { return this->it < other.it; }
      bool operator < (const int i) { return this->it < i; }
      T operator- () { return T(- this->it); }
    };

  template int abs(int);
  template double abs(double);
  template T::aux<int> abs(T::aux<int>);
  template T::aux<T::aux<int> > abs(T::aux<T::aux<int> >);
  template int max(int, int);
  template double max(double, double);

  struct S {
    // Member function templates
    template<class T>
    T abs(T n) {
      if (n<0) { return -n; }
      else { return n; }
    }
    
    template<class RETURN, class OTHER>
      RETURN max(RETURN a, OTHER b) {
      if (a < b) { return b; }
      else { return a; }
    }
  };
  template int S::abs(int);
  template double S::abs(double);
  template ::T::aux<int> S::abs(::T::aux<int>);
  template T::aux<T::aux<int> > S::abs(T::aux<T::aux<int> >);
  template int S::max(int, int);
  template double S::max(double, double);
}

struct dynamicTypeBase { 
  int datum;
  dynamicTypeBase() : datum(0) {}
  dynamicTypeBase(int i):datum(i) {}
  int report() {
    return datum;
  }
  int adjustReport(int i) {
    return ((dynamicTypeBase*)((int)this+i))->report();
  }
  dynamicTypeBase* returnNullPointer() { return NULL; }
  virtual ~dynamicTypeBase(){}
};

struct NotInDictionary : public dynamicTypeBase {};

struct dynamicTypeDerived : public dynamicTypeBase {
  dynamicTypeDerived(): dynamicTypeBase() {}
  dynamicTypeDerived(int i): dynamicTypeBase(i) {}
  dynamicTypeBase* returnBase(int i) {
    dynamicTypeBase* it = new dynamicTypeBase(i);
    return it;
  }
  dynamicTypeBase* returnDerived(int i) {
    dynamicTypeDerived* it = new dynamicTypeDerived(i);
    return it;
  }
  dynamicTypeBase* returnNotInDictionary() { return new NotInDictionary; }
  int addedMethod() {
    return datum+1;
  }
};

struct dynamicTypeDerivedWithoutDefaultConstructor : public dynamicTypeBase {
  dynamicTypeDerivedWithoutDefaultConstructor(int i): dynamicTypeBase(i) {}
  dynamicTypeBase* returnBase(int i) {
    dynamicTypeBase* it = new dynamicTypeBase(i);
    return it;
  }
  dynamicTypeBase* returnDerived(int i) {
    dynamicTypeDerivedWithoutDefaultConstructor* it = new dynamicTypeDerivedWithoutDefaultConstructor(i);
    return it;
  }
  int addedMethod() {
    return datum+1;
  }
};

struct dynamicTypeDerivedVirtually : virtual public dynamicTypeBase {
  dynamicTypeDerivedVirtually(): dynamicTypeBase() {}
  dynamicTypeDerivedVirtually(int i): dynamicTypeBase(i) {}
  dynamicTypeBase* returnBase(int i) {
    dynamicTypeBase* it = new dynamicTypeBase(i);
    return it;
  }
  dynamicTypeBase* returnDerivedVirtually(int i) {
    dynamicTypeDerivedVirtually* it = new dynamicTypeDerivedVirtually(i);
    return it;
  }
  int addedMethodVirtually() {
    return datum+1;
  }
  std::vector<dynamicTypeBase*> returnVectorOfPointers() {
    std::vector<dynamicTypeBase*> v;
    v.push_back(new dynamicTypeBase(0));
    v.push_back(new dynamicTypeDerived(11));
    v.push_back(new dynamicTypeDerivedVirtually(12));
    return v;
  }
};



//struct Base0 {};
//
//struct IA : public virtual Base0 { int A;};
//struct IB : public virtual Base0 { int B; };
//
//struct Diamond : public virtual IA, public virtual IB {};











//enum Answer { no=0, yes, maybe };
//
//class MyClass {
//private:
//  class InnerClass {
//  };
//  static int s_instances;
//public:
//  MyClass();
//  MyClass(const MyClass& );
//  ~MyClass();
//  MyClass& operator=(MyClass&);
//  int doSomething(const std::string& );
//  int magic() { return m_magic; }
//  void setMagic(int v) { m_magic = v; }
//  static int instances() { return s_instances; }
//  Answer answer() { return m_answer; }
//  void setAnswer(const Answer& a) { m_answer = a; } 
//private:
//  Answer m_answer;
//  int m_magic;
//};
//
//int MyClass::s_instances = 0;
//
//MyClass::MyClass() : m_magic( 987654321 ){
//  s_instances++;
//}
//MyClass::MyClass(const MyClass& c) {
//  m_magic = c.m_magic;
//  s_instances++;
//}
//MyClass::~MyClass() {
//  s_instances--;
//}
//MyClass& MyClass::operator=(MyClass& c) {
//  m_magic = c.m_magic;
//  return *this;
//}
//
//int MyClass::doSomething(const std::string& something) {
//  return something.size();
//}
//class Virtual {
//public:
//  virtual double vf() = 0;
//};
//
//class IntermediateA : public virtual MyClass, public virtual Virtual {
//  virtual double vf()  { return 999.999; }
//private:
//  int m_intA;
//};
//class IntermediateB : public virtual MyClass  {
//private:
//  int m_intB;
//};
//
//class Diamond : public virtual IntermediateA, public virtual IntermediateB {
//};


struct _Instantiations {
  typedef std::vector<float> Vf;
  typedef std::vector<int> Vi;
  Vf m_vf;
  Vi m_vi;
};

struct Concrete {};
struct AbstractInheritsConcrete : public Concrete {
  virtual void foo() = 0;
};

struct test_static_methods {
  int m_i;
  test_static_methods(int i):m_i(i){}

  static int noargs() { return 19; }
  static int onearg(int i) { return i*i; }
  static int twoarg(int i, int j) { return i+j; }
  static int overloaded() { return 19; }
  static int overloaded(int i) { return i*i; }
  static int overloaded(int i, int j) { return i+j; }

  int evil() { return m_i; }
  static int evil(int i) { return i*i+666; }
  static int more_evil() { return 668; }
  int more_evil(int i) { return i*m_i+668; }

};

struct test_templated_methods {
  int foo(int i) { return i+1; }
  template<class T>
  T foo(T ) { return T(); }
};

//template double test_templated_methods::foo(double);
//template int    test_templated_methods::foo(int);

struct test_output_parameter {
  void redirect(Basic*& b) {
    b = new Basic(2);
  }
  void mutate(Basic*& b) {
    b->i = 3;
  }
};

struct test_const {
  int overload(test_const) {
    return 1;
  }
  int overload(test_const) const {
    return 2;
  }
  int foo(test_const*) {
    return 3;
  }
  int foo(const test_const*) {
    return 4;
  }
  int bar(test_const*) {
    return 3;
  }
  int baz(const test_const*) {
    return 4;
  }
};

struct schiz {
  schiz(char*) {}
  void foo(char*) {}
};



struct test_upcast_extractor {
  struct Base {
    virtual ~Base() {}
  };
  struct Derived : public Base {
    int i;
    Derived(int ii): i(ii) {}
  };
  static int by_value(Base) {
    return 12; // Should check for slicing, somehow
  }
  static int by_pointer(Base* b) {
    Derived* d = dynamic_cast<Derived*>(b);
    if (!d) { 
      return 0;
    }
    return d->i;
  }
  static int by_reference(Base& b) {
    try {
      Derived& d = dynamic_cast<Derived&>(b);
      return d.i;
    } catch (...) {
      return 0;
    }
  }
};


namespace namespace_one {
  class the_class {};
}
namespace namespace_two {
  class the_class {};
}

struct test_null_pointer {
  static void mutate(Basic*& b) {
    b = new Basic;
  }
  static Basic* get_invalid() {
    return 0;
  }
  static int report_pointer(Basic* b) {
    return (int)b;
  }
  static void take_value(Basic ) {}
  static void take_reference(Basic& ) {}
};


struct test_null_pointerDynamic {
  static void mutate(Base*& b) {
    b = new Base;
  }
  static Base* get_invalid() {
    return 0;
  }
  static int report_pointer(Base* b) {
    return (int)b;
  }
  static void take_value(Base ) {}
  static void take_reference(Base& ) {}
};


struct test_pass_function {
  static double passme(double a, double b) {
    return a + b;
  }
  static int address_of_passme() {
    return (int)&passme;
  }
  static double take_function(double (*fn)(double, double)) {
    return fn(2,3);
  }
};



#include <iostream>

// The class with an insertion operator
struct insertable {
  std::string text;
  insertable(std::string t): text(t) {}
};
// The insertion operator for the above
std::ostream& operator << (std::ostream& out, insertable& i) {
  return out << i.text;
}

// A template class which overcomes 2 problems:
//
// 1) Removes the stream from the interface, replacing it with an output 
//    string
//
// 2) Provides as static member function to work around the dictionary's
//    ignorance of global functions
#include <sstream>
template<class INSERTABLE>
struct insertion_operator_to_repr {
  static std::string __Repr__(INSERTABLE& i) {
    std::ostringstream string;
    string << i;
    return string.str();
  }
};

// Instantiate the template with the appropriate argument, to make it known
// in the dictionary. Do this once for every class in which you want to map
//  operator<< to Python's __repr__
insertion_operator_to_repr<insertable> a;

namespace NMF {
  int add(int a) {
    std::cout << "int" << std::endl;
    return a+a;
  }

  int add(int a, int b) { 
    std::cout << "int, int" << std::endl;
    return a+b;
  }
  double add(int a, double b) { 
    std::cout << "int, double" << std::endl;
    return a+b;
  }
  double add(double a, int b) { 
    std::cout << "double, int" << std::endl;
    return a+b;
  }
  double add(double a, double b) { 
    std::cout << "double, double" << std::endl;
    return a+b;
  }
  
  typedef int foo;
  std::string string(std::string s) {
    return s+"_suffix";
  }
  char* charstar(char* arg) {
    return arg;
  }
  int string(foo s) {
    return s;
  }

  struct Boring {
    int i;
    Boring(int n) :i(n) {
    }
    int geti() { return this->i; }
    static std::string Static(char* s) {
      return std::string(s);
    }
  };
  Boring make_boring_v(int n) {
    return Boring(n);
  }
  Boring& make_boring_r(int n) {
    return *(new Boring(n));
  }
  Boring* make_boring_p(int n) {
    return new Boring(n);
  }
  Boring* make_boring_p(int n, int m) {
    return new Boring(n+m);
  }
  int i_of_boring_v(Boring* b) {
    return b->i;
  }
  int* i_of_boring_p(Boring* b) {
    return &(b->i);
  }
  int& i_of_boring_r(Boring* b) {
    return b->i;
  }

  void very_boring() {
    std::cout << "This method does nothing, and returns nothing." << std::endl;
  }
  int take_int_v(int i) {
    return 2 * i;
  }
  int take_int_p(int* i) {
    return 2 * *i;
  }
  int take_int_r(int& i) {
    return 2 * i;
  }

  int take_boring_v(Boring b) {
    return b.i;
  }
  int take_boring_p(Boring* b) {
    return b->i;
  }
  int take_boring_r(Boring& b) {
    return b.i;
  }

  struct Base {
    int i;
    Base(int j): i(j) {}
    virtual ~Base() {}
    void bench(int , Boring* , double ) {}
  };
  struct Derived : virtual public Base {
    Derived(int j): Base(j) {}
  };

  int take_base_p(Base* b) {
    return b->i;
  }

  Base* make_derived(int i) {
    return new Derived(i);
  }

  void bench(int , Boring* , double ) {}

};


struct VectorInt {
  std::vector<int> v;
  int operator[](int i) { return v[i]; }
  int size() { return v.size(); }
  void push_back(int i) { v.push_back(i); }
};


typedef int  TYPEDEFint;
typedef int* TYPEDEFintstar;

struct tdclass {
  int i;
  tdclass(int j): i(j) {}
};

typedef tdclass  TYPEDEFtdclass;
typedef tdclass* TYPEDEFtdclassstar;

struct test_typedef {
  TYPEDEFint ti;
  TYPEDEFint* tip;
  TYPEDEFintstar tis;

  TYPEDEFtdclass tc;
  TYPEDEFtdclass* tcp;
  TYPEDEFtdclassstar tcs;

  test_typedef(int j): ti(j), tip(&ti), tis(&ti), tc(j), tcp(&tc), tcs(&tc) {}

  int TDint(TYPEDEFint i) { return i; }
  int TDintstar(TYPEDEFintstar i) { return *i; }
  int starTDint(TYPEDEFint* i) { return *i; }

  TYPEDEFint returnTDint(int i) { return i; }
  TYPEDEFint* returnStarTDint() { return this->tip; }
  TYPEDEFintstar returnTDintstar() { return this->tis; }

  tdclass TDtdclass(TYPEDEFtdclass i) { return i; }
  tdclass TDtdclassstar(TYPEDEFtdclassstar i) { return *i; }
  tdclass starTDtdclass(TYPEDEFtdclass* i) { return *i; }

  TYPEDEFtdclass returnTDtdclass(tdclass i) { return i; }
  TYPEDEFtdclass* returnStarTDtdclass() { return this->tcp; }
  TYPEDEFtdclassstar returnTDtdclassstar() { return this->tcs; }

};

struct test_exception {
  static void chuck() {
    throw 2;
  }
};

namespace upcast_through_not_in_dict {
  struct Base { 
    int i;
    Base(int j): i(j) {}
    int doit(Base* b) { return b->i; }
    virtual ~Base() {} 
  };
  struct NotThere : public Base {
    NotThere(int j): Base(j) {}
  };
  struct There : public NotThere {
    There(int j): NotThere(j) {}
  };
  
}


// This is just a test of the underlying reflection system's ability
// to generate a correct dicitionary for these operators. No cliend
// code exists in test_pyreflex.py
struct test_operator_new {
  operator int () {
    return 3;
  }
  void* operator new(size_t s) {
    return malloc(s);
  };
};
