#include <iostream>
#include "functions.h"
#include "Matrix.h"


using namespace std;

int main() {

  setup_io();

  try
  {
    int sizes[5] = { 100, 200, 500, 1000, 2000 };
    for (auto size : sizes)
    {
      Matrix a = read_matrix("matrix_A.txt", size);
      Matrix b = read_matrix("matrix_b.txt", size);
      Matrix c = multiply(a, b);
      save_matrix("result.txt", c);
      for (int i = 1; i < 5;i++)
      {
        multiply(a, b); //Многократное повторение вычислений для получения более точного *среднего* времени вычисления
      }
    }
  }
  catch (const char* e)
  {
    cerr << e;
  }
  return 0;
}
