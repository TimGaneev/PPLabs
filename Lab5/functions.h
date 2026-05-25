#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include <iostream>
#include <random>
#include <ctime>
#include <fstream>
#include <string>
#include "mpi.h"
#include "Matrix.h"

 
void setup_io()
{
#ifdef _MSC_VER
  system("chcp 65001 > nul");
#endif

  std::ios_base::sync_with_stdio(false);
}

//Генерация квадратной матрицы
// Matrix generate_random_matrix(size_t size)
// {
  // std::random_device seed;
  // std::default_random_engine e(seed());
  // std::uniform_int_distribution d(-100, 100);

  // Matrix m(size);
  // for(int i = 0;i<size;i++)
  // {
    // for (int j = 0;j < size;j++)
    // {
      // *m(i, j) = d(e);
    // }
  // }
  // return m;
// }

//Сохранение матрицы в файл
void save_matrix(std::string file_name, const Matrix& m)
{
  std::string directorypath = "matrix_size_" + std::to_string(m.size());
  std::ofstream file(directorypath + "/" + file_name);
  file << m.size() << "\n" << m;
}

//Чтение матрицы из файла
Matrix read_matrix(std::string file_name, size_t size)
{
  std::ifstream file("matrix_size_" + std::to_string(size) + "/" + file_name);
  if (!file.is_open()) { throw "unable to open file"; }
  file >> size;
  Matrix m(size);

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      file >> *m(i, j);
    }
  }
  return m;
}

/*
Перемножение матриц и сохранение
  размеров исходных/полученной матриц
  времени вычисления
  числа операций
*/
Matrix multiply(int argc, char* argv[], const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.size() != rhs.size()) { throw "Matrix sizes dont match"; }

  std::clock_t start = clock();
  double duration = 0;
  int size, rank, n = lhs.size(), i, j, k, temp;
  int* a = new int[n * n];
  int* b = new int[n * n];
  int rem;
  Matrix result(n);

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  rem = n % size;

  for (i = 0; i < n/size; i++)
  {
    for (j = 0; j < n; j++)
    {
      temp = 0;
      for (k = 0; k < n; k++)
      {
        temp += lhs(i + rank * (n / size), k) * rhs(k, j);
      }
      b[i * n + j] = temp;
    }
  }

  MPI_Gather(b, n * (n / size), MPI_INT, result(0, 0), n * (n / size), MPI_INT, 0, MPI_COMM_WORLD);
  if (rank == 0)
  {
    for(i = 0; i < rem; i++)
    {
      for (j = 0; j < n; j++)
      {
        temp = 0;
        for (k = 0; k < n; k++)
        {
          temp += lhs(n-rem+i, k) * rhs(k, j);
        }
        *result(n - rem + i, j) = temp;
      }
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);

  duration = double(clock() - start) / CLOCKS_PER_SEC;
  std::cout << result.size() << ", " << rank << ", " << size << ", " << duration << "\n";

  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0)
  {
   std::fstream file((std::to_string(size) + "_data.csv"), std::ios::app);
   file << result.size() << ", " << duration << "\n";
   save_matrix("result.txt", result);
  }

  MPI_Finalize();
  return result;
}

#endif
