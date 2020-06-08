#include <iostream>
#include <stdexcept>
#include <map>
using namespace std;

template<class K, class V>
V& GetRefStrict(const map<K, V>& m, const K& key);

template<class K, class V>
V& GetRefStrict(const map<K, V>& m, const K& key) {

if (m.count(key) == 0) {
throw runtime_error("no such key in dictionary");
}
return m.at(key);
}
int main() {
map<int, string> m = { {0, "value"} };
string& item = GetRefStrict(m, 0);
item = "newvalue";
cout << m[0] << endl;
}