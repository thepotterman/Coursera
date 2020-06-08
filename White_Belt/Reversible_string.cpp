#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <set>

/* Реализуйте класс ReversibleString, хранящий строку и поддерживающий методы 
Reverse для переворота строки и 
ToString для получения строки. */

using namespace std;

class ReversibleString
{
public:
	ReversibleString() {};
	ReversibleString(const string & string)
	{
		s = string;
	}
	void Reverse() 
	{
		char c;
		for(int i = 0; i < s.size() / 2; i++)
		{
			c = s[i];
			s[i] = s[(s.size() - 1 - i)];
			s[(s.size() - 1 - i)] = c;
		}
	}
	string ToString() const
	{
		string result;
		result = s;
		return result;
	}
private:
	string s;
};

int main(void) 
{
	ReversibleString s("live");
	s.Reverse();
	cout << s.ToString() << endl;
  
	s.Reverse();
	const ReversibleString& s_ref = s;
	string tmp = s_ref.ToString();
	cout << tmp << endl;
  
	ReversibleString empty;
	cout << '"' << empty.ToString() << '"' << endl;
  
	return 0;
}
