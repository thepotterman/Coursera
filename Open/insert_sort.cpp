#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void Sort(vector<int> & numbers, ofstream & output) {
    output << 1 << " ";
    for(int i = 1; i < numbers.size(); ++i) {
        int key = numbers[i];
        int j = i - 1;
        while(j >= 0 && numbers[j] > key) {
            numbers[j + 1] = numbers[j];
            j--;
        }
        numbers[j + 1] = key;
        output << j + 2 << " ";
    }
    output << "\n";
}

void Print(const vector<int> & numbers, ofstream & output) {
    for(const int & x : numbers) {
        output << x << " ";
    }
    output << "\n";
}

int main() {
    vector<int> numbers;

    int n = 0;

    ifstream input("input.txt");
    ofstream output("output.txt");

    input >> n;
    
    while(n--){
        int x = 0;

        input >> x;

        numbers.push_back(x);
    }

    Sort(numbers, output);
    Print(numbers, output);

    return 0;
}
