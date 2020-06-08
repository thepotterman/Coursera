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

Напишите функцию SplitIntoWords, разбивающую строку на слова по пробелам.

Гарантируется, что:

строка непуста;
строка состоит лишь из латинских букв и пробелов;
первый и последний символы строки не являются пробелами;
строка не содержит двух пробелов подряд.

Подсказка

Рекомендуется следующий способ решения задачи:
искать очередной пробел с помощью алгоритма find;
создавать очередное слово с помощью конструктора строки по двум итераторам.

*/

vector<string> SplitIntoWords(const string& s)
{
	vector<string> answer;
	string x;
	stringstream ss(s);
	while(ss >> x)
	{
		answer.push_back(x);
	}
	return answer;
}

int main() 
{
	string s = "C Cpp Java Python";

	vector<string> words = SplitIntoWords(s);
	cout << words.size() << " ";
	for (auto it = begin(words); it != end(words); ++it) 
	{
		if (it != begin(words)) 
		{
			cout << "/";
		}
		cout << *it;
	}
	cout << endl;
	return 0;
}
