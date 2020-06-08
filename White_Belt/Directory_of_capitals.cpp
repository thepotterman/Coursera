/*

Реализуйте справочник столиц стран.

На вход программе поступают следующие запросы:

CHANGE_CAPITAL country new_capital — изменение столицы страны country на new_capital, 
либо добавление такой страны с такой столицей, если раньше её не было. 

RENAME old_country_name new_country_name — переименование страны из old_country_name в new_country_name. 

ABOUT country — вывод столицы страны country. 

DUMP — вывод столиц всех стран. 

Формат ввода В первой строке содержится количество запросов Q, в следующих Q строках — описания запросов. 
Все названия стран и столиц состоят лишь из латинских букв, цифр и символов подчёркивания.

Формат вывода Выведите результат обработки каждого запроса:

В ответ на запрос CHANGE_CAPITAL country new_capital выведите Introduce new country country with capital new_capital, 
если страны country раньше не существовало; 
Country country hasn't changed its capital, если страна country до текущего момента имела столицу new_capital; 
Country country has changed its capital from old_capital to new_capital, 
если страна country до текущего момента имела столицу old_capital, 
название которой не совпадает с названием new_capital. 

В ответ на запрос RENAME old_country_name new_country_name выведите Incorrect rename, skip, 
если новое название страны совпадает со старым, 
страна old_country_name не существует или страна new_country_name уже существует; 
Country old_country_name with capital capital has been renamed to new_country_name, 
если запрос корректен и страна имеет столицу capital. 

В ответ на запрос ABOUT country выведите Country country doesn't exist, 
если страны с названием country не существует; 
Country country has capital capital, если страна country существует и имеет столицу capital. 

В ответ на запрос DUMP выведите There are no countries in the world, если пока не было добавлено ни одной страны; 
последовательность пар вида country/capital, описывающую столицы всех стран, если в мире уже есть хотя бы одна страна. 
При выводе последовательности пары указанного вида необходимо упорядочить по названию страны и разделять между собой пробелом.

*/


  
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include<cstring>
#include<map>

using namespace std;

void Changing(map<string, string> & world)
{
	string country;
	string new_capital;
	cin >> country >> new_capital;
	if(world.count(country) == 0)
	{
		world[country] = new_capital;
		cout << "Introduce new country " << country << " with capital " 
		<< new_capital << endl;
	}
	else if(world[country] == new_capital)
	{
		cout << "Country " << country << " hasn't changed its capital" << endl;
	}
	else
	{
		cout <<"Country " << country << " has changed its capital from " 
		<< world[country] << " to " << new_capital << endl;
		world[country] = new_capital;
	}
}

void Renaming(map<string, string> & world)
{
	string old_country_name, new_country_name, capital;
	cin >> old_country_name >> new_country_name;
	if(new_country_name == old_country_name || 
		world.count(old_country_name) == 0 ||
		world.find(new_country_name) != world.end())
	{
		cout << "Incorrect rename, skip" << endl;
	}
	else
	{
		capital = world[old_country_name];
		cout << "Country " << old_country_name  << " with capital "
		<< capital << " has been renamed to " << new_country_name << endl;
		world.erase(old_country_name);
		world[new_country_name] = capital;
	}
}

void Abouting(map<string, string> & world)
{
	string country;
	cin >> country;
	if(world.count(country) == 0)
	{
		cout << "Country " << country << " doesn't exist" << endl;
	}
	else
	{
		cout << "Country " << country << " has capital " << world[country] << endl;
	}
}

void Dumping(map<string, string> & world)
{
	if(world.size() == 0)
	{
		cout << "There are no countries in the world" << endl;
	}
	else
	{
		for(auto c : world)
		{
			cout << c.first << "/" << c.second << " ";
		}
	}
}

int main()
{
	int n = 0;
	cin >> n;
	string s;
	map <string, string> world;
	for(int i = 0; i < n; i++)
	{
		cin >> s;
		if(s == "CHANGE_CAPITAL")
		{
			Changing(world);
		}
		else if(s == "RENAME")
		{
			Renaming(world);
		}
		else if(s == "ABOUT")
		{
			Abouting(world);
		}
		else if(s == "DUMP")
		{
			Dumping(world);
		}
	}
	return 0;
}
