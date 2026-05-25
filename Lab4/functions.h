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
      *m(i, j) = d(e);
    }
  }
  return m;
}

//Сохранение матрицы в файл
void save_matrix(std::filesystem::path file_name, const Matrix& m)
{
  std::filesystem::path directorypath = "/content/drive/MyDrive/Colab Notebooks/Matrixes/matrix_size_" + std::to_string(m.size());
  if (!std::filesystem::exists(directorypath))
  {
    std::filesystem::create_directory(directorypath);
  }
  std::ofstream file(directorypath/file_name);
  file << m.size() << "\n" << m;
}

//Чтение матрицы из файла
Matrix read_matrix(std::filesystem::path file_name, size_t size)
{
  std::filesystem::path directorypath = "/content/drive/MyDrive/Colab Notebooks/Matrixes/matrix_size_" + std::to_string(size);
  if (!std::filesystem::exists(directorypath / file_name)) { throw "file not found"; }

  std::ifstream file(directorypath / file_name);
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

#define BLOCK_SIZE 20
__global__ void CudaMult(int* a, int* b, int size, int* c)
{
  int n = BLOCK_SIZE;
  int bx = blockIdx.x;
  int by = blockIdx.y;
  int tx = threadIdx.x;
  int ty = threadIdx.y;

  int sum = 0;
  for (int i = 0; i < size / n; i++)
  {
    __shared__ int asub[BLOCK_SIZE * BLOCK_SIZE];
    __shared__ int bsub[BLOCK_SIZE * BLOCK_SIZE];

    asub[ty * n + tx] = a[by * n * size + i * n + ty * size + tx];
    bsub[ty * n + tx] = b[bx * n + i * n * size + ty * size + tx];

    __syncthreads();
    for (int k = 0; k < n; k++)
    {
      sum += asub[ty * n + k] * bsub[k * n + tx];
    }
    __syncthreads();
  }

  c[(by * n + ty) * size + bx * n + tx] = sum;
}

Matrix multiply(const Matrix& lhs, const Matrix& rhs)
{
  if (lhs.size() != rhs.size()) { throw "Matrix sizes dont match"; }

  int size = lhs.size();
  int bytes_num = size * size * sizeof(int);

  Matrix result(size);

  cudaEvent_t start, stop;
  float time = 0.0f;
  cudaEventCreate(&start);
  cudaEventCreate(&stop);

  int* aDev, * bDev, * cDev;
  cudaMalloc((void**)&aDev, bytes_num);
  cudaMalloc((void**)&bDev, bytes_num);
  cudaMalloc((void**)&cDev, bytes_num);

  cudaEventRecord(start, 0);
  cudaMemcpy(aDev, &lhs(0, 0), bytes_num, cudaMemcpyHostToDevice);
  cudaMemcpy(bDev, &rhs(0, 0), bytes_num, cudaMemcpyHostToDevice);

  dim3 threads = dim3(BLOCK_SIZE, BLOCK_SIZE);
  dim3 blocks = dim3(size / threads.x, size / threads.y);

  CudaMult <<<blocks, threads >>> (aDev, bDev, size, cDev);
  cudaMemcpy(result(0, 0), cDev, bytes_num, cudaMemcpyDeviceToHost);
  cudaEventRecord(stop, 0);
  cudaEventSynchronize(stop);

  cudaEventElapsedTime(&time, start, stop);

  cudaFree(aDev);
  cudaFree(bDev);
  cudaFree(cDev);

  std::filesystem::path directorypath = "/content/drive/MyDrive/Colab Notebooks";
  std::fstream file(directorypath / (std::to_string(BLOCK_SIZE) + "_data.csv"), std::ios::app);
  if (std::filesystem::is_empty(directorypath / (std::to_string(BLOCK_SIZE) + "_data.csv")))
  {
    file << "size," << "calculation time\n";
  }
  file << result.size() << ", " << time << "\n";;
  return result;
  }

#endif
