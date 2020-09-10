#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void Sort(vector<int> & numbers, ofstream & output) {
    for(int i = 0; i < numbers.size(); ++i) {
        int j = 0;
        for(j = i; j > 0; --j) {
            if(numbers[j] < numbers[i]) {
                break;
            }
        }
        int tmp = numbers[j + 1];
        numbers[j + 1] = numbers[i];
        numbers[i] = tmp;
        output << j + 1 << " ";
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
