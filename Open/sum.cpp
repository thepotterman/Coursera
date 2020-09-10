#include <iostream>
#include <fstream>

using namespace std;

int main() {
    int64_t a = 0;
    int64_t b = 0;

    ifstream input("input.txt");

    ofstream output("output.txt");

    input >> a >> b;

    output << a + b * b;
    return 0;
}
