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
/*
class Person {
public:
  // Вы можете вставлять сюда различные реализации класса,
  // чтобы проверить, что ваши тесты пропускают корректный код
  // и ловят некорректный
  void ChangeFirstName(int year, const string& first_name) {
  }
  void ChangeLastName(int year, const string& last_name) {
  }
  string GetFullName(int year) {
  }
};
*/
/*
class Person {
public:
	void ChangeFirstName(int year, const string& first_name) 
	{
		year_to_first_name[year] = first_name;
	}
	void ChangeLastName(int year, const string& last_name) 
	{
		year_to_last_name[year] = last_name;
	}
	string GetFullName(int year) 
	{
		string s;
		bool flag1 = false;
		bool flag2 = false;
		int year1 = 0;
		int year2 = 0;
		for(const auto & item : year_to_first_name)
		{
			if(item.first <= year)
			{
				if(year1 <= item.first)
				{
					year1 = item.first;
					flag1 = true;
				}
			}
		}
		for(const auto & item : year_to_last_name)
		{
			if(item.first <= year)
			{
				if(year2 <= item.first)
				{
					year2 = item.first;
					flag2 = true;
				}
			}
		}
		if(flag1 == true)
		{
			s += year_to_first_name[year1];
			if(flag2 == true)
			{
				s += " ";
				s += year_to_last_name[year2];
			}
			else
			{
				s += " with unknown last name";
			}
		}
		else
		{
			if(flag2 == true)
			{
				s += year_to_last_name[year2];
				s += " with unknown first name";
			}
			else
			{
				s += "Incognito";
			}
		}
		return s;
	}
private:
	map<int, string> year_to_first_name;
	map<int, string> year_to_last_name;
};
*/
void SampleTest1()
{
	Person person;
  
  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  // for (int year : {1900, 1965, 1990}) {
  //   cout << person.GetFullName(year) << endl;
  // }
  
  person.ChangeFirstName(1970, "Appolinaria");
  // for (int year : {1969, 1970}) {
  //   cout << person.GetFullName(year) << endl;
  // }
  
  person.ChangeLastName(1968, "Volkova");
  // for (int year : {1969, 1970}) {
  //   cout << person.GetFullName(year) << endl;
  // }
  AssertEqual(person.GetFullName(1969), "Polina Volkova", "DownGrade");
  AssertEqual(person.GetFullName(1900), "Incognito", "IncognitoCheck");
  AssertEqual(person.GetFullName(1965), "Polina with unknown last name", "Unknown last name");
  AssertEqual(person.GetFullName(1967), "Polina Sergeeva", "FirstLastName");
  /*

	Incognito
	Polina with unknown last name
	Polina Sergeeva
	Polina Sergeeva
	Appolinaria Sergeeva
	Polina Volkova
	Appolinaria Volkova

  */
}

void SampleTest2()// last_name with unknown first name
{
  Person person;
  
  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1907, "Sergeeva");
  // for (int year : {1900, 1965, 1990}) {
  //   cout << person.GetFullName(year) << endl;
  // }
  
  person.ChangeFirstName(1970, "Appolinaria");
  // for (int year : {1969, 1970}) {
  //   cout << person.GetFullName(year) << endl;
  // }
  
  person.ChangeLastName(1968, "Volkova");
  // for (int year : {1969, 1970}) {
  //   cout << person.GetFullName(year) << endl;
  // }
  AssertEqual(person.GetFullName(1920), "Sergeeva with unknown first name", "Unknown first name");
  /*

	Incognito
	Polina with unknown last name
	Polina Sergeeva
	Polina Sergeeva
	Appolinaria Sergeeva
	Polina Volkova
	Appolinaria Volkova

  */
}

void TestAll()
{
	TestRunner runner;
	runner.RunTest(SampleTest1, "SampleTest1");
	runner.RunTest(SampleTest2, "SampleTest2");
}

int main() {
  TestAll();
  return 0;
}
