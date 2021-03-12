#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <optional>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

template <typename It>
class Range {
public:
    Range(It begin, It end) : begin_(begin), end_(end) {}
    It begin() const { return begin_; }
    It end() const { return end_; }

private:
    It begin_;
    It end_;
};

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ") {
    const size_t pos = s.find(delimiter);
    if (pos == s.npos) {
        return {s, nullopt};
    } else {
        return {s.substr(0, pos), s.substr(pos + delimiter.length())};
    }
}

vector<string_view> Split(string_view s, string_view delimiter = " ") {
    vector<string_view> parts;
    if (s.empty()) {
        return parts;
    }
    while (true) {
        const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
        parts.push_back(lhs);
        if (!rhs_opt) {
            break;
        }
        s = *rhs_opt;
    }
    return parts;
}


class Domain {
public:
    explicit Domain(string_view text) {
        vector<string_view> parts = Split(text, ".");
        parts_reversed_.assign(rbegin(parts), rend(parts));
    }

    size_t GetPartCount() const {
        return parts_reversed_.size();
    }

    auto GetParts() const {
        return Range(rbegin(parts_reversed_), rend(parts_reversed_));
    }
    auto GetReversedParts() const {
        return Range(begin(parts_reversed_), end(parts_reversed_));
    }

    bool operator==(const Domain& other) const {
        return parts_reversed_ == other.parts_reversed_;
    }

private:
    vector<string> parts_reversed_;
};

ostream& operator<<(ostream& stream, const Domain& domain) {
    bool first = true;
    for (const string_view part : domain.GetParts()) {
        if (!first) {
            stream << '.';
        } else {
            first = false;
        }
        stream << part;
    }
    return stream;
}

// domain is subdomain of itself
bool IsSubdomain(const Domain& subdomain, const Domain& domain) {
    const auto subdomain_reversed_parts = subdomain.GetReversedParts();
    const auto domain_reversed_parts = domain.GetReversedParts();
    return
            subdomain.GetPartCount() >= domain.GetPartCount()
            && equal(begin(domain_reversed_parts), end(domain_reversed_parts),
                              begin(subdomain_reversed_parts));
}

bool IsSubOrSuperDomain(const Domain& lhs, const Domain& rhs) {
    return lhs.GetPartCount() >= rhs.GetPartCount()
                  ? IsSubdomain(lhs, rhs)
                  : IsSubdomain(rhs, lhs);
}


class DomainChecker {
public:
    template <typename InputIt>
    DomainChecker(InputIt domains_begin, InputIt domains_end) {
        sorted_domains_.reserve(distance(domains_begin, domains_end));
        for (const Domain& domain : Range(domains_begin, domains_end)) {
            sorted_domains_.push_back(&domain);
        }
        sort(begin(sorted_domains_), end(sorted_domains_), IsDomainLess);
        sorted_domains_ = AbsorbSubdomains(move(sorted_domains_));
    }

    // Check if candidate is subdomain of some domain
    bool IsSubdomain(const Domain& candidate) const {
        const auto it = upper_bound(
                begin(sorted_domains_), end(sorted_domains_),
                &candidate, IsDomainLess);
        if (it == begin(sorted_domains_)) {
            return false;
        }
        return ::IsSubdomain(candidate, **prev(it));
    }

private:
    vector<const Domain*> sorted_domains_;

    static bool IsDomainLess(const Domain* lhs, const Domain* rhs) {
        const auto lhs_reversed_parts = lhs->GetReversedParts();
        const auto rhs_reversed_parts = rhs->GetReversedParts();
        return lexicographical_compare(
            begin(lhs_reversed_parts), end(lhs_reversed_parts),
            begin(rhs_reversed_parts), end(rhs_reversed_parts)
        );
    }

    static vector<const Domain*> AbsorbSubdomains(vector<const Domain*> domains) {
        domains.erase(
                unique(begin(domains), end(domains),
                              [](const Domain* lhs, const Domain* rhs) {
                                  return IsSubOrSuperDomain(*lhs, *rhs);
                              }),
                end(domains)
        );
        return domains;
    }
};


vector<Domain> ReadDomains(istream& in_stream = cin) {
    vector<Domain> domains;

    size_t count;
    in_stream >> count;
    domains.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        string domain_text;
        in_stream >> domain_text;
        domains.emplace_back(domain_text);
    }
    return domains;
}

vector<bool> CheckDomains(const vector<Domain>& banned_domains, const vector<Domain>& domains_to_check) {
    const DomainChecker checker(begin(banned_domains), end(banned_domains));

    vector<bool> check_results;
    check_results.reserve(domains_to_check.size());
    for (const Domain& domain_to_check : domains_to_check) {
        check_results.push_back(!checker.IsSubdomain(domain_to_check));
    }

    return check_results;
}

void PrintCheckResults(const vector<bool>& check_results, ostream& out_stream = cout) {
    for (const bool check_result : check_results) {
        out_stream << (check_result ? "Good" : "Bad") << "\n";
    }
}

