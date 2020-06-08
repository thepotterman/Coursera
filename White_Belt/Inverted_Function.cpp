#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <map>
#include <set>
 
using namespace std;

/*Реализуйте рассказанный на лекции класс Function, 
позволяющий создавать, вычислять и инвертировать функцию, 
состоящую из следующих элементарных операций:

прибавить вещественное число x;
вычесть вещественное число x.
При этом необходимо объявить константными все методы, которые по сути такими являются.
*/

struct Image 
{
	double quality;
	double freshness;
	double rating;
};

struct Params 
{
	double a;
	double b;
	double c;
};

class FunctionPart
{
public:
	FunctionPart (char new_operation, double new_value)
	{
		operation = new_operation;
		value = new_value;
	}
	double Apply(double new_value) const
	{
		if(operation == '+')
		{
			return new_value + value;
		}
		else
		{
			return new_value - value;
		}
	}
	void Invert()
	{
		if(operation == '+')
		{
			operation = '-';
		}
		else
		{
			operation = '+';
		}
	}
private:
	char operation;
	double value;
};

class Function 
{
public:
	void AddPart(char operation, double value)
	{
		parts.push_back({operation, value});
	}
	double Apply (double value) const
	{
		for(const auto & item : parts)
		{
			value = item.Apply(value);
		}
		return value;
	}
	void Invert()
	{
		for(auto & item : parts)
		{
			item.Invert();
		}
		reverse(parts.begin(), parts.end());
	}
private:
	vector<FunctionPart> parts;
};

Function MakeWeightFunction(const Params& params,
                            const Image& image)
{
	Function function;
	function.AddPart('-', image.freshness * params.a + params.b);
	function.AddPart('+', image.rating * params.c);
	return function;
}

double ComputeImageWeight(const Params& params, const Image& image)
{
	Function function = MakeWeightFunction(params, image);
	return function.Apply(image.quality);
}

double ComputeQualityByWeight(const Params& params,
                              const Image& image,
                              double weight)
{
	Function function = MakeWeightFunction(params, image);
	function.Invert();
	return function.Apply(weight);
}

int main()
{
	Image image = {10, 2, 6};
	Params params = {4, 2, 6};
	cout << ComputeImageWeight(params, image) << endl;
	cout << ComputeQualityByWeight(qzr0pjdsatvgaxec580356hvacre8t7pc5538pauvm
