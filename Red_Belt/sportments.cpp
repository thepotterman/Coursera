#include <iostream>
#include <algorithm>
#include <list>

using namespace std;

int main(void) {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int n = 0; 
    cin >> n;
    list<int> sportmens(n);
    while(n--) {
        int a = 0;
        int b = 0;
        cin >> a >> b;
        sportmens.insert(find(sportmens.begin(), sportmens.end(), b), a);
    }
    for(const auto & x : sportmens) {
        cout << x << '\n';
    }
    return 0;
}
