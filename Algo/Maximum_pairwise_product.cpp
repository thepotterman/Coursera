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

int64_t MaxPairwiseProduct(vector<int64_t> & numbers) 
{
	int64_t n = numbers.size();
	int64_t answer = 0;
	sort(numbers.begin(), numbers.end());
	answer = numbers[n - 1] * numbers[n - 2];
    return answer;
}

int main() 
{
    int64_t n = 0, x = 0;
    cin >> n;
    vector<int64_t> numbers;
    for (int64_t i = 0; i < n; ++i) {
        cin >> x;
        numbers.push_back(x);
    }

    cout << MaxPairwiseProduct(numbers) << '\n';
    return 0;
}
