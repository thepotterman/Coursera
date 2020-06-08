/*

Реализуйте систему хранения автобусных маршрутов. Вам нужно обрабатывать следующие запросы:

NEW_BUS bus stop_count stop1 stop2 ... — 
добавить маршрут автобуса с названием bus и stop_count остановками с названиями stop1, stop2, ... 

BUSES_FOR_STOP stop — вывести названия всех маршрутов автобуса, проходящих через остановку stop. 

STOPS_FOR_BUS bus — вывести названия всех остановок маршрута bus со списком автобусов, на которые можно пересесть на каждой из остановок. 

ALL_BUSES — вывести список всех маршрутов с остановками. 


Формат ввода В первой строке ввода содержится количество запросов Q, затем в Q строках следуют описания запросов.

Гарантируется, что все названия маршрутов и остановок состоят лишь из латинских букв, цифр и знаков подчёркивания.

Для каждого запроса NEW_BUS bus stop_count stop1stop2 ... гарантируется, что маршрут bus отсутствует, 
количество остановок больше 0, а после числа stop_count следует именно такое количество названий остановок, 
причём все названия в каждом списке различны.

Формат вывода Для каждого запроса, кроме NEW_BUS, выведите соответствующий ответ на него:

На запрос BUSES_FOR_STOP stop выведите через пробел список автобусов, проезжающих через эту остановку, 
в том порядке, в котором они создавались командами NEW_BUS. 
Если остановка stop не существует, выведите No stop. 

На запрос STOPS_FOR_BUS bus выведите описания остановок маршрута bus в отдельных строках в том порядке, 
в котором они были заданы в соответствующей команде NEW_BUS. 
Описание каждой остановки stop должно иметь вид Stop stop: bus1bus2 ..., 
где bus1bus2 ... — список автобусов, проезжающих через остановку stop, в порядке, в котором они создавались командами NEW_BUS, 
за исключением исходного маршрута bus. 
Если через остановку stop не проезжает ни один автобус, кроме bus, вместо списка автобусов для неё выведите no interchange. 
Если маршрут bus не существует, выведите No bus. 

На запрос ALL_BUSES выведите описания всех автобусов в алфавитном порядке. 
Описание каждого маршрута bus должно иметь вид Bus bus: stop1stop2 ..., 
где stop1stop2 ... — список остановок автобуса bus в порядке, в котором они были заданы в соответствующей команде NEW_BUS. 
Если автобусы отсутствуют, выведите No buses.

*/


#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>

using namespace std;

void Add(map <string, vector<string>> & Buses, map <string, vector <string>> & Stops)
{
	string stop;
	string bus;
	int stop_count = 0;
	cin >> bus >> stop_count;
	for(int i = 0; i < stop_count; i++)
	{
		cin >> stop;
		Buses[bus].push_back(stop);
		Stops[stop].push_back(bus);
	}
}

void Buses_for_stop(map <string, vector <string>> & Stops)
{
	string stop;
	cin >> stop;
	if(Stops.count(stop) == 0)
	{
		cout << "No stop \n";
		return;
	}
	for (const string & bus : Stops[stop])
	{
		cout << bus << " ";
	}
	cout << "\n";
}

void Stops_for_bus(map <string, vector<string>> & Buses, map <string, vector <string>> & Stops)
{
	bool flag;
	string bus;
	cin >> bus;
	if(Buses.count(bus) == 0)
	{
		cout << "No bus\n";
		return;
	}
	for(const auto & stop : Buses[bus])
	{
		flag = false;
		cout << "Stop " << stop << ": ";
		for(const string & buses : Stops[stop])
		{
			if(buses != bus)
			{
				cout << buses << " ";
				flag = true;
			}
		}
		if(flag == false)
		{
			cout << "no interchange";
		}
		cout << endl;
	}
}

void All_buses(map <string, vector<string>> & Buses, map <string, vector <string>> & Stops)
{
	bool flag = false;
	for(auto bus : Buses)
	{
		flag = true;
		cout << "Bus " << bus.first << ": ";
		for(auto stop : bus.second)
		{
			cout << stop << " ";
		}
		cout << endl;
	}
	if(flag == false)
	{
		cout << "No buses\n";
	}
}

int main()
{
	int n = 0;
	cin >> n;
	string s;
	map <string, vector <string>> Buses;
	map <string, vector <string>> Stops;
	for(int i = 0; i < n; i++)
	{
		cin >> s;
		if(s == "NEW_BUS")
		{
			Add(Buses, Stops);
		}
		else if(s == "BUSES_FOR_STOP")
		{
			Buses_for_stop(Stops);
		}
		else if(s == "STOPS_FOR_BUS")
		{
			Stops_for_bus(Buses, Stops);
		}
		else
		{
			All_buses(Buses, Stops);
		}
	}
	return 0;
}
