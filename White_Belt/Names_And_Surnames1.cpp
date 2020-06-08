/*

Реализуйте класс для человека, поддерживающий историю изменений человеком своих фамилии и имени.

Считайте, что в каждый год может произойти не более одного изменения фамилии и не более одного изменения имени. 
При этом с течением времени могут открываться всё новые факты из прошлого человека, 
поэтому года́ в последовательных вызовах методов ChangeLastName и ChangeFirstName не обязаны возрастать.

Гарантируется, что все имена и фамилии непусты.

Строка, возвращаемая методом GetFullName, должна содержать разделённые одним пробелом имя и фамилию человека 
по состоянию на конец данного года.

Если к данному году не случилось ни одного изменения фамилии и имени, верните строку "Incognito".
Если к данному году случилось изменение фамилии, но не было ни одного изменения имени, верните "last_name with unknown first name".
Если к данному году случилось изменение имени, но не было ни одного изменения фамилии, верните "first_name with unknown last name".

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
				if(year1 <= item.first)
				{
					year1 = item.first;
					flag1 = true;
				}
			}
		}
		for(const auto & item : year_to_last_name)
		{
			if(item.first <= year)
			{
				if(year2 <= item.first)
				{
					year2 = item.first;
					flag2 = true;
				}
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
private:
	map<int, string> year_to_first_name;
	map<int, string> year_to_last_name;
};

int main() {
  Person person;
  
  person.ChangeFirstName(1965, "Polina");
  person.ChangeLastName(1967, "Sergeeva");
  for (int year : {1900, 1965, 1990}) {
    cout << person.GetFullName(year) << endl;
  }
  
  person.ChangeFirstName(1970, "Appolinaria");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }
  
  person.ChangeLastName(1968, "Volkova");
  for (int year : {1969, 1970}) {
    cout << person.GetFullName(year) << endl;
  }
  
  return 0;
}
