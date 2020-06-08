/*

В стандартном потоке дана одна строка, состоящая из числа N и следующих за ним N строк S. 
Между собой число и строки разделены пробелом.

Отсортируйте строки S в лексикографическом порядке по возрастанию, игнорируя регистр букв, 
и выведите их в стандартный поток вывода.

*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <set>

using namespace std;

//

int main(void)
{
	int n = 0;
	string s;
	vector<string> words;
	cin >> n;
	for(int i = 0; i < n; i++)
	{
		cin >> s;
		words.push_back(s);
	}
	sort(words.begin(), words.end(), [](const string & x, const string & y)
	{
		string xx, yy;
		for(const char & c : x)
		{
			xx += tolower(c);
		}
		for(const char & c : y)
		{
			yy += tolower(c);
		}
		if(xx > yy)
		{
			return false;
		}
		return true;;
	});
	for(const string & ss : words)
	{
		cout << ss << " ";
	}
	return 0;
}
