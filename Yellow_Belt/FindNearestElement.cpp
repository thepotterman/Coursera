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

Напишите функцию FindNearestElement, 
для множества целых чисел numbers и данного числа border возвращающую итератор на элемент множества, 
ближайший к border. Если ближайших элементов несколько, верните итератор на наименьший из них.

set<int>::const_iterator FindNearestElement(
    const set<int>& numbers,
    int border);
// set<int>::const_iterator —
// тип итераторов для константного множества целых чисел

*/

set<int>::const_iterator FindNearestElement(
    const set<int>& numbers,
    int border)
{
	auto l = numbers.lower_bound(border);
	auto u = numbers.upper_bound(border);
	if(l == numbers.begin())
	{
		return l;
	}
	if(u == numbers.end() && u == l)
	{
		return prev(l);
	}
	if(border == *(l))
	{
		return l;
	}
	if(border - *(prev(l)) <= *(u) - border)
	{
		return prev(l);
	}
	return u;
}

int main() 
	{
	set<int> numbers = {1, 4, 6};
	cout <<
	  *FindNearestElement(numbers, 0) << " " <<
	  *FindNearestElement(numbers, 3) << " " <<
	  *FindNearestElement(numbers, 5) << " " <<
	  *FindNearestElement(numbers, 6) << " " <<
	  *FindNearestElement(numbers, 100) << endl;
	  
	set<int> empty_set;

	cout << (FindNearestElement(empty_set, 8) == end(empty_set)) << endl;
	return 0;
}

/*

Вывод :

1 4 4 6 6
1

*/