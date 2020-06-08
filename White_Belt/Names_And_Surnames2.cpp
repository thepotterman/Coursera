/*

Дополните класс из предыдущей задачи «Имена и фамилии — 1» методом GetFullNameWithHistory:

class Person {
public:
  void ChangeFirstName(int year, const string& first_name) {
    // добавить факт изменения имени на first_name в год year
  }
  void ChangeLastName(int year, const string& last_name) {
    // добавить факт изменения фамилии на last_name в год year
  }
  string GetFullName(int year) {
    // получить имя и фамилию по состоянию на конец года year
  }
  string GetFullNameWithHistory(int year) {
    // получить все имена и фамилии по состоянию на конец года year
  }
private:
  // приватные поля
};

В отличие от метода GetFullName, 
метод GetFullNameWithHistory должен вернуть не только последние имя и фамилию к концу данного года, 
но ещё и все предыдущие имена и фамилии в обратном хронологическом порядке. 

Если текущие факты говорят о том, что человек два раза подряд изменил фамилию или имя на одно и то же, 
второе изменение при формировании истории нужно игнорировать.

Для лучшего понимания формата см. примеры.

Пример 1

int main() {
  Person person;

  person.ChangeFirstName(1900, "Eugene");
  person.ChangeLastName(1900, "Sokolov");
  person.ChangeLastName(1910, "Sokolov");
  person.ChangeFirstName(1920, "Evgeny");
  person.ChangeLastName(1930, "Sokolov");
  cout << person.GetFullNameWithHistory(1940) << endl;
  
  return 0;
}

Вывод :

Evgeny (Eugene) Sokolov

Пример 2 :

int main() {
  Person person;
  
  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeFirstName(1990, "Polina");
  person.ChangeLastName(1990, "Volkova-Sergeeva");
  cout << person.GetFullNameWithHistory(1990) << endl;
  
  person.ChangeFirstName(1966, "Pauline");
  cout << person.GetFullNameWithHistory(1966) << endl;
  
  person.ChangeLastName(1960, "Sergeeva");
  for (int year : {1960, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }
  
  person.ChangeLastName(1961, "Ivanova");
  cout << person.GetFullNameWithHistory(1967) << endl;
  
  return 0;
}

Вывод :

Incognito
Polina with unknown last name
Polina Sergeeva
Polina Sergeeva
Appolinaria (Polina) Sergeeva
Polina Volkova (Sergeeva)
Appolinaria (Polina) Volkova (Sergeeva)
Polina (Appolinaria, Polina) Volkova-Sergeeva (Volkova, Sergeeva)
Pauline (Polina) with unknown last name
Sergeeva with unknown first name
Pauline (Polina) Sergeeva
Pauline (Polina) Sergeeva (Ivanova, Sergeeva)

Пример 3 :

int main() {
  Person person;

  person.ChangeFirstName(1965, "Polina");
  person.ChangeFirstName(1965, "Appolinaria");

  person.ChangeLastName(1965, "Sergeeva");
  person.ChangeLastName(1965, "Volkova");
  person.ChangeLastName(1965, "Volkova-Sergeeva");

  for (int year : {1964, 1965, 1966}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  return 0;
}

Вывод :

Incognito
Appolinaria Volkova-Sergeeva
Appolinaria Volkova-Sergeeva

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
 
class Person {
public:
	void ChangeFirstName(int year, const string& first_name) 
	{
		year_to_first_name[year] = first_name;
	}
	void ChangeLastName(int year, const string& last_name) 
	{
		year_to_last_name[year] = last_name;
	}
	string GetFullName(int year) 
	{
		string s;
		bool flag1 = false;
		bool flag2 = false;
		int year1 = 0;
		int year2 = 0;
		for(const auto & item : year_to_first_name)
		{
			if(item.first <= year)
			{
				year1 = item.first;
				flag1 = true;
			}
		}
		for(const auto & item : year_to_last_name)
		{
			if(item.first <= year)
			{
				year2 = item.first;
				flag2 = true;
			}
		}
		if(flag1 == true)
		{
			s += year_to_first_name[year1];
			if(flag2 == true)
			{
				s += " ";
				s += year_to_last_name[year2];
			}
			else
			{
				s += " with unknown last name";
			}
		}
		else
		{
			if(flag2 == true)
			{
				s += year_to_last_name[year2];
				s += " with unknown first name";
			}
			else
			{
				s += "Incognito";
			}
		}
		return s;
	}
	string PrintHistory(vector<int> & years, map<int, string> & names)
	{
		/*
		cout << "\n\n\n";
		for(const auto & year: years)
		{
			cout << year << " " << names[year] << " ";
		}
		cout << "\n\n\n";
		*/
		string s;
		if(years.size() < 2)
		{
			return "";
		}
		s += " (";
		for(int i = years.size() - 2; i >= 0 ; i--)
		{
			s += names[years[i]];
			if(i != 0)
			{
				s += ", ";
			}

		}
		s += ")";
//		cout << "\n\n\n\n" << s << "\n\n\n\n";
		return s;
	}
	string GetFullNameWithHistory(int year) 
	{
		string s;
		string prev;
		bool flag1 = false;
		bool flag2 = false;
		bool flag = false;
		vector<int> years_first;
		vector<int> years_last;
		int year1 = 0;
		int year2 = 0;
		for(const auto & item : year_to_first_name)
		{
			if(item.first <= year)
			{
				year1 = item.first;
				flag1 = true;
				if(prev != item.second)
				{
					years_first.push_back(item.first);
					prev = item.second;
				}
			}
		}
		for(const auto & item : year_to_last_name)
		{
			if(item.first <= year)
			{
				year2 = item.first;
				flag2 = true;
				if(prev != item.second)
				{
					years_last.push_back(item.first);
					prev = item.second;
				}
			}
		}													//NACHALO
		if(flag1 == true)
		{
			s += year_to_first_name[year1];
			s += PrintHistory(years_first, year_to_first_name);
			if(flag2 == true)
			{
				s += " ";
				s += year_to_last_name[year2];
				s += PrintHistory(years_last, year_to_last_name);
			}
			else
			{
				s += " with unknown last name";
			}
		}
		else
		{
			if(flag2 == true)
			{
				s += year_to_last_name[year2];
				s += PrintHistory(years_last, year_to_last_name);
				s += " with unknown first name";
			}
			else
			{
				s += "Incognito";
			}
		}
		return s;
	}
	void PrintAll()
	{
		for(const auto & item : year_to_first_name)
		{
			cout << item. first << " " << item.second << endl;
		}
		for(const auto & item : year_to_last_name)
		{
			cout << item. first << " " << item.second << endl;
		}
	}
private:
	map<int, string> year_to_first_name;
	map<int, string> year_to_last_name;
};

int main() {
  Person person;

  person.ChangeFirstName(1900, "Eugene");
  person.ChangeLastName(1900, "Sokolov");
  person.ChangeLastName(1910, "Sokolov");
  person.ChangeFirstName(1920, "Evgeny");
  person.ChangeLastName(1930, "Sokolov");
  cout << person.GetFullNameWithHistory(1940) << endl;
  
  return 0;
}


