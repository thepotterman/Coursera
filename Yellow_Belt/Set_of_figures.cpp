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
#include <memory>


using namespace std;

/*

Вам дана функция main, 
которая считывает из стандартного ввода команды по работе с набором геометрических фигур

Как видно из кода, есть два вида команд:

ADD — добавить фигуру в набор;
PRINT — для каждой фигуры в наборе распечатать название, периметр и площадь.
Программа поддерживает три вида фигур: прямоугольник, треугольник и круг. 

Их добавление описывается так:

ADD RECT width height — добавить прямоугольник с размерами widthи height
(например, ADD RECT 2 3).
ADD TRIANGLE a b c — добавить треугольник со сторонами a, b и c 
(например, ADD TRIANGLE 3 4 5).
ADD CIRCLE r — добавить круг радиуса r 
(например, ADD CIRCLE 5).

Не меняя функцию main, реализуйте недостающие функции и классы:

базовый класс Figure с чисто виртуальными методами Name, Perimeter и Area;
классы Triangle, Rect и Circle, которые являются наследниками класса Figure 
и переопределяют его виртуальные методы;
функцию CreateFigure, которая в зависимости от содержимого входного потока 
создаёт shared_ptr<Rect>, shared_ptr<Triangle> или shared_ptr<Circle>.
Гарантируется, что все команды ADD корректны; размеры всех фигур — 
это натуральные числа не больше 1000. В качестве значения π используйте 3,14.

*/

enum class FigureType
{
	TRIANGLE,
	RECT,
	CIRCLE
};

class Figure
{
public:
	virtual string Name() = 0;
	virtual double Perimeter() = 0;
	virtual double Area() = 0;
};

class Triangle : public Figure
{
public:
	Triangle(int a, int b, int c)
	{
		a_ = a;
		b_ = b;
		c_ = c;
	}
	string Name() override
	{
		return "TRIANGLE";
	}
	double Perimeter() override
	{
		double answer = a_ + b_ + c_;
		return answer;
	}
	double Area() override
	{
		double s = 0;
		double p = 0;
		p += static_cast<double>(a_);
		p += static_cast<double>(b_);
		p += static_cast<double>(c_);
		p /= 2;
		s = p * (p - a_) * (p - b_) * (p - c_);
		s = sqrt(s);
		return s;
	}
private:
	int a_;
	int b_;
	int c_;
};

class Rect : public Figure
{
public:
	Rect(int width, int height)
	{
		width_ = width;
		height_ = height;
	}
	string Name() override
	{
		return "RECT";
	}
	double Perimeter() override
	{
		double answer = 2 * width_ + 2 * height_;
		return answer;
	}
	double Area() override
	{
		double answer = width_ * height_;
		return answer;
	}
private:
	int width_;
	int height_;
};

class Circle : public Figure
{
public:
	Circle(int r)
	{
		r_ = r;
	}
	string Name() override
	{
		return "CIRCLE";
	}
	double Perimeter() override
	{
		double p = 3.14;
		p *= 2;
		p *= r_;
		return p;
	}
	double Area() override
	{
		double s = 3.14;
		s *= r_;
		s *= r_;
		return s;
	}
private:
	int r_;
};


shared_ptr<Figure> CreateFigure(istringstream & is)
{
	FigureType type_;
	string type;
	is >> type;
	if(type == "TRIANGLE")
	{
		type_ = FigureType::TRIANGLE;
		shared_ptr<Figure> t;
		int a = 0, b = 0, c = 0;
		is >> a >> b >> c;
		t = make_shared<Triangle>(a, b, c);
		return t;
	}
	else if(type == "RECT")
	{
		type_ = FigureType::RECT;
		shared_ptr<Figure> r;
		int width = 0, height = 0;
		is >> width >> height;
		r = make_shared<Rect>(width, height);
		return r;
	}
	else if(type == "CIRCLE")
	{
		type_ = FigureType::CIRCLE;
		shared_ptr<Figure> c;
		int r = 0;
		is >> r;
		c = make_shared<Circle>(r);
		return c;
	}
	throw runtime_error("");
}

int main() 
{
	vector<shared_ptr<Figure>> figures;
	for (string line; getline(cin, line); ) 
	{
		istringstream is(line);

		string command;
		is >> command;
		if (command == "ADD") 
		{
			figures.push_back(CreateFigure(is));
		}
		else if (command == "PRINT") 
		{
			for (const auto& current_figure : figures) 
			{
			    cout << fixed << setprecision(3)
			         << current_figure->Name() << " "
			         << current_figure->Perimeter() << " "
			         << current_figure->Area() << endl;
			}
		}
	}
	return 0;
}

/*

Ввод:

ADD RECT 2 3
ADD TRIANGLE 3 4 5
ADD RECT 10 20
ADD CIRCLE 5
PRINT

Вывод:

RECT 10.000 6.000
TRIANGLE 12.000 6.000
RECT 60.000 200.000
CIRCLE 31.400 78.500

*/