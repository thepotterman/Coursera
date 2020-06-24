#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    size_t range = range_end - range_begin;
    if(range < 2) {
        return;
    }
    vector<typename RandomIt::value_type> tmp(make_move_iterator(range_begin), 
                                              make_move_iterator(range_end));
    auto it1 = tmp.begin() + range / 3;
    auto it2 = tmp.begin() + 2 * range / 3;
    MergeSort(tmp.begin(), it1);
    MergeSort(it1, it2);
    MergeSort(it2, tmp.end());
    vector<typename RandomIt::value_type> temporary;
    merge(
            move_iterator<RandomIt>(tmp.begin()),
            move_iterator<RandomIt>(it1),
            move_iterator<RandomIt>(it1),
            move_iterator<RandomIt>(it2),
            back_inserter(temporary)
         );
    merge(
            move_iterator<RandomIt>(temporary.begin()),
            move_iterator<RandomIt>(temporary.end()),
            move_iterator<RandomIt>(it2),
            move_iterator<RandomIt>(tmp.end()),
            range_begin
         );

}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
