#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>

using namespace std;

/*
�� �������� ����� ��������� �������, �������� ������� �������� ������������ �����, 
��������, ������ (threads), TCP-���������� ��� ����������� � ���� ������. 
�������, ����� ����� ������ ���������� �� �����, ��� �� ���������, 
� ����������� � �������, ����� ����� ����������������. 
� ���� ������ ��� ��������� ����������� ����� ����� � 
��� ���� �������� ������ ������ ObjectPool:

template <class T>
class ObjectPool {
public:
  T* Allocate();
  T* TryAllocate();

  void Deallocate(T* object);

  ~ObjectPool();

private:
  ...
};

������ ������ ObjectPool ������ ������������ ��� ������ ��������: ���������� � ������������, � 
���������� ��� ������ �����.

����� Allocate ������ �������� ���:

���� ���� ���� �� ���� ������������ ������, 
�� ��� ���� ��������� � ��������� ���������� � ������� 
��������� �� ���� � �������� ���������� �������

���� �� ������������ �������� ���, �� �������� ����� ������, 
���������� � ��������� ����������, � ��������� �� ���� ������������ � �������� ���������� �������

����� TryAllocate �������� ����������, ������ ���� ������������ �������� ���, 
�� ������ ������ ���������� nullptr.

����� Deallocate ��������� ������ �� ��������� ���������� � ��������� ������������; 
���� ���������� ������ �� ���������� � ��������� ����������, 
����� Deallocate ������ ������� ���������� invalid_argument.

������ Allocate � TryAllocate ������ ���������� ������� � ������� FIFO, 
�.�. ��������� ������������ �������� ������ ������������ ����� �������: 
������ [Try]Allocate ������ ��������� ������� �� � ������, 
� ����� Deallocate ������ �������� ������������� ������ � � �����.

���������� ������� ObjectPool ������ ���������� ��� ������� ����, ��� ����������, ��� � ������������.

*/

template <class T>
class ObjectPool {
public:
    T* Allocate() {
        T* result;
        if(freed.size() != 0) {
            result = freed.front();
            freed.pop();
            alloted.insert(result);
        } else {
            result = new T;
            alloted.insert(result);
        }
        return result;
    }
    
    T* TryAllocate() {
        T* result = nullptr;
        if(freed.size() != 0) {
            result = freed.front();
            freed.pop();
            alloted.insert(result);
        }
        return result;
    }
    
    void Deallocate(T* object) {
        if(alloted.count(object) == 0) {
            throw invalid_argument("IA");
            return;
        }
        freed.push(object);
        alloted.erase(object);
    }
    
    ~ObjectPool() {
        while(freed.size() > 0) {
            T* cur = freed.front();
            freed.pop();
            delete cur;    
        }
        for(T* cur : alloted) {     
            delete cur;
        }
        alloted.clear();
    }
    
private:
      queue<T*> freed;
      set<T*> alloted;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
