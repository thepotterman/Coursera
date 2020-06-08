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
#include <deque>

using namespace std;

/*

Реализуйте построение арифметического выражения согласно следующей схеме:

изначально есть выражение, состоящее из некоторого целого числа x;
на каждом шаге к текущему выражению применяется некоторая операция: 
прибавление числа, вычитание числа, умножение на число или деление на число; 
перед применением операции выражение всегда должно быть заключено в скобки.

*/

void Print(deque<string> & answer)
{
	for(string s : answer)
	{
		cout << s;
	}
	cout << endl;
}

int main(void)
{
	int x = 0, n = 0, y = 0;
	string s;
	string c;
	deque<string> answer;
	cin >> x >> n;
	answer.push_back(to_string(x));
	for(int i = 0; i < n; i++)
	{
		cin >> c >> y;
		//answer = "(" + answer + ")" + " " + c + " " + to_string(y);
		
		answer.push_front("(");
		answer.push_back(")");
		answer.push_back(" ");
		answer.push_back(c);
		answer.push_back(" ");
		answer.push_back(to_string(y));
	}
	Print(answer);
	return 0;
}

/*

Ввод:

8
3
* 3
- 6
/ 1

Вывод:

(((8) * 3) - 6) / 1

*/