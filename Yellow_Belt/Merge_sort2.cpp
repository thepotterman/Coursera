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
#include <iterator>

using namespace std;

/*

Часть 2. Реализация с разбиением на 3 части
Реализуйте сортировку слиянием, разбивая диапазон на 3 равные части, а не на 2. 
Гарантируется, что длина исходного диапазона является степенью 3.

Соответственно, пункты 3–5 алгоритма нужно заменить следующими:

Разбить вектор на 3 равные части.
Вызвать функцию MergeSort от каждой части вектора.
Слить первые две трети вектора с помощью алгоритма merge, 
сохранив результат во временный вектор с помощью back_inserter.
Слить временный вектор из предыдущего пункта с последней третью вектора из п. 2, 
записав полученный отсортированный диапазон вместо исходного.
Пример кода

int main() {
  vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    cout << x << " ";
  }
  cout << endl;
  return 0;
}

Вывод

0 1 4 4 4 5 6 6 7

*/

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
	if(range_end - range_begin > 1)
	{
		vector<typename RandomIt::value_type> elements(range_begin, range_end);
		vector<typename RandomIt::value_type> var_elements;
		auto it1 = elements.begin();
		auto it2 = elements.begin();
		auto it3 = elements.end();
		auto it4 = elements.end();
		it2 += (it4 - it1) / 3;
		it3 -= (it4 - it1) / 3;
		MergeSort(it1, it2);
		MergeSort(it2, it3);	
		MergeSort(it3, it4);
		merge(it1, it2, it2, it3, back_inserter(var_elements));
		merge(var_elements.begin(), var_elements.end(), it3, it4, range_begin);
	}
}

int main() 
{
	vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
	MergeSort(begin(v), end(v));
	for (int x : v) 
	{
		cout << x << " ";
	}
	cout << endl;
	return 0;
}