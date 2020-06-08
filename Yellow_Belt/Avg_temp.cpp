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
	int64_t n = 0, t = 0, avg = 0, k = 0;
	vector<int64_t> nums;
	vector<int64_t> big;
	cin >> n;
	for(int64_t i = 0; i < n; i++)
	{
		cin >> t;
		nums.push_back(t);
		avg += t;
	}
	avg /= n;
	for(int64_t i = 0; i < n; i++)
	{
		if(nums[i] > avg)
		{
			k++;
			big.push_back(i);
		}
	}
	cout << k << endl;
	for(const auto & x : big)
	{
		cout << x << ' ';
	}
	return 0;
}

