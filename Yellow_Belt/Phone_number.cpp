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

#include "phone_number.h"

/*

Вам дан заголовочный файл phone_number.h, содержащий объявление класса PhoneNumber. 
При этом в комментариях описано поведение, которое ожидается от реализации этого класса.

#pragma once

#include <string>

using namespace std;

class PhoneNumber {
public:
  	 Принимает строку в формате +XXX-YYY-ZZZZZZ
     Часть от '+' до первого '-' - это код страны.
     Часть между первым и вторым символами '-' - код города
     Всё, что идёт после второго символа '-' - местный номер.
     Код страны, код города и местный номер не должны быть пустыми.
     Если строка не соответствует этому формату, нужно выбросить исключение invalid_argument. Проверять, что номер содержит только цифры, не нужно.

     Примеры:
     * +7-495-111-22-33
     * +7-495-1112233
     * +323-22-460002
     * +1-2-coursera-cpp
     * 1-2-333 - некорректный номер - не начинается на '+'
     * +7-1233 - некорректный номер - есть только код страны и города
  
  explicit PhoneNumber(const string &international_number);

  string GetCountryCode() const;
  string GetCityCode() const;
  string GetLocalNumber() const;
  string GetInternationalNumber() const;

private:
  string country_code_;
  string city_code_;
  string local_number_;
};

Пришлите cpp-файл с определениями методов класса PhoneNumber, которые реализуют описанное поведение.

*/
//

PhoneNumber::PhoneNumber(const string &international_number)
{
	stringstream ss(international_number);
	char c;
	string country_code;
	string city_code;
	string local_number;
	ss >> c;
	if(c != '+')
	{
		throw invalid_argument("");
	}
	getline(ss, country_code, '-');
	if(!ss)
	{
		throw invalid_argument("");
	}
	//ss.ignore(1);
	getline(ss, city_code, '-');
	if(!ss)
	{
		throw invalid_argument("");
	}
	//ss.ignore(1);
	getline(ss, local_number);
	if(!ss)
	{
		throw invalid_argument("");
	}
	country_code_ = country_code;
	city_code_ = city_code;
	local_number_ = local_number;
}

string PhoneNumber::GetCountryCode() const
{
	return country_code_;
}

string PhoneNumber::GetCityCode() const
{
	return city_code_;
}

string PhoneNumber::GetLocalNumber() const
{
	return local_number_;
}

string PhoneNumber::GetInternationalNumber() const
{
	string s;
	s += '+';
	s += country_code_;
	s += '-';
	s += city_code_;
	s += '-';
	s += local_number_;
	return s;
}


// int main(void)
// {
// 	PhoneNumber test("+7-999-999-99-99");
// 	cout << test.GetCountryCode() << endl;
// 	cout << test.GetCityCode() << endl;
// 	cout << test.GetLocalNumber() << endl;
// 	cout << test.GetInternationalNumber() << endl;
// 	return 0;
// }