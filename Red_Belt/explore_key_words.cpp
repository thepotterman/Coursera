#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <functional>
#include <sstream>
#include <iostream>
#include <future>

using namespace std;

// Реализуйте шаблон класса Paginator

template <typename Iterator>
class IteratorRange
{
public:
	IteratorRange(Iterator begin, Iterator end)
	{
		_begin = begin;
		_end = end;
	}
	size_t size() const
	{
		return _end - _begin;
	}
	Iterator begin() const
	{
		return _begin;
	}
	Iterator end() const
	{
		return _end;
	}
private:
	Iterator _begin, _end;
};

template <typename Iterator>
class Paginator {
public:
	Paginator(Iterator begin, Iterator end, size_t page_size) {
		while(begin != end)
		{
			size_t range = end - begin;
			_pages.push_back(IteratorRange(begin, begin + min(range, page_size)));
			begin = _pages.back().end();	
		}
	}
	size_t size() const
	{
		return _pages.size();
	}
	auto end() const
	{
		return _pages.end();
	}
	auto begin() const
	{
		return _pages.begin();
	}
private:
	vector<IteratorRange<Iterator>> _pages;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
	return Paginator(c.begin(), c.end(), page_size);
}

struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for(auto it = other.word_frequences.begin(); it != other.word_frequences.end(); ++it) {
            this->word_frequences[it->first] += it->second;
        }
    }
};

//Stats ExploreLine(const set<string>& key_words, const string& line) {
//    Stats result;
//    stringstream ss(line);
//    string s;
//    while(ss >> s) {
//        if(key_words.count(s) != 0) {
//            ++result.word_frequences[move(s)];
//        }
//        s.clear();
//    }
//    return result;
//}

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, 
                                        IteratorRange<vector<string>::iterator> page) {
    Stats result;
    for(auto & s : page) {
        if(key_words.count(s) == 1) {
            ++result.word_frequences[s];
        }
    }
    return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
    // Реализуйте эту функцию
    Stats result;
    vector<string> strings;
    string s;
    int count = 4;
    while(input >> s) {
        strings.push_back(move(s));
        s.clear();
    }
//    auto pages = Paginate(strings, strings.size());
    auto pages = Paginate(strings, strings.size() / count);
    vector<future<Stats>> futures;
    for(auto page : pages) {
        futures.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), page));
    }
    for(int i = 0; i < futures.size(); ++i) {
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
