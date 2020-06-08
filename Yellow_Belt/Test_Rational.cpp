#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

// int gcd (int a, int b)
// {
//   if(a == 0)
//   {
//     return b;
//   }
//   else if(a > b)
//   {
//     a = a % b;
//     return gcd(a, b);
//   }
//   else
//   {
//     b = b % a;
//     return gcd(b, a);
//   }
// }

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    if (!hint.empty()) {
       os << " hint: " << hint;
    }
    throw runtime_error(os.str());
  }
}

void Assert(bool b, const string& hint) {
  AssertEqual(b, true, hint);
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    } catch (exception& e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    } catch (...) {
      ++fail_count;
      cerr << "Unknown exception caught" << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};

// class Rational {
// public:
//     Rational() 
//     {
//         numerator = 0;
//         denominator = 1;
//     }

//     Rational(int new_numerator, int new_denominator) 
//     {
//       int new_gcd = 0;
//       if(new_numerator * new_denominator > 0)
//       {
//         new_numerator = abs(new_numerator);
//         new_denominator = abs(new_denominator);
//       }
//       else
//       {
//         if(new_denominator < 0)
//         {
//           new_denominator *= -1;
//           new_numerator *= -1;
//         }
//       }
//       new_gcd = gcd(abs(new_numerator), abs(new_denominator));
//         numerator = new_numerator / new_gcd;
//         denominator = new_denominator / new_gcd;
//     }

//     int Numerator() const 
//     {
//         return numerator;
//     }

//     int Denominator() const 
//     {
//         return denominator;
//     }

// private:
//     int numerator;
//     int denominator;
// };

bool operator== (const Rational & num1, const Rational & num2)
{
  if(num1.Numerator() == num2.Numerator() &&
    num1.Denominator() == num2.Denominator())
  {
    return true;
  }
  return false;
}

bool operator!= (const Rational & num1, const Rational & num2)
{
  if(num1.Numerator() == num2.Numerator() &&
    num1.Denominator() == num2.Denominator())
  {
    return false;
  }
  return true;
}

bool operator< (const Rational & num1, const Rational & num2)
{
  if(num1.Numerator() * num2.Denominator() < 
    num2.Numerator() * num1.Denominator())
  {
    return true;
  }
  return false;
}

Rational operator+ (const Rational & num1, const Rational & num2)
{
  
  return Rational(
    (num1.Numerator() * num2.Denominator()) + 
    (num2.Numerator() * num1. Denominator()),
    (num1.Denominator() * num2.Denominator()));
}

Rational operator- (const Rational & num1, const Rational & num2)
{
  return Rational(
    (num1.Numerator() * num2.Denominator()) - 
    (num2.Numerator() * num1. Denominator()),
    (num1.Denominator() * num2.Denominator()));
}

Rational operator*(const Rational & num1, const Rational & num2)
{
    return Rational(num1.Numerator() * num2.Numerator(),
                    num1.Denominator() * num2.Denominator());
}

Rational operator/(const Rational & num1, const Rational & num2)
{
    return Rational(num1.Numerator() * num2.Denominator(),
                    num1.Denominator() * num2.Numerator());
}

ostream & operator<<(ostream & stream, const Rational & num)
{
    stream << num.Numerator();
    stream << "/";
    stream << num.Denominator();
    return stream;
}

istream & operator>>(istream & stream, Rational & num)
{
    int n = num.Numerator(), d = num.Denominator();
    stream >> n;
    stream.ignore(1);
    stream >> d;
    num = Rational(n, d);
    return stream;
} 


void Test1()
{
  Rational a;
  Rational b = {0, 1};
  AssertEqual(a, b, "Const w/o params");
}

void Test2()
{
  Rational a = {4, 6};
  Rational b = {2, 3};
  AssertEqual(a, b, "Division both");
}

void Test3()
{
  Rational a = {1, -2};
  Rational b = {-1, 2};
  AssertEqual(a, b, "Unpositive denominator");
}

void Test4()
{
  Rational a = {-2, -3};
  Rational b = {2, 3};
  AssertEqual(a, b, "Positive both");
}

void Test5()
{
  Rational a = {0, 999};
  Rational b = {0, 1};
  AssertEqual(a, b, "0 with non 1 denominator");
}

void TestAll()
{
  TestRunner runner;
  runner.RunTest(Test1, "Test1");
  runner.RunTest(Test2, "Test2");
  runner.RunTest(Test3, "Test3");
  runner.RunTest(Test4, "Test4");
  runner.RunTest(Test5, "Test5");
}

int main() {
  TestAll();
  return 0;
}
