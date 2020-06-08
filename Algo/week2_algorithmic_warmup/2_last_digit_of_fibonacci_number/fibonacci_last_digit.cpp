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

int fibonacci_fast(int n)
{
    int prev = 0, cur = 1, buffer = 0;
    for(int i = 0; i < n; i++)
    {
        buffer = prev + cur;
        buffer %= 10;
        prev = cur % 10;
        cur = buffer % 10;
    }
    return prev % 10;
}

int main() {
    int n = 0;
    std::cin >> n;
    int c = fibonacci_fast(n);
    std::cout << c << '\n';
    }
