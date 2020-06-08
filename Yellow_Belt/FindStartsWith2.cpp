#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <set>
#include <fstream>
#include <iomanip>
#include <exception>
#include <tuple>

using namespace std;

/*

Часть 2. Группировка по префиксу
Напишите более универсальный вариант функции FindStartsWith, 
принимающий в качестве префикса произвольную строку, состоящую из строчных латинских букв.

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(
    RandomIt range_begin, RandomIt range_end,
    const string& prefix);

*/

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(
    RandomIt range_begin, RandomIt range_end,
    const string& prefix)
{
	string s = prefix;
	char c;
//	cout << s << " -> ";
	auto min = range_begin;
	auto max = range_begin;
	min = lower_bound(range_begin, range_end, prefix);
	c = s[s.size() - 1];
	c++;
	s.pop_back();
	s.push_back(c);
//	cout << s << "\n\n";
	max = lower_bound(range_begin, range_end, s);
	return {min, max};
}

int main() 
{
	const vector<string> sorted_strings = {"moscow", "motovilikha", "murmansk"};

	const auto mo_result =
		FindStartsWith(begin(sorted_strings), end(sorted_strings), "mo");
	for (auto it = mo_result.first; it != mo_result.second; ++it) 
	{
		cout << *it << " ";
	}
	cout << endl;

	const auto mt_result =
		FindStartsWith(begin(sorted_strings), end(sorted_strings), "mt");
	cout << (mt_result.first - begin(sorted_strings)) << " " <<
		(mt_result.second - begin(sorted_strings)) << endl;

	const auto na_result =
		FindStartsWith(begin(sorted_strings), end(sorted_strings), "na");
	cout << (na_result.first - begin(sorted_strings)) << " " <<
		(na_result.second - begin(sorted_strings)) << endl;

	return 0;
}


/*

Вывод:

moscow motovilikha
2 2
3 3

*/