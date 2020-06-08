#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

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
void AssertEqual(const T& t, const U& u, const string& hint = {})
{ 
    if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    os << " hint: " << hint;
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



int GetLinearRealRoot(double b, double c)
{
	if(b == 0)
	{
		return 0;
	}
	return 1;
}

int GetSquareRealRoot(double a, double b, double c)
{
	double D = 0;
	D = b * b - 4 * a * c;
	if(D > 0)
	{
		return 2;
	}
	if(D == 0)
	{
		return 1;
	}
	return 0;
}

int GetDistinctRealRootCount(double a, double b, double c) 
{
	if(a == 0)
	{
		return GetLinearRealRoot(b, c);
	}
	else
	{
		return GetSquareRealRoot(a, b, c);
	}
}

void TestLinear()
{
	AssertEqual(GetDistinctRealRootCount(0, 1, 1), 1, "");
	AssertEqual(GetDistinctRealRootCount(0, 0, 1), 0, "");
}

void TestSquare()
{
	AssertEqual(GetDistinctRealRootCount(1, 1, 1), 0, "");
	AssertEqual(GetDistinctRealRootCount(1, -2, 1), 1, "");
	AssertEqual(GetDistinctRealRootCount(1, 5, 1), 2, "");	
}

void TestAll()
{
	TestRunner runner;
	runner.RunTest(TestLinear, "TestLinear");
	runner.RunTest(TestSquare, "TestSquare");
}

int main() 
{
	TestAll();
	return 0;
}
