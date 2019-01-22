// *********************************************************************
//            Fundamental types
// *********************************************************************

struct test_int {
  int v;
  test_int(int arg) : v(arg) {}
  int  return_v() { return  v; }
  int* return_p() { return &v; }
  int& return_r() { return  v; }
  int  pass_v(int  a) { return a+v; }
  //int  pass_r(int ai) { return a+v; }
};


struct test_double {
  double v;
  test_double(double arg) : v(arg) {}
  double  return_v() { return  v; }
  double* return_p() { return &v; }
  double& return_r() { return  v; }
  double  pass_v(double  a) { return a+v; }
};

// *********************************************************************
//    str, std::string, char* and all that ...
// *********************************************************************



// *********************************************************************
//             User-defined classes
// *********************************************************************

struct Class1 {
  
};

struct UserClass {
  int i;
  UserClass(int j): i(j) {}
  int get() { return i; }
  void set(int j) { i = j; }
};

struct test_UserClass {
  UserClass v;
  test_UserClass(UserClass arg) : v(arg) {}
  UserClass  return_v() { return  v; }
  UserClass* return_p() { return &v; }
  UserClass& return_r() { return  v; }
  UserClass  pass_v(UserClass  a) { return a.i  += v.i; }
  UserClass  pass_p(UserClass* a) { return a->i += v.i; }
  UserClass  pass_r(UserClass& a) { return a.i  += v.i; }
};


struct Operator__eq__Implicit {
  int i;
  Operator__eq__Implicit(int j): i(j) {}
};
struct Operator__eq__Explicit {
  int i;
  Operator__eq__Explicit(int j): i(j) {}
  bool operator == (const Operator__eq__Explicit other) {
    return this->i == other.i;
  }
};
