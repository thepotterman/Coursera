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

Реализуйте шаблонную функцию GetRefStrict, которая на вход принимает: 
map и значение ключа k. 
Если элемент по ключу k в коллекции отсутствует, 
то функция должна бросить исключение runtime_error, 
иначе вернуть ссылку на элемент в коллекции.


*/

template <typename key, typename value>
value & GetRefStrict(map<key, value> & m, const key & k);

template <typename key, typename value>
value & GetRefStrict(map<key, value> & m, const key & k)
{
	if(m.count(k) == 0)
	{
		throw runtime_error("");
	}
	else
	{
		return m[k];
	}
}

int main() 
{
	map<int, string> m = {{0, "value"}};
	string& item = GetRefStrict(m, 0);
	item = "newvalue";
	cout << m[0] << endl; // выведет newvalue

	return 0;
}

