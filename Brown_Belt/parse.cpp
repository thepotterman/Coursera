#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

int main() {        
    const string name = []{                
        cout << "Enter the File name\n";                
        string s;                
        cin >> s;                
        return s;        
    }();        
    ifstream in(name);        
    string s;        
    vector<string> all;        
    while(getline(in, s)) {                
        string p;                
        for(const auto & a : s) {                        
            if(a == ' ') {                                
                p.push_back(' ');                        
            } else {                                
                break;                        
            }                
        }                
        p = p + s;                
        all.push_back(p);        
    }        
    const string name1 = []{                
        cout << "Enter the File name\n";                
        string s;                
        cin >> s;                
        return s;        
    }();        
    ofstream out(name1);        
    for(const auto & s : all) {                
        out << s << endl;        
    }        
    return 0;
}
