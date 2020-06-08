#pragma once

#include <cstdlib>
#include <algorithm>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
    SimpleVector() {
        _vector = nullptr;
        _size = 0;
        _capacity = 0;
    }

    explicit SimpleVector(size_t size) {
    _vector = new T[size * 2];
    _size = size;
    _capacity = size * 2;
}

    ~SimpleVector() {
    delete[] _vector;
}

    
    T& operator[](size_t index) {
    return _vector[index];
}

    T* begin() {
    return _vector;
}

    T* end() {
    return _vector + _size;
}
    
    size_t Size() const {
    return _size;
}

    size_t Capacity() const {
    return _capacity;
}

    void PushBack(const T& value) {
    if(_size < _capacity) {
        _vector[_size] = value;
        _size++;
    } else if(_capacity != 0) {
        T* tmp = new T[_capacity * 2];
        std::copy(_vector, _vector + _size, tmp);
        delete[] _vector;
        _vector = tmp;
        _vector[_size] = value;
        ++_size;
        _capacity *= 2; 
    } else {
        T* tmp = new T[1];
        tmp[0] = value;
        _vector = tmp;
        _size = 1;
        ++_capacity = 1;
    }
}

private:
    T* _vector;
    size_t _size;
    size_t _capacity;
};
