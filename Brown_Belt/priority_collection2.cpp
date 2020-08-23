#include "test_runner.h"
#include <algorithm>
#include <list>
#include <iostream>
#include <iterator>
#include <memory>
#include <utility>
#include <map>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = size_t;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object) {
    Id id = current_id;
    ids_to_value[id] = move(object);
    ids_to_priority[id] = 0;
    priority_to_ids[0].push_back(id);
    sort(priority_to_ids[0].begin(), priority_to_ids[0].end());
    current_id++;
    return id;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin) {
      while(true) {
          if(range_begin == range_end) {
              break;
          }
          T obj = move(*(range_begin));
          *(ids_begin) = Add(move(obj));
          range_begin++;
          ids_begin++;
      }
  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const {
      if(ids_to_value.count(id) == 0) {
          return false;
      } else {
          return true;
      }
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const {
    return ids_to_value.at(id);
  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id) {
    size_t priority = ids_to_priority[id];
    ids_to_priority[id]++;
    priority_to_ids[priority].erase(lower_bound(priority_to_ids[priority].begin(), 
                                                priority_to_ids[priority].end(), 
                                                id));
    priority++;
    priority_to_ids[priority].push_back(id);
    sort(priority_to_ids[priority].begin(), priority_to_ids[priority].end());
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const {
    auto it = priority_to_ids.rbegin();
    Id id = *((*(it)).second.rbegin());
    return {ids_to_value.at(id), ids_to_priority.at(id)}; 
  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax() {
    pair<T, int> answer;
    auto it = priority_to_ids.rbegin();
    Id id = *((*(it)).second.rbegin());
    size_t priority = ids_to_priority[id];
    sort(priority_to_ids[priority].begin(), priority_to_ids[priority].end());
    priority_to_ids[priority].erase(lower_bound(priority_to_ids[priority].begin(), 
                                                priority_to_ids[priority].end(), 
                                                id));
    if(priority_to_ids[priority].size() == 0) {
        priority_to_ids.erase(prev(priority_to_ids.end()));
    }
    ids_to_priority.erase(id);
    answer.first = move(ids_to_value[id]);
    answer.second = priority;
    ids_to_value.erase(id);
    return answer;
  }

private:
  map<Id, T> ids_to_value;
  map<Id, size_t> ids_to_priority;
  map<size_t, vector<Id>> priority_to_ids;
  Id current_id = 0;
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
