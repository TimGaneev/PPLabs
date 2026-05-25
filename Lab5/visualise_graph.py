import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import os
import csv


#Чтение матрицы из файла
def read_matrix(size: int, file_name: str) -> list:
    with open("matrix_size_"+str(size)+"/"+file_name, "r", encoding="utf-8") as file:
        file.readline()
        matrix = list(map(int, file.read().split()))
        return [np.array_split(matrix, size), size]


#Нахождение среднего времени вычисления при разных размерах матриц
def process_data(base_data: pd.DataFrame, sizes: list[int]) -> pd.DataFrame:
    data = []
    for size in sizes:
        line = base_data[base_data["size"]==size].mean()
        data.append(line)
    return pd.DataFrame(data)


#Отображение обработанных данных на графике, сохранение графика
def display_graph(data_list: dict[int, pd.DataFrame], col: str, save_path: str) -> None:
    for proc in data_list.keys():
        plt.plot(data_list[proc]["size"], data_list[proc][" calculation time"],  label="processes: " + str(proc), marker="o")
    plt.grid(True)
    plt.xlabel("Matrix size")
    plt.ylabel(col)
    plt.legend()
    plt.savefig(save_path)
    plt.show()
    plt.clf()
    return


def main() -> None:
    try:
        #Проверка корректности вычислений
        sizes = [100, 200, 500, 1000, 2000]
        for size in sizes:
            matrix_a, size_a = read_matrix(size, "matrix_A.txt")
            matrix_b, size_b = read_matrix(size, "matrix_B.txt")
            my_result, size_r = read_matrix(size, "result.txt")
            correct_result = np.dot(matrix_a, matrix_b)
            if np.array_equal(my_result, correct_result):
              print("Size ", size, ": all good")
            else:
              print("Size ", size, ": smth aint right")

        #Обработка данных после перемножения матриц
        processes = [1, 2, 4, 8, 12]
        processed_data = {}
        for proc in processes:
          data = pd.read_csv(str(proc)+"_data.csv")
          processed_data[proc] = process_data(data, sizes)
        print(processed_data)
        display_graph(processed_data, "calculation time", "time_graph.jpg")
    except Exception as exc:
      print(f"Возникла ошибка: {exc}")


if __name__ == "__main__":
    main()
