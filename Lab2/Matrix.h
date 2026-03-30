#ifndef MATRIX_HEADER_H
#define MATRIX_HEADER_H

#include <iostream>

class Matrix {
//Класс квадратная матрица для удобства дальнейшей работы с ними
private:
  int* _elems = nullptr;
  size_t _size = 0;
public:
  Matrix() = default;
  Matrix(size_t size) : _size(size)
  {
    _elems = new int[size*size];
    for (size_t i = 0;i < _size;i++) {
      for (size_t j = 0;j < _size;j++) {
        _elems[i * _size + j] = 0;
      }
    }
  }
  Matrix(const Matrix& other)
  {
    for (size_t i = 0;i < _size;i++) {
      for (size_t j = 0;j < _size;j++) {
        _elems[i * _size + j] = other._elems[i * _size + j];
      }
    }
  }
  ~Matrix()
  {
    delete[] _elems;
  }
  Matrix& operator=(const Matrix& other)
  {
    for (size_t i = 0;i < _size;i++) {
      for (size_t j = 0;j < _size;j++) {
        _elems[i * _size + j] = other._elems[i * _size + j];
      }
    }
    return *this;
  }

  int& operator()(size_t i, size_t j)
  {
    return _elems[i * _size + j];
  }
  int& operator()(size_t i, size_t j) const
  {
    return _elems[i * _size + j];
  }
  size_t size() const
  {
    return _size;
  }
};

std::ostream& operator<<(std::ostream& os, const Matrix& m)
{
  for (size_t i = 0; i < m.size();i++)
  {
    for (size_t j = 0; j < m.size();j++)
    {
      os << m(i, j) << ' ';
    }
    os << '\n';
  }
  return os;
}
#endif
