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

using namespace std;

/*

Имеется база регионов, представленная вектором структур Region.
Здесь Lang — идентификатор языка.

Напишите функцию FindMaxRepetitionCount, принимающую базу регионов и определяющую, 
какое максимальное количество повторов(число вхождений одного и того же элемента) 
она содержит. Две записи (объекты типа Region) считаются различными, 
если они отличаются хотя бы одним полем.

Если все записи уникальны, считайте максимальное количество повторов равным 1. 
Если записи отсутствуют, верните 0. 
Гарантируется, что типа int достаточно для хранения ответа.

*/

enum class Lang
{
  DE, FR, IT
};

struct Region 
{
	string std_name;
	string parent_std_name;
	map<Lang, string> names;
	int64_t population;
};

bool operator < (const Region & lhs, const Region & rhs)
{
	return tie(lhs.std_name, lhs.parent_std_name, lhs.names, lhs.population) < 
		   tie(rhs.std_name, rhs.parent_std_name, rhs.names, rhs.population);
} 

int FindMaxRepetitionCount(const vector<Region> & regions)
{
	int r = 1;
	map<Region, int> Povtoreniya;
	for(auto x : regions)
	{
		if(Povtoreniya.count(x) == 0)
		{
			Povtoreniya[x] = 1;
			continue;
		}
		Povtoreniya[x]++;
		r = max(r, Povtoreniya[x]);
	}
	switch(r)
	{
		case 1:
		{
			if(regions.size() == 0)
			{
				return 0;
			}
			else
			{
				return 1;
			}
			break;
		}
		default:
		{
			return r;
		}
	}
}

int main() {
  cout << FindMaxRepetitionCount({
      {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Russia",
          "Eurasia",
          {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
          89
      }, {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Russia",
          "Eurasia",
          {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
          89
      },
  }) << endl;
  
  cout << FindMaxRepetitionCount({
      {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Russia",
          "Eurasia",
          {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
          89
      }, {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou deux"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Moscow",
          "Toulouse",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          89
      }, {
          "Moscow",
          "Russia",
          {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
          31
      },
  }) << endl;
  cout << FindMaxRepetitionCount({}) << endl;
  
  
  return 0;
}
