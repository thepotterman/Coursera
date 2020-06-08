/*

В стандартном потоке дана одна строка, состоящая из N + 1 целых чисел. 

Первым числом идёт само число N. Далее следуют ещё N чисел, обозначим их за массив A. Между собой числа разделены пробелом.

Отсортируйте массив А по модулю и выведите его в стандартный поток.

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
	int n = 0, el = 0;
	cin >> n;
	vector<int> nums;
	for(int i = 0; i < n; i++)
	{
		cin >> el;
		nums.push_back(el);
	}
	sort(nums.begin(), nums.end(), [](int x, int y)
	{
		if(abs(x) > abs(y))
		{
			return 0;
		}
		return 1;
	});
	for(const int & x : nums)
	{
		cout << x << " ";
	}
	return 0;
}
