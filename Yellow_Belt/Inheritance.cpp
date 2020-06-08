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
#include <deque>

using namespace std;

/*

Дан следующий код:

class Animal {
public:
    // ваш код

    const string Name;
};


class Dog {
public:
    // ваш код

    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};

Определите до конца тела классов, соблюдая следующие требования:

Класс Dog должен являться наследником класса Animal.
Конструктор класса Dog должен принимать параметр типа string и 
инициализировать им поле Name в классе Animal.

*/

class Animal {
public:
    Animal(string s)
    : Name(s)
    {

    }

    const string Name;
};


class Dog : public Animal{
public:
   	Dog(string s)
   	: Animal(s)
   	{

   	}

    void Bark() {
        cout << Name << " barks: woof!" << endl;
    }
};

int main(void)
{
	Dog Mirana("Good girl");
	return 0;
}

/*



*/