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

//

int main() 
{
	int n = 0;
	uint64_t sum = 0, w = 0, h = 0, d = 0, ro = 0;
	cin >> n >> ro;
	for(int i = 0; i < n; i++)
	{
		cin >> w >> h >> d;
		sum += (w * h * d * ro);
	}
	cout << sum << endl;
	return 0;
}
