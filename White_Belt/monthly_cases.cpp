/*

У каждого из нас есть повторяющиеся ежемесячные дела, каждое из которых нужно выполнять в конкретный день каждого месяца:
оплата счетов за электричество, абонентская плата за связь и пр.
Вам нужно реализовать работу со списком таких дел на месяц, а именно, реализовать набор следующих операций:
ADD i s Назначить дело с названием s на день i текущего месяца. 
DUMP i Вывести все дела, запланированные на день i текущего месяца. 
NEXT Перейти к списку дел на новый месяц. 
При выполнении данной команды вместо текущего (старого) списка дел на текущий месяц
создаётся и становится активным (новый) список дел на следующий месяц: 
все дела со старого списка дел копируются в новый список. 
После выполнения данной команды новый список дел и следующий месяц становятся текущими, а работа со старым списком дел прекращается. 
При переходе к новому месяцу необходимо обратить внимание на разное количество дней в месяцах: 
если следующий месяц имеет больше дней, чем текущий, «дополнительные» дни необходимо оставить пустыми (не содержащими дел); 
если следующий месяц имеет меньше дней, чем текущий, 
дела со всех «лишних» дней необходимо переместить на последний день следующего месяца.

*/

  
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include<cstring>

using namespace std;

void Adding(vector<vector<string>> & dela)
{
	int i = 0;
	string s;
	cin >> i >> s;
	i--;
	dela[i].push_back(s);
}

int Nexting(vector<vector<string>> & dela, const vector<int> & days, const int & curr)
{
	int next = 0;
	if(curr + 1 == 12)
	{
		next = 0;
	}
	else 
	{
		next = curr + 1;
	}
	if(days[curr] <= days[next])
	{
		dela.resize(days[next]);
	}
	else
	{
		for(int i = days[next]; i < days[curr]; i++)
		{
			dela[days[next] - 1].insert(end(dela[days[next] - 1]), begin(dela[i]), end(dela[i]));
		}
		dela.resize(days[next]);
	}
	return next;
}

void Dumping(const vector<vector<string>> & dela)
{
	int i = 0;
	cin >> i;
	i--;
	cout << dela[i].size() << " ";
	for(int j = 0; j < dela[i].size(); j++)
	{
		cout << dela[i][j] << " ";
	}
	cout << endl;
}

int main()
{
	int n = 0, curr = 0;
	string s;
	cin >> n;
	vector<int> days = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	vector<vector<string>> dela;
	dela.resize(days[curr]);
	for(int j = 0; j < n; j++)
	{
		cin >> s;
		if(s == "ADD")
		{
			Adding(dela);
		}
		else if(s == "DUMP")
		{
			Dumping(dela);
		}
		else if(s == "NEXT") 
		{
			curr = Nexting(dela, days, curr);
		}
	}
	return 0;
}
