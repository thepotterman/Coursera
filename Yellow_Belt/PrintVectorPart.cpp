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

Напишите функцию PrintVectorPart, принимающую вектор целых чисел numbers, 
выполняющую поиск первого отрицательного числа в нём и выводящую в стандартный вывод все числа, 
расположенные левее найденного, в обратном порядке. Если вектор не содержит отрицательных чисел, 
выведите все числа в обратном порядке.

*/

void PrintVectorPart(const vector<int>& numbers)
{
	auto it = find_if(numbers.begin(), numbers.end(), [](int x)
	{
		if(x < 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	});
	while(it != numbers.begin())
	{
		it--;
		cout << *it << " ";
	}
	cout << endl;
}

int main() {
  PrintVectorPart({6, 1, 8, -5, 4});
  cout << endl;
  PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
  cout << endl;
  PrintVectorPart({6, 1, 8, 5, 4});
  cout << endl;
  return 0;
}
