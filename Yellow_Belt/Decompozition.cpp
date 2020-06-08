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

using namespace std;

/*

В курсе «Белый пояс по С++» у нас была задача «Автобусные остановки — 1». 
В этой задаче вам будет дано её правильное решение, целиком содержащееся внутри функции main. 
Вам надо будет выполнить декомпозицию этого решения на заранее заданные блоки так, 
чтобы получившаяся программа так же корректно решала задачу. 

*/

enum class QueryType {
  NewBus,
  BusesForStop,
  StopsForBus,
  AllBuses
};

struct Query {
  QueryType type;
  string bus;
  string stop;
  vector<string> stops;
};

istream& operator >> (istream& is, Query& q) 
{
	q.stops.clear();
	string s;
	is >> s;
	if(!is)
	{
		return is;
	}
	if(s == "NEW_BUS")
	{
		q.type = QueryType::NewBus;
		int stop_count = 0;
		string stop;
		is >> q.bus >> stop_count;
		for(int i = 0; i < stop_count; i++)
		{
			cin >> stop;
			q.stops.push_back(stop);
		}
	}
	else if(s == "BUSES_FOR_STOP")
	{
		q.type = QueryType::BusesForStop;
		is >> q.stop;
	}
	else if(s == "STOPS_FOR_BUS")
	{
		q.type = QueryType::StopsForBus;
		is >> q.bus;
	}
	else
	{
		q.type = QueryType::AllBuses;
	}
	return is;
}

struct BusesForStopResponse 
{
	string answer;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) 
{
	os << r.answer;
	return os;
}

struct StopsForBusResponse 
{
	string answer;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) 
{
	os << r.answer;
	return os;
}

struct AllBusesResponse 
{
	string answer;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) 
{
	os << r.answer;
	return os;
}

class BusManager 
{
public:
	void AddBus(const string& bus, const vector<string>& stops) 
	{
		for(const auto & stop: stops)
		{
			Bus_to_stop[bus].push_back(stop);
			Stop_to_bus[stop].push_back(bus);
		}
	}

	BusesForStopResponse GetBusesForStop(const string& stop) const 
	{
		BusesForStopResponse r;
		if(Stop_to_bus.count(stop) == 0)
		{
			r.answer = "No stop";
			return r;
			
		}
		for (const string & bus : Stop_to_bus.at(stop))
		{
			r.answer += bus;
			r.answer += " ";
		}
		// r.answer += "\n";
		return r;
	}

	StopsForBusResponse GetStopsForBus(const string& bus) const 
	{
		StopsForBusResponse r;
		if(Bus_to_stop.count(bus) == 0)
		{
			r.answer = "No bus";
			return r;
		}
		for(const auto & stop : Bus_to_stop.at(bus))
		{
			r.answer += "Stop ";
			r.answer += stop;
			r.answer += ": ";
			for(const auto & buses: Stop_to_bus.at(stop))
			{
				if(buses != bus)
				{
					r.answer += buses;
					r.answer += " ";
				}
				if(Stop_to_bus.at(stop).size() == 1)
				{
					r.answer += "no interchange";
				}
			}
			r.answer += "\n";
		}
		r.answer.pop_back();
		return r;
	}

	AllBusesResponse GetAllBuses() const 
	{
		AllBusesResponse r;
		for(const auto & x : Bus_to_stop)
		{
			r.answer += "Bus ";
			r.answer += x.first;
			r.answer += ": ";
			for(const auto stop : x.second)
			{
				r.answer += stop;
				r.answer += " ";
			}
			r.answer += "\n";
		}
		if(Bus_to_stop.size() == 0)
		{
			r.answer += "No buses\n";
		}
		r.answer.pop_back();
		return r;
	}
private:
	map <string, vector<string>> Bus_to_stop;
	map <string, vector <string>> Stop_to_bus;
};

// Не меняя тела функции main, реализуйте функции и классы выше

int main() 
{
	int query_count;
	Query q;

	cin >> query_count;

	BusManager bm;
	for (int i = 0; i < query_count; ++i) 
	{
		cin >> q;
		switch (q.type) 
		{
			case QueryType::NewBus:
				bm.AddBus(q.bus, q.stops);
				break;
			case QueryType::BusesForStop:
				cout << bm.GetBusesForStop(q.stop) << endl;
				break;
			case QueryType::StopsForBus:
				cout << bm.GetStopsForBus(q.bus) << endl;
				break;
			case QueryType::AllBuses:
				cout << bm.GetAllBuses() << endl;
				break;
		}
	}
	return 0;
}