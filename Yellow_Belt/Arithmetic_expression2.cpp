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

Модифицируйте решение предыдущей части так, 
чтобы предыдущее выражение обрамлялось скобками лишь при необходимости, 
то есть только в том случае, 
когда очередная операция имеет бо́льший приоритет, чем предыдущая.

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
	string p = "*";
	deque<string> answer;
	cin >> x >> n;
	answer.push_back(to_string(x));
	for(int i = 0; i < n; i++)
	{
		cin >> c >> y;
		if(c == "*" || c == "/")
		{
			if(p == "+"|| p == "-")
			{
				//answer = "(" + answer + ")" + " " + c + " " + to_string(y);
				answer.push_front("(");
				answer.push_back(")");
				answer.push_back(" ");
				answer.push_back(c);
				answer.push_back(" ");
				answer.push_back(to_string(y));
				p = c;
				continue;
			}
		}
		//answer = answer + " " + c + " " + to_string(y);
		answer.push_back(" ");
		answer.push_back(c);
		answer.push_back(" ");
		answer.push_back(to_string(y));
		p = c;
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

(8 * 3 - 6) / 1

*/