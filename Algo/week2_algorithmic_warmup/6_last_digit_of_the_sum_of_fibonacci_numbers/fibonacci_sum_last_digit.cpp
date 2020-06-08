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

int fibonacci_fast(uint64_t n)
{
    n %= 100;
    int64_t prev = 0, cur = 1, buffer = 0, s = 1;
    if(n <= 1)
    {
        return n;
    }
    for(uint64_t i = 0; i < n - 1; i++)
    {
        buffer = prev;
        prev = cur;
        cur = prev + buffer;
        cur %= 10;
        s += cur;
        s %= 10;
    }
    return s;
}


int fibonacci_sum_naive(long long n) {
    if (n <= 1)
        return n;

    long long previous = 0;
    long long current  = 1;
    long long sum      = 1;

    for (long long i = 0; i < n - 1; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
        sum += current;
    }

    return sum % 10;
}

int main() {
    uint64_t n = 0;
    cin >> n;
//    cout << fibonacci_sum_naive(n) << '\n';
    cout << fibonacci_fast(n) << "\n";
}
