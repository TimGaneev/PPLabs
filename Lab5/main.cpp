#include <iostream>
#include "functions.h"
#include "Matrix.h"


using namespace std;

int main(int argc, char* argv[]) {

  setup_io();

  try
  {
    if (argc < 2) { exit(0); }
    int size = stoi(argv[1]);
    Matrix a = read_matrix("matrix_A.txt", size);
    Matrix b = read_matrix("matrix_b.txt", size);
    Matrix c(b.size());
    c = multiply(argc, argv, a, b);
  }
  catch (const char* e)
  {
    cerr << e;
  }
  return 0;
}
