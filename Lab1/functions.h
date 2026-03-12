#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

#include <iostream>
#include <random>
#include <ctime>
#include <fstream>
#include <filesystem>
#include "Matrix.h"

 
void setup_io()
{
#ifdef _MSC_VER
  system("chcp 65001 > nul");
#endif

  std::ios_base::sync_with_stdio(false);
}

//Генерация квадратной матрицы
Matrix generate_random_matrix(size_t size)
{
  std::random_device seed;
  std::default_random_engine e(seed());
  std::uniform_int_distribution d(-100, 100);

  Matrix m(size);
  for(int i = 0;i<size;i++)
  {
    for (int j = 0;j < size;j++)
    {
      m(i, j) = d(e);
    }
  }
  return m;
}

//Сохранение матрицы в файл
void save_matrix(std::filesystem::path file_name, const Matrix& m)
{
  std::filesystem::path directorypath = "../../../matrix_size_" + std::to_string(m.size());
  if (!std::filesystem::exists(directorypath))
  {
    std::filesystem::create_directory(directorypath);
  }
  std::ofstream file(directorypath / file_name);
  file << m.size() << "\n" << m;
}

//Чтение матрицы из файла
Matrix read_matrix(std::filesystem::path file_name, size_t size)
{
  std::filesystem::path directorypath = "../../../matrix_size_" + std::to_string(size);
  if (!std::filesystem::exists(directorypath / file_name)) { throw "file not found"; }

  std::ifstream file(directorypath / file_name);
  if (!file.is_open()) { throw "unable to open file"; }
  file >> size;
  Matrix m(size);

  for (int i = 0; i < size; i++)
  {
    for (int j = 0; j < size; j++)
    {
      file >> m(i, j);
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
Matrix multiply(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.size() != rhs.size()) { throw "Matrix sizes dont match"; }

  int operations = 0;
  std::clock_t start = clock();

  Matrix result(lhs.size());
  for (int i = 0; i < result.size();i++)
  {
    for (int j = 0; j < result.size();j++)
    {
      int temp = 0;
      for (int k = 0;k < result.size();k++)
      {
        temp += lhs(i, k) * rhs(k, j);
        operations++;
      }
      result(i, j) = temp;
    }
  }

  double duration = double(clock() - start) / CLOCKS_PER_SEC;

  std::filesystem::path directorypath = "../../..";
  std::fstream file(directorypath / "data.csv", std::ios::app);
  if (std::filesystem::is_empty(directorypath/"data.csv"))
  {
    file << "size," << "calculation time," << "total operations\n";
  }
  file << result.size() << ", " << duration << ", " << operations << "\n";
  return result;
}

#endif