void TestSimple() {
    string text = "one.two.three.four";
    {
        auto parts = Split(text, ".");
        vector<string_view> correct_parts = {"one", "two", "three", "four"};
        ASSERT_EQUAL(parts.size(), correct_parts.size());
        ASSERT_EQUAL(parts, correct_parts);
    }
    {
        Domain domain(text);
        vector<string_view> correct_parts = {"four", "three", "two", "one"};
        //vector<string_view> correct_parts = {"eno", "owt", "eerht", "ruof"};
        vector<string_view> parts;
        
        for(const auto & s : domain.GetReversedParts()) {
            parts.push_back(s);
        }
        ASSERT_EQUAL(parts.size(), correct_parts.size());
        ASSERT_EQUAL(parts, correct_parts);
    }
    {
        Domain domain(text);
        ASSERT_EQUAL(IsSubdomain(domain, domain), 1);
        //add method
        vector<Domain> domains;
        domains.push_back(domain);
        DomainChecker checker(domains.begin(), domains.end());
        ASSERT_EQUAL(checker.IsSubdomain(domain), 1);
    }
    {
        Domain domain1(text);
        string text2 = "three.four";
        Domain domain2(text2);
        ASSERT_EQUAL(IsSubdomain(domain1, domain2), 1);
        ASSERT_EQUAL(IsSubdomain(domain2, domain1), 0);
        //add method
        vector<Domain> domains1;
        domains1.push_back(domain1);
        DomainChecker checker1(domains1.begin(), domains1.end());
        ASSERT_EQUAL(checker1.IsSubdomain(domain2), 0);
        vector<Domain> domains2;
        domains2.push_back(domain2);
        DomainChecker checker2(domains2.begin(), domains2.end());
        ASSERT_EQUAL(checker2.IsSubdomain(domain1), 1);
    }
    {
        Domain domain1("ya.ru");
        Domain domain2("m.ya.ru");
        Domain domain3("pets.ya.ru");
        vector<Domain> domains;
        domains.push_back(domain1);
        domains.push_back(domain2);
        DomainChecker checker(domains.begin(), domains.end());
        ASSERT_EQUAL(checker.IsSubdomain(domain3), 1);
    }
    {
        Domain domain1("m.ya.com");
        Domain domain2("maps.ya.com");
        Domain domain3("ya.com");
        vector<Domain> domains;
        domains.push_back(domain1);
        domains.push_back(domain2);
        DomainChecker checker(domains.begin(), domains.end());
        ASSERT_EQUAL(checker.IsSubdomain(domain3), 0);
    }
    {
        vector<bool> v = {true, false};
        stringstream ss;
        PrintCheckResults(v, ss);
        string s;
        getline(ss, s);
        ASSERT_EQUAL(s, "Good");
        getline(ss, s);
        //ss >> s;
        ASSERT_EQUAL(s, "Bad");
    }
    {
        stringstream ss;
        ss << "2\n";
        ss << "ya.ru\n";
        ss << "yandex.ru\n";
        vector<Domain> domains = ReadDomains(ss);
        Domain domain1("ya.ru");
        Domain domain2("yandex.ru");
        ASSERT_EQUAL(domains.size(), 2);
        ASSERT_EQUAL(domains[0], domain1);
        ASSERT_EQUAL(domains[1], domain2);
    }
    {
        vector<Domain> banned_domains = {
                Domain("ya.ya"),
                Domain("ya.ru"),
                Domain("ya.com")
        };
        vector<Domain> good_domains = {
                Domain("haya.ya"),
                Domain("teya.ru"),
                Domain("suya.com"),
        };
        vector<Domain> bad_domains = {
                Domain("ha.ya.ya"),
                Domain("te.ya.ru"),
                Domain("su.ya.com"),
        };
        vector<bool> answer1 = {true, true, true};
        vector<bool> answer2 = {false, false, false};
        ASSERT_EQUAL(CheckDomains(banned_domains, good_domains), answer1);
        ASSERT_EQUAL(CheckDomains(banned_domains, bad_domains), answer2);
    }
    {
        Domain domain1("ya.ya");
        Domain domain2("ya.ru");
        Domain domain3("ya.com");
        Domain domain4("haya.ya");
        Domain domain5("teya.ru");
        Domain domain6("suya.com");
        Domain domain7("ha.ya.ya");
        Domain domain8("te.ya.ru");
        Domain domain9("su.ya.com");
        vector<Domain> domains;
        domains.push_back(domain1);
        domains.push_back(domain2);
        domains.push_back(domain3);
        DomainChecker checker(domains.begin(), domains.end());
        ASSERT_EQUAL(checker.IsSubdomain(domain4), 0);
        ASSERT_EQUAL(checker.IsSubdomain(domain5), 0);
        ASSERT_EQUAL(checker.IsSubdomain(domain6), 0);
        ASSERT_EQUAL(checker.IsSubdomain(domain7), 1);
        ASSERT_EQUAL(checker.IsSubdomain(domain8), 1);
        ASSERT_EQUAL(checker.IsSubdomain(domain9), 1);
    }
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSimple);

    return 0;
}
