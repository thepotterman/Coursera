#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using namespace std;


bool IsSubdomain(const string & subdomain, const string & domain) {
    if(domain.size() < subdomain.size()) {
        return false;
    }
    for(size_t i = 0; i < subdomain.size(); ++i) {
        if(subdomain[i] != domain[i]) {
            return false;
        }
    }
    return true;
}

int main() {
    vector<string> banned_domains;
    vector<string> domains_to_check;
    banned_domains.reserve(1000);
    domains_to_check.reserve(1000);
    size_t count = 0;
    string s;

    cin >> count;
    for(size_t i = 0; i < count; ++i) {
        s.reserve(50);
        cin >> s;
        reverse(s.begin(), s.end());
        //banned_domains.push_back(move(s));
        //banned_domains.push_back("." + move(s));
        banned_domains.push_back(move(s) + ".");
    }

    count = 0;
    cin >> count;
    for(size_t i = 0; i < count; ++i) {
        s.reserve(50);
        cin >> s;
        reverse(s.begin(), s.end());
        s.push_back('.');
        domains_to_check.push_back(move(s));
    }

    sort(banned_domains.begin(), banned_domains.end());

    size_t insert_pos = 0;

    for(size_t i = 0; i < banned_domains.size(); ++i) {
        if(insert_pos == 0 || 
           !IsSubdomain(banned_domains[insert_pos - 1], banned_domains[i])) {
            swap(banned_domains[i], banned_domains[insert_pos]);
            ++insert_pos;
        }
    }
    banned_domains.resize(insert_pos);

/*    for(const string & ss : banned_domains) {
        cout << endl << ss;
    }
    cout << endl;
*/
    for(const string & ss : domains_to_check) {
        auto it = upper_bound(banned_domains.begin(), banned_domains.end(), ss);
        if(it != banned_domains.begin())
        {
            it--;
        }
//        cout << endl << "domain: " << ss << endl << "banned: " << *it << endl << " result: ";
        if(IsSubdomain(*it, ss)) {
            cout << "Bad\n";
        } else {
            cout << "Good\n";
        }
    }

    return 0;
}
