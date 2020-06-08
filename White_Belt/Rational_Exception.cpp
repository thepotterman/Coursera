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

Вспомним класс Rational, который мы реализовали в задачах, 
посвящённых перегрузке операторов. Там специально говорилось, 
что в тестах объекты класса Rational никогда не создаются с нулевым знаменателем 
и никогда не выполняется деление на ноль. 
Настало время избавиться от этого ограничения и научиться обрабатывать нулевой знаменатель 
и деление на ноль. В этой задаче вам предстоит это сделать.

Поменяйте реализацию конструктора класса Rational так, 
чтобы он выбрасывал исключение invalid_argument, 
если знаменатель равен нулю. Кроме того, 
переделайте реализацию оператора деления для класса Rational так, 
чтобы он выбрасывал исключение domain_error, если делитель равен нулю.

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
        throw invalid_argument("0");
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
        throw domain_error("0");
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

int main() {
    try {
        Rational r(1, 0);
        cout << "Doesn't throw in case of zero denominator" << endl;
        return 1;
    } catch (invalid_argument&) {
    }

    try {
        auto x = Rational(1, 2) / Rational(0, 1);
        cout << "Doesn't throw in case of division by zero" << endl;
        return 2;
    } catch (domain_error&) {
    }

    cout << "OK" << endl;
    return 0;
}