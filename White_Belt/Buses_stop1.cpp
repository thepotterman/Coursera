/*

В этой задаче вам нужно присваивать номера автобусным маршрутам.

А именно, для каждого маршрута, заданного множеством названий остановок, нужно либо выдать новый номер 
(первому маршруту — 1, второму — 2 и т. д.), 
либо вернуть номер существующего маршрута, которому соответствует такое множество остановок.

Наборы остановок, которые можно получить друг из друга перестановкой элементов или добавлением/удалением повторяющихся, 
следует считать одинаковыми.

Формат ввода
Сначала вводится количество запросов Q, затем Q описаний запросов.

Каждый запрос представляет собой положительное количество остановок N, 
за которым следуют разделённые пробелом N названий остановок соответствующего маршрута (не обязательно различных). 
Названия остановок состоят лишь из латинских букв и символов подчёркивания.

Формат вывода
Выведите ответ на каждый запрос в отдельной строке.

Если маршрут с данным набором остановок уже существует, в ответ на соответствующий запрос выведите Already exists for i, 
где i — номер маршрута с таким набором остановок. 
В противном случае нужно выделить введённому набору остановок новый номер i и вывести его в формате New bus i.

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
	int q = 0, n = 0, nomer = 0;
	bool flag;
	map<int, set<string>> Buses;
	string s;
	cin >> q;
	for(int i = 0; i < q; i++)
	{
		Buses[nomer].clear();
		flag = false;
		cin >> n;
		for(int j = 0; j < n; j++)
		{
			cin >> s;
			Buses[nomer].insert(s);
		}
		for(int j = 0; j < nomer; j++)
		{
			if(Buses[j] == Buses[nomer])
			{
				cout << "Already exists for " << j + 1 << endl;
				nomer--;
				flag = true;
			}
		}
		if(flag == false)
		{
			cout << "New bus " << nomer + 1 << endl;
		}
		nomer++;
	}
	return 0;
}