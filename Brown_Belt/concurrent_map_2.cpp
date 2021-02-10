#include "test_runner.h"
#include "profile.h"

#include <future>
#include <deque>
#include <mutex>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
using namespace std;

template <typename K, typename V, typename Hash = std::hash<K>>
class ConcurrentMap {
public:
    using MapType = unordered_map<K, V, Hash>;

    struct WriteAccess {
        lock_guard<mutex> guard;
        V& ref_to_value;
    };

    struct ReadAccess {
        lock_guard<mutex> guard;
        const V& ref_to_value;
    };

    explicit ConcurrentMap(size_t bucket_count) {
        _maps.resize(bucket_count);
        _mutexes.resize(bucket_count);
    }

    WriteAccess operator[](const K& key) {
        return {
            lock_guard(_mutexes[hasher(key) % _mutexes.size()]), 
            _maps[hasher(key) % _maps.size()][key]
        };
    }
    ReadAccess At(const K& key) const
    {
        return {
            lock_guard(_mutexes[hasher(key) % _mutexes.size()]), 
            _maps.at(hasher(key) % _maps.size()).at(key)
        };
    }

    bool Has(const K& key) const {
        lock_guard<mutex> guard(_mutexes[hasher(key) % _mutexes.size()]);       //Хочется закоментить, но пока не буду.
        return _maps.at(hasher(key) % _maps.size()).count(key);
        //return _maps.at(hasher(key) % _maps.size()).at(key).size();
    }

    MapType BuildOrdinaryMap() const {
        MapType answer;
        for(size_t i = 0; i < _maps.size(); ++i) {
            lock_guard<mutex> guard(_mutexes[i]);
            MapType _map = _maps[i];
            answer.merge(_map);
        }
        return answer;
    }

private:
    vector<MapType> _maps;
    //vector<unordered_map<K, V, Hash>> _maps;
    mutable deque<mutex> _mutexes;
    Hash hasher;
};

void RunConcurrentUpdates(
        ConcurrentMap<int, int>& cm, size_t thread_count, int key_count
) {
    auto kernel = [&cm, key_count](int seed) {
        vector<int> updates(key_count);
        iota(begin(updates), end(updates), -key_count / 2);
        shuffle(begin(updates), end(updates), default_random_engine(seed));

        for (int i = 0; i < 2; ++i) {
            for (auto key : updates) {
                cm[key].ref_to_value++;
            }
        }
    };

    vector<future<void>> futures;
    for (size_t i = 0; i < thread_count; ++i) {
        futures.push_back(async(kernel, i));
    }
}

void TestConcurrentUpdate() {
    const size_t thread_count = 3;
    const size_t key_count = 50000;

    ConcurrentMap<int, int> cm(thread_count);
    RunConcurrentUpdates(cm, thread_count, key_count);

    const auto result = std::as_const(cm).BuildOrdinaryMap();
    ASSERT_EQUAL(result.size(), key_count);
    for (auto& [k, v] : result) {
        AssertEqual(v, 6, "Key = " + to_string(k));
    }
}

void TestReadAndWrite() {
    ConcurrentMap<size_t, string> cm(5);

    auto updater = [&cm] {
        for (size_t i = 0; i < 50000; ++i) {
            cm[i].ref_to_value += 'a';
        }
    };
    auto reader = [&cm] {
        vector<string> result(50000);
        for (size_t i = 0; i < result.size(); ++i) {
            result[i] = cm[i].ref_to_value;
        }
        return result;
    };

    auto u1 = async(updater);
    auto r1 = async(reader);
    auto u2 = async(updater);
    auto r2 = async(reader);

    u1.get();
    u2.get();

    for (auto f : {&r1, &r2}) {
        auto result = f->get();
        ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
            return s.empty() || s == "a" || s == "aa";
        }));
    }
}

void TestSpeedup() {
    {
        ConcurrentMap<int, int> single_lock(1);

        LOG_DURATION("Single lock");
        RunConcurrentUpdates(single_lock, 4, 50000);
    }
    {
        ConcurrentMap<int, int> many_locks(100);

        LOG_DURATION("100 locks");
        RunConcurrentUpdates(many_locks, 4, 50000);
    }
}

void TestConstAccess() {
    const unordered_map<int, string> expected = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {31, "thirty one"},
        {127, "one hundred and twenty seven"},
        {1598, "fifteen hundred and ninety eight"}
    };

    const ConcurrentMap<int, string> cm = [&expected] {
        ConcurrentMap<int, string> result(3);
        for (const auto& [k, v] : expected) {
            result[k].ref_to_value = v;
        }
        return result;
    }();

    vector<future<string>> futures;
    for (int i = 0; i < 10; ++i) {
        futures.push_back(async([&cm, i] {
            try {
                return cm.At(i).ref_to_value;
            } catch (exception&) {
                return string();
            }
        }));
    }
    futures.clear();

    ASSERT_EQUAL(cm.BuildOrdinaryMap(), expected);
}

void TestStringKeys() {
    const unordered_map<string, string> expected = {
        {"one", "ONE"},
        {"two", "TWO"},
        {"three", "THREE"},
        {"thirty one", "THIRTY ONE"},
    };

    const ConcurrentMap<string, string> cm = [&expected] {
        ConcurrentMap<string, string> result(2);
        for (const auto& [k, v] : expected) {
            result[k].ref_to_value = v;
        }
        return result;
    }();

    ASSERT_EQUAL(cm.BuildOrdinaryMap(), expected);
}

struct Point {
    int x, y;
};

struct PointHash {
    size_t operator()(Point p) const {
        std::hash<int> h;
        return h(p.x) * 3571 + h(p.y);
    }
};

bool operator==(Point lhs, Point rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

void TestUserType() {
    ConcurrentMap<Point, size_t, PointHash> point_weight(5);

    vector<future<void>> futures;
    for (int i = 0; i < 1000; ++i) {
        futures.push_back(async([&point_weight, i] {
            point_weight[Point{i, i}].ref_to_value = i;
        }));
    }

    futures.clear();

    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQUAL(point_weight.At(Point{i, i}).ref_to_value, i);
    }

    const auto weights = point_weight.BuildOrdinaryMap();
    for (int i = 0; i < 1000; ++i) {
        ASSERT_EQUAL(weights.at(Point{i, i}), i);
    }
}

void TestHas() {
    ConcurrentMap<int, int> cm(2);
    cm[1].ref_to_value = 100;
    cm[2].ref_to_value = 200;

    const auto& const_map = std::as_const(cm);
    ASSERT(const_map.Has(1));
    ASSERT(const_map.Has(2));
    ASSERT(!const_map.Has(3));
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestConcurrentUpdate);
    RUN_TEST(tr, TestReadAndWrite);
    RUN_TEST(tr, TestSpeedup);
    RUN_TEST(tr, TestConstAccess);
    RUN_TEST(tr, TestStringKeys);
    RUN_TEST(tr, TestUserType);
    RUN_TEST(tr, TestHas);
}
