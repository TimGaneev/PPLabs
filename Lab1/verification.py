import numpy as np
import pandas as pd
import matplotlib.pyplot as plt


def read_matrix(size: int, file_name: str) -> list:
    with open("matrix_size_"+str(size)+"/"+file_name, "r", encoding="utf-8") as file:
        file.readline()
        matrix = list(map(int, file.read().split()))
        return [np.array_split(matrix, size), size]


def process_data(base_data: pd.DataFrame, sizes: list[int]) -> pd.DataFrame:
    data = []
    for size in sizes:
        line = base_data[base_data["size"]==size].mean()
        if line["total operations"] < 0:
            line["total operations"] = None
        data.append(line)
    return pd.DataFrame(data)


def display_graph(data: pd.DataFrame, col: str, save_path: str) -> None:
    plt.plot(data["size"], data[col], marker="o")
    plt.grid(True)
    plt.xlabel("Matrix size")
    plt.ylabel(col)
    plt.savefig(save_path)
    #plt.show()
    plt.clf()
    return


def main() -> None:
    try:
        sizes = [100, 200, 500, 1000, 2000]
        # for size in sizes:
        #   matrix_a, size_a = read_matrix(size, "matrix_A.txt")
        #   matrix_b, size_b = read_matrix(size, "matrix_B.txt")
        #   my_result, size_r = read_matrix(size, "result.txt")
        #   correct_result = np.dot(matrix_a, matrix_b)
        #   if np.array_equal(my_result, correct_result):
        #     print(size, ": all good")
        #   else:
        #     print(size, ": smth aint right")
        data = pd.read_csv("data.csv")
        processed_data = process_data(data, sizes)
        print(processed_data)
        display_graph(processed_data, "total operations", "operations_graph.jpg")
        display_graph(processed_data, "calculation time", "time_graph.jpg")
    except Exception as exc:
      print(f"Возникла ошибка: {exc}")


if __name__ == "__main__":
    main()
