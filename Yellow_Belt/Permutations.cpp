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

Дано целое положительное число N, не превышающее 9. 
Выведите все перестановки чисел от 1 до N в обратном лексикографическом порядке (см. пример).

Пример

Ввод

3

Вывод

3 2 1
3 1 2
2 3 1
2 1 3
1 3 2
1 2 3

Подсказка

Библиотека <algorithm> содержит готовые функции, позволяющие решить эту задачу.

*/

void PrintVector(const vector<int> & nums)
{
	for(const int & x : nums)
	{
		cout << x << " ";
		//printf("%d ", x);
	}
	cout << endl;
	//printf("\n");
}

int main(void)
{
	int n = 0;
	cin >> n;
	vector<int> permutation;
	for(int i = n; i > 0; i--)
	{
		permutation.push_back(i);
	}
	PrintVector(permutation);
	while(prev_permutation(permutation.begin(),permutation.end()))
	{
		PrintVector(permutation);
	}
	return 0;
}