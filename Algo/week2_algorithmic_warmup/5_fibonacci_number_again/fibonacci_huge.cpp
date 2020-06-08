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

int fibonacci_fast(int n, int m)
{
    int prev = 0, cur = 1, buffer = 0;
    for(int i = 0; i < n; i++)
    {
        buffer = prev + cur;
        buffer %= m;
        prev = cur % m;
        cur = buffer % m;
    }
    return prev % m;
}

long long get_fibonacci_huge_naive(long long n, long long m) {
    if (n <= 1)
        return n;

    long long previous = 0;
    long long current  = 1;

    for (long long i = 0; i < n - 1; ++i) {
        long long tmp_previous = previous;
        previous = current;
        current = tmp_previous + current;
    }

    return current % m;
}

int main() {
    long long n, m;
    cin >> n >> m;
//    cout << get_fibonacci_huge_naive(n, m) << '\n';
    cout << fibonacci_fast(n, m) << '\n';
}
