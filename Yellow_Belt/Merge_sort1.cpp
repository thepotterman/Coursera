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

Напишите шаблонную функцию MergeSort, принимающую два итератора шаблонного типа RandomIt и 
сортирующую заданный ими диапазон с помощью сортировки слиянием. 

Гарантируется, что:

итераторы типа RandomIt аналогичны по функциональности итераторам вектора и строки, 
то есть их можно сравнивать с помощью операторов <, <=, > и >=, а также вычитать и складывать с числами;
сортируемые объекты можно сравнивать с помощью оператора <.

Часть 1. Реализация с разбиением на 2 части
Алгоритм
Классический алгоритм сортировки слиянием выглядит следующим образом:

Если диапазон содержит меньше 2 элементов, выйти из функции.
Создать вектор, содержащий все элементы текущего диапазона.
Разбить вектор на две равные части. 
(В этой задаче гарантируется, что длина передаваемого диапазона является степенью двойки, 
так что вектор всегда можно разбить на две равные части.)
Вызвать функцию MergeSort от каждой половины вектора.
С помощью алгоритма merge слить отсортированные половины, 
записав полученный отсортированный диапазон вместо исходного.
Вы должны реализовать именно этот алгоритм и никакой другой: 
тестирующая система будет проверять, что вы выполняете с элементами именно эти действия.

Подсказка
Чтобы создать вектор, содержащий все элементы текущего диапазона (п. 2 алгоритма), 
необходимо уметь по типу итератора узнавать тип элементов, на которые он указывает. 
Если итератор RandomIt принадлежит стандартному контейнеру (вектору, строке, множеству, словарю...), 
нижележащий тип можно получить с помощью выражения typename RandomIt::value_type. 
Таким образом, гарантируется, что создать вектор в п. 2 можно следующим образом:

vector<typename RandomIt::value_type> elements(range_begin, range_end);

Пример кода

int main() {
  vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
  MergeSort(begin(v), end(v));
  for (int x : v) {
    cout << x << " ";
  }
  cout << endl;
  return 0;
}

Вывод

0 1 4 4 4 6 6 7 

*/

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end)
{
	if(range_end - range_begin > 1)
	{
		vector<typename RandomIt::value_type> elements(range_begin, range_end);
//		vector<typename RandomIt::value_type> var_elements;
		auto it1 = elements.begin();
		auto it2 = elements.begin();
		auto it3 = elements.end();
		it2 += (it3 - it1) / 2;
		MergeSort(it1, it2);
		MergeSort(it2, it3);
		inplace_merge(it1, it2, it3);
		while(range_begin != range_end)
		{
			*(range_begin) = *(it1);
			range_begin++;
			it1++;
		}
	}
	return;
}

int main() {
	vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
	MergeSort(begin(v), end(v));
	for (int x : v) 
	{
		cout << x << " ";
	}
	cout << endl;
	return 0;
}