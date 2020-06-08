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

Напишите шаблонную функцию FindGreaterElements, 
принимающую множество elements объектов типа T и ещё один объект border типа T 
и возвращающую вектор из всех элементов множества, бо́льших border, в возрастающем порядке.

*/

template <typename T>
vector<T> FindGreaterElements(const set<T>& elements, const T& border)
{
	vector<T> answer;
	auto iterator = find_if(elements.begin(), elements.end(), [border](T x)
	{
		return x > border;
	});
	for(auto it = iterator; it != elements.end(); it++)
	{
		answer.push_back(*(it));
	}
	return answer;
}

int main() 
{
	for (int x : FindGreaterElements(set<int>{1, 5, 7, 8}, 5)) 
	{
		cout << x << " ";
	}
	cout << endl;

	string to_find = "Python";
	cout << FindGreaterElements(set<string>{"C", "C++"}, to_find).size() << endl;
	return 0;
}