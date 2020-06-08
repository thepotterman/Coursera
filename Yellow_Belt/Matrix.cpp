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

// Реализуйте здесь
// * класс Matrix
// * оператор ввода для класса Matrix из потока istream
// * оператор вывода класса Matrix в поток ostream
// * оператор проверки на равенство двух объектов класса Matrix
// * оператор сложения двух объектов класса Matrix

class Matrix
{
public:
	Matrix()
	{

	}
	Matrix(int num_rows, int num_cols)
	{
		mat.resize(num_rows);
		for(auto & x : mat)
		{
			x.resize(num_cols);
			for(auto & y : x)
			{
				y = 0;
			}
		}
	}
	Reset(int new_rows, int new_cols)
	{
		mat.resize(new_rows);
		for(auto & x : mat)
		{
			x.resize(new_cols);
			for(auto & y : x)
			{
				y = 0;
			}
		}
	}
	int At (const int & row, const int & col) const
	{
		return mat[row][col];
	}

private:
	vector<vector<int>> mat;
};

int main() 
{
	Matrix one;
	Matrix two;

	cin >> one >> two;
	cout << one + two << endl;
	return 0;
}
