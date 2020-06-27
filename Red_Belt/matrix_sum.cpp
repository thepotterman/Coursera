#include "test_runner.h"
#include <vector>
#include <future>
#include <algorithm>
#include <numeric>

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

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
    // Реализуйте эту функцию
    auto pages = Paginate(matrix, matrix.size() / 4);
    int64_t sum = 0;
    for(auto & page : pages) {
        for(auto & row : page) {
            sum += accumulate(row.begin(), row.end(), 0);
        }
    }
    return sum;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
