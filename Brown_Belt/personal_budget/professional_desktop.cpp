#include <iostream>
#include <ctime>
#include <unordered_map>

using namespace std;

const string kComputeIncome = "ComputeIncome";
const string kEarn = "Earn";
const string kPayTax = "PayTax";
const string kSpend = "Spend";

struct Date {
    int year_;
    int month_;
    int day_;
    time_t AsTimestamp() const {
        std::tm t;
        t.tm_sec   = 0;
        t.tm_min   = 0;
        t.tm_hour  = 0;
        t.tm_mday  = day_;
        t.tm_mon   = month_ - 1;
        t.tm_year  = year_ - 1900;
        t.tm_isdst = 0;
        return mktime(&t);
    }
};

istream& operator>>(istream& in, Date& date) {
    in >> date.year_;
    in.ignore(1);
    in >> date.month_;
    in.ignore(1);
    in >> date.day_;
    return in;
}

int ComputeDaysDiff(const Date& date_from, const Date& date_to) {
    const time_t timestamp_to = date_to.AsTimestamp();
    const time_t timestamp_from = date_from.AsTimestamp();
    static const int SECONDS_IN_DAY = 60 * 60 * 24;
    return (timestamp_to - timestamp_from) / SECONDS_IN_DAY;
}

size_t GetIndex(const Date& date) {
    Date begin;
    begin.year_ = 2000;
    begin.month_ = 1;
    begin.day_ = 1;
    return static_cast<size_t>(ComputeDaysDiff(begin, date));
}

struct MoneyPerDay {
    double earned = 0;
    double spend = 0;
};

class PersonalBudget {
public:
    void ComputeIncome(const Date& from, const Date& to) {
        double answer = 0;
        size_t index = GetIndex(from);
        const size_t end = GetIndex(to);
        while(index <= end) {
            answer += budget_[index].earned;
            answer -= budget_[index].spend;
            index++;
        }
        cout << answer << endl;
    }
    void Earn(const Date& from, const Date& to, const double value) {
        double daily_value = value / static_cast<double>(ComputeDaysDiff(from, to) + 1);
        size_t index = GetIndex(from);
        const size_t end = GetIndex(to);
        while(index <= end) {
            budget_[index].earned += daily_value;
            index++;
        }
    }
    void PayTax(const Date& from, const Date& to, const double tax) {
        size_t index = GetIndex(from);
        const size_t end = GetIndex(to);
        while(index <= end) {
            budget_[index].earned *= ((100.0 - tax) / 100.0);
            index++;
        }
    }
    void Spend(const Date& from, const Date& to, const double value) {
        double daily_value = value / static_cast<double>(ComputeDaysDiff(from, to) + 1);
        size_t index = GetIndex(from);
        const size_t end = GetIndex(to);
        while(index <= end) {
            budget_[index].spend += daily_value;
            index++;
        }
    }
private:
    unordered_map<size_t, MoneyPerDay> budget_;
};

int main(void) {
    cin.tie(nullptr);
    std::ios_base::sync_with_stdio(false);
    cout.precision(25);
    size_t q = 0;
    cin >> q;
    PersonalBudget budget;
    for(size_t i = 0; i < q; ++i) {
        string request;
        Date from;
        Date to;
        cin >> request >> from >> to;
        if(request == kComputeIncome) {
            budget.ComputeIncome(from, to);
        } else if(request == kEarn) {
            double value = 0;
            cin >> value;
            budget.Earn(from, to, value);
        } else if(request == kPayTax) {
            double tax = 0;
            cin >> tax;
            budget.PayTax(from, to, tax);
        } else if(request == kSpend) {
            double value = 0;
            cin >> value;
            budget.Spend(from, to, value);
        }
    }
    return 0;
}
