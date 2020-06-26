#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    using Id = size_t;
    
    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object) {
        _id_to_element[_current_id] = move(object);
        _id_to_priority[_current_id] = 0;
        _priority_to_id[0].insert(_current_id);
        _valid_id.insert(_current_id);
        ++_current_id;
        return _current_id - 1;
    }
    
    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end,
             IdOutputIt ids_begin) {
        for(; range_begin != range_end; ++range_begin) {
            _id_to_element[_current_id] = move(*(range_begin));
            _id_to_priority[_current_id] = 0;
            _priority_to_id[0].insert(_current_id);
            *(ids_begin) = _current_id;
            _valid_id.insert(_current_id);
            ++ids_begin;
            ++_current_id;
        }
    }
    
    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const {
        if(_valid_id.find(id) != _valid_id.end()) {
            return true;
        }
        return false;
    }
    
    // Получить объект по идентификатору
    const T& Get(Id id) const {
        return _id_to_element.at(id);
    }
    
    // Увеличить приоритет объекта на 1
    void Promote(Id id) {
        _priority_to_id[_id_to_priority[id]].erase(id);
        if(_priority_to_id[_id_to_priority[id]].size() == 0) {
            _priority_to_id.erase(_id_to_priority[id]);
        }
        ++_id_to_priority[id];
        _priority_to_id[_id_to_priority[id]].insert(id);
    }
    
    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const {
        auto it = _priority_to_id.end();
        --it;
        auto iit = it->second.end();
        --iit;
        Id id = *(iit);
        int priority = _id_to_priority.at(id);
        return {_id_to_element.at(id), priority};
    }
    
    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax() {
        auto it = _priority_to_id.end();
        --it;
        auto iit = it->second.end();
        --iit;
        Id id = *(iit);
        int priority = _id_to_priority[id];
        it->second.erase(iit);
        if(it->second.size() == 0) {
            _priority_to_id.erase(it);
        }
        T element = move(_id_to_element[id]);
        _id_to_element.erase(id);
        _id_to_priority.erase(id);
        _valid_id.erase(id);
        return {move(element), priority};
    }
    
private:
    // Приватные поля и методы
    map<int, set<Id>> _priority_to_id;
    map<Id, T> _id_to_element;
    map<Id, int> _id_to_priority;
    Id _current_id = 0;
    set<Id> _valid_id;
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
