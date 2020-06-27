#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <list>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using It = typename list<pair<T, int>>::iterator;
    using Id = int;
        // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        It it = _objects_with_priority.insert(_objects_with_priority.end(), {move(object), 0});
        _priority_with_id.insert({0, _current_id});
        _valid_id.insert(_current_id);
        _id_to_it[_current_id] = move(it);
        return _current_id++;
    }
    
    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        for(; range_begin != range_end; ++range_begin, ++ids_begin) {
            It it = _objects_with_priority.insert(_objects_with_priority.end(), 
                                                  {move(*(range_begin)), 0});
            _priority_with_id.insert({0, _current_id});
            _valid_id.insert(_current_id);
            _id_to_it[_current_id] = move(it);
            *(ids_begin) = _current_id++;
        }
    }
    
    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        if(_valid_id.count(id) == 0) {
            return false; 
        }
        return true;
    }
    
    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return _id_to_it.at(id)->first;
    }
    
    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        It it = _id_to_it[id];
        _priority_with_id.erase({it->second, id});
        ++it->second;
        _priority_with_id.insert({it->second, id});
    }
    
    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
        auto pos = prev(_priority_with_id.end());
        return *(_id_to_it.at(pos->second));
    }
    
    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        auto pos = prev(_priority_with_id.end());
        _valid_id.erase(pos->second);
        It it = _id_to_it[pos->second];
        auto answer = move(*(it));
        _id_to_it.erase(pos->second);
        _priority_with_id.erase(pos);
        return move(answer);
    }
    
private:
    // Приватные поля и методы
    list<pair<T, int>> _objects_with_priority;
    set<pair<int, Id>> _priority_with_id;           //???????????????????????????
    map<Id, It> _id_to_it;
    set<Id> _valid_id;
    Id _current_id = 0;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
