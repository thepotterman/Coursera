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

Реализуйте шаблонную функцию Sqr, 
которая работает не только для чисел, 
но и для контейнеров. 
Функция должна возвращать копию исходного контейнера, 
модифицировав его следующим образом:

для vector элементы нужно возвести в квадрат;

для map в квадрат нужно возвести только значения, но не ключи;

для pair в квадрат нужно возвести каждый элемент пары.

Функция должна корректно работать не только для контейнеров, состоящих из чисел, 
но и для составных объектов, например, векторов словарей пар чисел.

*/

template <typename T>
T Sqr(T x);

template <typename T>
vector<T> operator * (const vector<T> & lhs, const vector<T> & rhs);

template <typename Key, typename Value>
map<Key, Value> operator * (const map<Key, Value> & lhs, const map<Key, Value> & rhs);

template <typename Key, typename Value>
pair<Key, Value> operator * (const pair<Key, Value> & lhs, const pair<Key, Value> & rhs);

template <typename T>
T Sqr(T x)
{
	return x * x;
}

template <typename T>
vector<T> operator * (const vector<T> & lhs, const vector<T> & rhs)
{
	vector<T> answer;
	for(size_t i = 0; i < lhs.size(); i++)
	{
		answer.push_back(lhs[i] * rhs[i]);
	}
	return answer;
}

template <typename Key, typename Value>
map<Key, Value> operator * (const map<Key, Value> & lhs, const map<Key, Value> & rhs)
{
	map<Key, Value> answer;
	for(const pair<Key, Value>& x : lhs)
	{
		answer[x.first] = lhs.at(x.first) * rhs.at(x.first);
	}
	return answer;
}

template <typename Key, typename Value>
pair<Key, Value> operator * (const pair<Key, Value> & lhs, const pair<Key, Value> & rhs)
{
	return {lhs.first * rhs.first, lhs.second * rhs.second};
}

int main() 
{
	vector<int> v = {1, 2, 3};
	cout << "vector:";
	for (int x : Sqr(v)) {
	  cout << ' ' << x;
	}
	cout << endl;

	map<int, pair<int, int>> map_of_pairs = {
	  {4, {2, 2}},
	  {7, {4, 3}}
	};
	cout << "map of pairs:" << endl;
	for (const auto& x : Sqr(map_of_pairs)) {
	  cout << x.first << ' ' << x.second.first << ' ' << x.second.second << endl;
	}
	return 0;
}