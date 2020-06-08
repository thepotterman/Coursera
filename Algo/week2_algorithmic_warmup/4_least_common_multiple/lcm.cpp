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

long long lcm_naive(int64_t a, int64_t b) {
  for (long l = 1; l <= (long long) a * b; ++l)
    if (l % a == 0 && l % b == 0)
      return l;

  return (long long) a * b;
}

int64_t lcm_fast(int64_t a, int64_t b)
{
	if(b > a)
	{
		return lcm_fast(b, a);
	}
	if(b % a == 0)
	{
		return b;
	}
	for(int i = 1; b % a != 0; i++)
	{
		b = (b / i) * (i + 1);
	}
	if(b % a == 0)
	{
		return b;
	}

}

int main() {
  int64_t a, b;
  cin >> a >> b;
//  cout << lcm_naive(a, b) << endl;
  cout << lcm_fast(a, b) << endl;
  return 0;
}
