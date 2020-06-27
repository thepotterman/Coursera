#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <functional>
#include <sstream>
#include <iostream>
#include <future>

using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for(auto it = other.word_frequences.begin(); it != other.word_frequences.end(); ++it) {
            this->word_frequences[it->first] += it->second;
        }
    }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    stringstream ss(line);
    string s;
    while(ss >> s) {
        if(key_words.count(s) != 0) {
            ++result.word_frequences[move(s)];
        }
        s.clear();
    }
    return result;
}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
    Stats result;
    for (string line; getline(input, line); ) {
        result += ExploreLine(key_words, line);
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    // Реализуйте эту функцию
    vector<future<Stats>> futures;
    for(int i = 0; i < 4; ++i) {
        futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(input)));
    }
    Stats result;
    for(int i = 0; i < 4; ++i) {
        result += futures[i].get();
    }
    return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
