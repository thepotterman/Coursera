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


class Date 
{
public:
	Date(int new_year, int new_month, int new_day)
	{
		if(new_month > 12 || new_month < 1)
		{
			string error = "Month value is invalid: " + to_string(new_month);
			throw runtime_error(error);
		}
		else if(new_day > 31 || new_day < 1)
		{
			string error = "Day value is invalid: " + to_string(new_day);
			throw runtime_error(error);
		}
		year = new_year;
		month = new_month;
		day = new_day;
	}
	int GetYear() const
	{
		return year;
	}
	int GetMonth() const
	{
		return month;
	}
	int GetDay() const
	{
		return day;
	}
private:
	int year;
	int month;
	int day;
};

bool operator<(const Date& l, const Date& r)
{
	if(l.GetYear() != r.GetYear())
	{
		return l.GetYear() < r.GetYear();
	}
	else
	{
		if(l.GetMonth() != r.GetMonth())
		{
			return l.GetMonth() < r.GetMonth();
		}
		else
		{
			return l.GetDay() < r.GetDay();
		}
	}
}

ostream & operator << (ostream & stream, const Date & date)
{
	stream << setw(4) << setfill('0') << date.GetYear() << '-'
		   << setw(2) << setfill('0') << date.GetMonth() << '-'
		   << setw(2) << setfill('0') << date.GetDay();
	return stream;
}

void CheckDefis(stringstream & stream, const string & s)
{
	if(stream.peek() != '-')
	{
		string error = "Wrong date format: " + s;
		throw runtime_error(error);
	}
	stream.ignore(1);
}

Date StringToDate(const string & s)
{
	int day = 0, month = 0, year = 0;
	string s2 = s;
	stringstream stream(s2);
	stream >> year;
	if(!stream)
	{
		string error = "Wrong date format: " + s;
		throw runtime_error(error);
	}
	CheckDefis(stream, s);
	stream >> month;
	if(!stream)
	{
		string error = "Wrong date format: " + s;
		throw runtime_error(error);
	}
	CheckDefis(stream, s);
	stream >> day;
	if(!stream)
	{
		string error = "Wrong date format: " + s;
		throw runtime_error(error);
	}
	if(stream.peek() != -1)
	{
		string error = "Wrong date format: " + s;
		throw runtime_error(error);
	}
	return Date(year, month, day);
}

class Database {
public:
	void AddEvent(const Date& date, const string& event)
	{
		dates[date].insert(event);
	}
	bool DeleteEvent(const Date& date, const string& event)
	{
		if(dates.count(date) > 0)
		{
			if(dates[date].count(event) > 0)
			{
				dates[date].erase(event);
				return true;
			}
		}
		return false;
	}
	int  DeleteDate(const Date& date)
	{
		int n = 0;
		if(dates.count(date) > 0)
		{
			n = dates[date].size();
			dates.erase(date);
		}
		return n;
	}

  	void Find(const Date& date) 
  	{
  		if(dates.count(date) > 0)
		{
			for(const string & s : dates[date])
			{
				cout << s << endl;
			}
		}
  	}
  
	void Print() const
	{
		for(auto e : dates)
		{
			for(const string & s : e.second)
			{
				cout << e.first << " ";
				cout << s << endl;
			}
		}
	}
private:
	map<Date, set<string>> dates;
};

int main() 
{
	Database db;
    
	string command, s, date, event;
	int n = 0;
	bool flag = false;
	while (getline(cin, command))
	{
		try
		{
			s.clear();
			date.clear();
			event.clear();
			stringstream stream(command);
			stream >> s;
			if(s == "Add")
			{
				stream >> date;
				stream >> event;
				db.AddEvent(StringToDate(date), event);
			}
			else if(s == "Del")
			{
				stream >> date;
				stream >> event;
				if(event.size() == 0)
				{
					n = db.DeleteDate(StringToDate(date));
					cout << "Deleted " << n << " events\n";
				}
				else
				{
					flag = db.DeleteEvent(StringToDate(date), event);
					if(flag == false)
					{
						cout << "Event not found\n";
					}
					else
					{
						cout << "Deleted successfully\n";
					}
				}
			}
			else if(s == "Find")
			{
				stream >> date;
				db.Find(StringToDate(date));
			}
			else if(s == "Print")
			{
				db.Print();
			}
			else if(s.size() != 0)
			{
				string error = "Unknown command: " + s;
				throw runtime_error(error);
			}
		}
		catch (runtime_error & e)
		{
			cout << e.what() << endl;
		}
  	}

	return 0;
}


