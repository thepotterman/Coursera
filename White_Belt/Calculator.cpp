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
#include <sstream>

using namespace std;

/*

Итак, теперь у нас с вами есть полноценный тип для представления обыкновенных дробей: 
класс Rational интегрирован в язык с помощью перегрузки операторов и не позволяет 
выполнять некорректные операции за счёт выбрасывания исключений. 
Давайте на основе класса Rational создадим простой калькулятор для обыкновенных дробей.

Вам надо написать программу, которая считывает из стандартного ввода одну строку 
в формате ДРОБЬ_1 операция ДРОБЬ_2. ДРОБЬ_1 и ДРОБЬ_2 имеют формат X/Y, 
где X — целое, а Y — целое неотрицательное число. 
операция — это один из символов '+', '-', '*', '/'.

Если ДРОБЬ_1 или ДРОБЬ_2 является некорректной обыкновенной дробью, 
ваша программа должна вывести в стандартный вывод сообщение "Invalid argument". 
Если считанная операция — это деление на ноль, выведите в стандартный вывод 
сообщение "Division by zero". В противном случае выведите результат операции.

*/

int gcd (int a, int b)
{
  if(a == 0)
  {
    return b;
  }
  else if(a > b)
  {
    a = a % b;
    return gcd(a, b);
  }
  else
  {
    b = b % a;
    return gcd(b, a);
  }
}


class Rational {
public:
    Rational() 
    {
        numerator = 0;
        denominator = 1;
    }

    Rational(int new_numerator, int new_denominator) 
    {
      if(new_denominator == 0)
      {
        throw invalid_argument("Invalid argument");
      }
      int new_gcd = 0;
      if(new_numerator * new_denominator > 0)
      {
        new_numerator = abs(new_numerator);
        new_denominator = abs(new_denominator);
      }
      else
      {
        if(new_denominator < 0)
        {
          new_denominator *= -1;
          new_numerator *= -1;
        }
      }
      new_gcd = gcd(abs(new_numerator), abs(new_denominator));
        numerator = new_numerator / new_gcd;
        denominator = new_denominator / new_gcd;
    }

    int Numerator() const 
    {
        return numerator;
    }

    int Denominator() const 
    {
        return denominator;
    }

private:
    int numerator;
    int denominator;
};

bool operator== (const Rational & num1, const Rational & num2)
{
  if(num1.Numerator() == num2.Numerator() &&
    num1.Denominator() == num2.Denominator())
  {
    return true;
  }
  return false;
}

bool operator< (const Rational & num1, const Rational & num2)
{
  if(num1.Numerator() * num2.Denominator() < 
    num2.Numerator() * num1.Denominator())
  {
    return true;
  }
  return false;
}

Rational operator+ (const Rational & num1, const Rational & num2)
{
  
  return Rational(
    (num1.Numerator() * num2.Denominator()) + 
    (num2.Numerator() * num1. Denominator()),
    (num1.Denominator() * num2.Denominator()));
}

Rational operator- (const Rational & num1, const Rational & num2)
{
  return Rational(
    (num1.Numerator() * num2.Denominator()) - 
    (num2.Numerator() * num1. Denominator()),
    (num1.Denominator() * num2.Denominator()));
}

Rational operator*(const Rational & num1, const Rational & num2)
{
    return Rational(num1.Numerator() * num2.Numerator(),
                    num1.Denominator() * num2.Denominator());
}

Rational operator/(const Rational & num1, const Rational & num2)
{
    if(num2.Numerator() == 0)
    {
        throw invalid_argument("Division by zero");
    }
    return Rational(num1.Numerator() * num2.Denominator(),
                    num1.Denominator() * num2.Numerator());
}

ostream & operator<<(ostream & stream, const Rational & num)
{
    stream << num.Numerator();
    stream << "/";
    stream << num.Denominator();
    return stream;
}

istream & operator>>(istream & stream, Rational & num)
{
    int n = num.Numerator(), d = num.Denominator();
    stream >> n;
    stream.ignore(1);
    stream >> d;
    num = Rational(n, d);
    return stream;
} 

Rational StringToRational(const string & s)
{
	stringstream stream(s);
	int n = 0, d = 0;
	stream >> n;
	if(stream.peek() != '/')
	{
		throw invalid_argument("Invalid argument");
	}
	stream.ignore(1);
	stream >> d;
	return Rational(n, d);
}

Rational MakeOperation(const string & operation, const Rational & num1, 
													const Rational & num2)
{
	if(operation == "+")
	{
		return num1 + num2;
	}
	else if(operation == "-")
	{
		return num1 - num2;
	}
	else if(operation == "*")
	{
		return num1 * num2;
	}
	else if(operation == "/")
	{
		return num1 / num2;
	}
	else
	{
		throw invalid_argument("Invalid argument");
	}
}

int main() 
{
	string s, operation;
    try
    {
    	cin >> s;
    	Rational num1 = StringToRational(s);
    	cin >> operation;
    	cin >> s;
    	Rational num2 = StringToRational(s);
    	num1 = MakeOperation(operation, num1, num2);
    	cout << num1;
    }
    catch (invalid_argument & e)
    {
    	cout << e.what();
    }
    return 0;
}