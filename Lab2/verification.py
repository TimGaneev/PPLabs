import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


#Чтение матрицы из файла
def read_matrix(size: int, file_name: str) -> list:
    with open("matrix_size_"+str(size)+"/"+file_name, "r", encoding="utf-8") as file:
        file.readline()
        matrix = list(map(int, file.read().split()))
        return [np.array_split(matrix, size), size]


#Нахождение среднего времени вычисления при разных размерах матриц
def process_data(base_data: pd.DataFrame, sizes: list[int], threads: int) -> pd.DataFrame:
    data = []
    for size in sizes:
        line = base_data[(base_data["size"]==size) & (base_data["threads"]==threads)].mean()
        data.append(line)
    return pd.DataFrame(data)


#Отображение обработанных данных на графике, сохранение графика
def display_graph(data_list: list[pd.DataFrame], save_path: str) -> None:
    for data in data_list:
      plt.plot(data["size"], data["calculation time"], label="threads: " + str(int(data["threads"][0])), marker="o")
    plt.grid(True)
    plt.xlabel("Matrix size")
    plt.ylabel("calculation time")
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
        threads = [1, 2, 4, 8, 10]
        data = pd.read_csv("data.csv")
        processed_data = []
        for threads_num in threads:
            processed_data.append(process_data(data, sizes, threads_num))
        #print(processed_data)
        display_graph(processed_data, "time_graph.jpg")
    except Exception as exc:
      print(f"Возникла ошибка: {exc}")


if __name__ == "__main__":
    main()
