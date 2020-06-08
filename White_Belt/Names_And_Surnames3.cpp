#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <set>
 
using namespace std;

/*

Дополните класс Person из задачи «Имена и фамилии — 2» конструктором, 
позволяющим задать имя и фамилию человека при рождении, а также сам год рождения. 
Класс не должен иметь конструктора по умолчанию.

При получении на вход года, который меньше года рождения:

методы GetFullName и GetFullNameWithHistory должны отдавать "No person";
методы ChangeFirstName и ChangeLastName должны игнорировать запрос.
Кроме того, необходимо объявить константными все методы, которые по сути ими являются.

Пример

int main() {
  Person person("Polina", "Sergeeva", 1960);
  for (int year : {1959, 1960}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeFirstName(1965, "Appolinaria");
  person.ChangeLastName(1967, "Ivanova");
  for (int year : {1965, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  return 0;
}

Вывод : 

No person
Polina Sergeeva
Appolinaria (Polina) Sergeeva
Appolinaria (Polina) Ivanova (Sergeeva)
*/

class Person {
public:
	Person(const string & name, const string & surname, int year)
	{
		year_to_first_name[year] = name;
		year_to_last_name[year] = surname;
	}
	void ChangeFirstName(int year, const string& first_name) 
	{
		year_to_first_name[year] = first_name;
	}
	void ChangeLastName(int year, const string& last_name) 
	{
		year_to_last_name[year] = last_name;
	}
	vector <string> GetHistoryNames(int year) const
	{
		vector <string> :: iterator ip;
		vector <string> names;
		for(const auto & item : year_to_first_name)
		{
			if(item.first <= year)
			{
				names.push_back(item.second);
			}
			else
			{
				break;
			}
		}
		reverse(names.begin(), names.end());
		ip = unique(names.begin(), names.end());
		names.resize(distance(names.begin(), ip));
		return names;
	}
	vector <string> GetHistorySurnames(int year) const
	{
		vector <string> :: iterator ip;
		vector <string> surnames;
		for(const auto & item : year_to_last_name)
		{
			if(item.first <= year)
			{
				surnames.push_back(item.second);
			}
			else
			{
				break;
			}
		}
		reverse(surnames.begin(), surnames.end());
		ip = unique(surnames.begin(), surnames.end());
		surnames.resize(distance(surnames.begin(), ip));
		return surnames;
	}
	string GetFullName (int year) const
	{
		vector<string> names = GetHistoryNames(year);
		vector<string> surnames = GetHistorySurnames(year);
		if(names.size() == 0)
		{
			return "No person";
		}
		else
		{
			return names[0] + " " + surnames[0];
		}
	}
	string GetFullNameWithHistory(int year) const
	{
		string s;
		vector<string> names = GetHistoryNames(year);
		vector<string> surnames = GetHistorySurnames(year);
		if(names.size() == 0)
		{
			return "No person";
		}
		else
		{
			s += names[0];
			for(int i = 1; i < names.size(); i++)
			{
				if(i == 1)
				{
					s += " (";
				}
				s += names[i];
				if(i != names.size() - 1)
				{
					s += ", ";
				}
				else
				{
					s += ")";
				}
			}
			s += " " + surnames[0];
			for(int i = 1; i < surnames.size(); i++)
			{
				if(i == 1)
				{
					s += " (";
				}
				s += surnames[i];
				if(i != surnames.size() - 1)
				{
					s += ", ";
				}
				else
				{
					s += ")";
				}
			}
		}
		return s;
	}
private:
	map<int, string> year_to_first_name;
	map<int, string> year_to_last_name;
};

int main() {
  Person person("Eugene", "Sokolov", 1900);
  person.ChangeLastName(1910, "Sokolov");
  person.ChangeFirstName(1920, "Evgeny");
  person.ChangeLastName(1930, "Sokolov");
  cout << person.GetFullNameWithHistory(1940) << endl;
  
  return 0;
}
