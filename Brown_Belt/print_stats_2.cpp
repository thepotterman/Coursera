#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
        : first(begin)
        , last(end)
    {
    }

    Iterator begin() const {
        return first;
    }

    Iterator end() const {
        return last;
    }

private:
    Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
    return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
    string name;
    int age, income;
    bool is_male;
};

struct Data {
    vector<Person> peoples;
    vector<string> names;               // 0 - m; 1 - f;
    vector<int64_t> incomes;
};


int main() {
    const Data people = []{
        Data answer;
        int count;
        cin >> count;

        vector<Person> data(count);
        vector<string> names(2);
        vector<int64_t> incomes;
        for(Person& p : data) {
            char gender;
            cin >> p.name >> p.age >> p.income >> gender;
            p.is_male = gender == 'M';
        }

        sort(begin(data), end(data), [](const Person& lhs, const Person& rhs) {
            return lhs.income > rhs.income;
        });
        int64_t total;
        for(int i = 0; i < count; ++i) { 
            total += data[i].income;
            incomes.push_back(total);
        }

        
        auto it = partition(data.begin(), data.end(), [](const Person& p){
            return p.is_male == 1;
        });
        IteratorRange range(data.begin(), it);
        sort(range.begin(), range.end(), [](const Person& lhs, const Person& rhs){
            return lhs.name < rhs.name;
        });
        string best = "";
        string current = "";
        int best_count = 0;
        int current_count = 0;
        for(const Person& p : range) {
            if(p.name != current) {
                current = p.name;
                current_count = 1;
            } else {
                ++current_count;
            }
            if(current_count > best_count) {
                best = current;
                best_count = current_count;
            }
        }
        names[0] = best;


        //range.begin = range.end();
        //range.end = data.end();
        IteratorRange range1(it, data.end());
        sort(range1.begin(), range1.end(), [](const Person& lhs, const Person& rhs){
            return lhs.name < rhs.name;
        });
        best = "";
        current = "";
        best_count = 0;
        current_count = 0;
        for(const Person& p : range1) {
            if(p.name != current) {
                current = p.name;
                current_count = 1;
            } else {
                ++current_count;
            }
            if(current_count > best_count) {
                best = current;
                best_count = current_count;
            }
        }
        names[1] = best;



        sort(begin(data), end(data), [](const Person& lhs, const Person& rhs) {
            return lhs.age < rhs.age;
        });
        answer.peoples = move(data);
        answer.names = move(names);
        answer.incomes = move(incomes);
        return answer;
    }();


    for (string command; cin >> command; ) {
        if (command == "AGE") {
            int adult_age;
            cin >> adult_age;

            auto adult_begin = lower_bound(
                begin(people.peoples), end(people.peoples), adult_age, [](const Person& lhs, int age) {
                    return lhs.age < age;
                }
            );

            cout << "There are " << std::distance(adult_begin, end(people.peoples))
                      << " adult people for maturity age " << adult_age << '\n';
        } else if (command == "WEALTHY") {
            int count;
            cin >> count;
/*
            auto head = Head(people, count);

            partial_sort(
                head.begin(), head.end(), end(people), [](const Person& lhs, const Person& rhs) {
                    return lhs.income > rhs.income;
                }
            );

            int total_income = accumulate(
                head.begin(), head.end(), 0, [](int cur, Person& p) {
                    return p.income += cur;
                }
            );
*/            
            cout << "Top-" << count << " people have total income " << people.incomes[count - 1] << '\n';
        } else if (command == "POPULAR_NAME") {
            char gender;
            cin >> gender;
            int is = gender == 'W';
            if(people.names[is].size() == 0) {
                cout << "No people of gender " << gender << '\n';
            } else {
                cout << "Most popular name among people of gender " << gender << " is "
                          << people.names[is] << '\n';
            }
        }
    }
}
