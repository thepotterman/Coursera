#include "test_runner.h"

#include <iostream>
#include <map>
#include <list>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

// Реализуйте этот класс
class Database {
public:

    struct Iters {
        multimap<int, list<Record>::iterator>::iterator it1;    //Time
        multimap<int, list<Record>::iterator>::iterator it2;    //Karma
        multimap<string, list<Record>::iterator>::iterator it3; //User
//        list<Record>::iterator it4;
    };

    bool Put(const Record& record) {
        if(id_to_it.count(record.id) == 1) {
            return false;
        }
        data.push_back(move(record));
        auto iterator = prev(data.end());
        Iters it;
        it.it1 = time_to_data.insert({iterator->timestamp, iterator});
        it.it2 = karma_to_data.insert({iterator->karma, iterator});
        it.it3 = user_to_data.insert({iterator->user, iterator});
//      it.it4 = iterator;
        id_to_it[iterator->id] = move(it);
        return true;
    }
    const Record* GetById(const string& id) const {
        if(id_to_it.count(id) == 0) {
            return nullptr;
        }
        return &*(id_to_it.at(id).it1->second);
    }
    bool Erase(const string& id) {
        if(id_to_it.count(id) == 0) {
            return false;
        }
        Iters it = id_to_it[id];
        data.erase(it.it1->second);
        time_to_data.erase(it.it1);
        karma_to_data.erase(it.it2);
        user_to_data.erase(it.it3);
        id_to_it.erase(id);
        return true;
    }
    
    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
        auto begin = time_to_data.lower_bound(low);
        auto end = time_to_data.upper_bound(high);
        while(begin != end) {
            if(callback(*begin->second) == 0) {
                return;
            }
            ++begin;
        }
    }
    
    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
        auto begin = karma_to_data.lower_bound(low);
        auto end = karma_to_data.upper_bound(high);
        while(begin != end) {
            if(callback(*begin->second) == 0) {
                return;
            }
            ++begin;
        }
    }
    
    template <typename Callback>
    void AllByUser(const string& user, Callback callback) const {
        auto range = user_to_data.equal_range(user);
        for(auto i = range.first; i != range.second; ++i) {
            if(callback(*i->second) == 0) {
                return;
            }
        }
    }
    
private:
    map<string, Iters> id_to_it;
    multimap<int, list<Record>::iterator> time_to_data;
//    multimap<int, Record> time_to_data;
    multimap<int, list<Record>::iterator> karma_to_data;
//    multimap<int, Record> karma_to_data;
    multimap<string, list<Record>::iterator> user_to_data;
//    multimap<string, Record> user_to_data;
    list<Record> data;
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}
