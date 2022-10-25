import math
import random
import matplotlib.pyplot


class Matrix:
    def __init__(self):
        self.matrix = list()
        self.row_count = 0
        self.col_count = 0

    def initialize(self, array: list) -> None:
        self.matrix = array
        self.row_count = array.__len__()
        self.col_count = array[0].__len__()

    def read_from_csv(self, filename: str = "test.csv"):
        self.matrix = list()
        with open(filename, "r") as file:
            lines = file.readlines()
        for line in lines:
            split_line = line.strip().split(",")
            self.matrix.append(list(map(lambda x: float(x), split_line)))
        self.row_count = self.matrix.__len__()
        self.col_count = self.matrix[0].__len__()

    def add_ones_column(self):
        self.col_count += 1
        for i in range(0, self.row_count):
            self.matrix[i].append(1)

    def __getitem__(self, item):
        if type(item) == list:
            if item[0] == "back" and item[1] == "back":
                return self.matrix[self.row_count - 1][self.col_count - 1]
            elif item[0] == "back":
                return self.__getitem__([self.row_count - 1, item[1]])
            elif item[1] == "back":
                return self.__getitem__([item[0], self.col_count - 1])
            else:
                return self.matrix[item[0]][item[1]]
        else:
            if type(item) == int:
                return self.matrix[item]
            elif type(item) == str and item == "back":
                return self.matrix[self.row_count - 1]
            else:
                print("[Warning] Getting the first row of the matrix.")
                return self.matrix[0]

    @property
    def __len__(self) -> list:
        return [self.row_count, self.col_count]

    @property
    def doc(self) -> str:
        return "Matrix"


def sub_matrix(array: list, m, n) -> list:
    order = array.__len__()
    res = list()
    for i in range(0, order):
        if i != m:
            current_row = list()
            for j in range(0, order):
                if j != n:
                    current_row.append(array[i][j])
            res.append(current_row)
    return res


def determinant(array: list) -> float:
    order = array.__len__()
    assert order == array[0].__len__()
    if order == 1:
        return array[0][0]
    elif order == 2:
        return array[0][0] * array[1][1] - array[0][1] * array[1][0]
    else:
        res, row_count = 0, 0
        for i in array:
            res += i[0] * determinant(sub_matrix(array, row_count, 0)) * (-1) ** (row_count % 2)
            row_count += 1
        return res


def transpose(array: list) -> Matrix:
    row_count, col_count = array[0].__len__(), array.__len__()
    res_array = [[0.0 for _ in range(0, col_count)] for _ in range(0, row_count)]
    for i in range(0, row_count):
        for j in range(0, col_count):
            res_array[i][j] = array[j][i]
    res = Matrix()
    res.initialize(res_array)
    return res


def multiply(array1: list, array2: list) -> Matrix:
    row_count, mid_count, col_count = array1.__len__(), array2.__len__(), array2[0].__len__()
    assert mid_count == array1[0].__len__()
    res_array = [[0 for _ in range(0, col_count)] for _ in range(0, row_count)]
    for i in range(0, row_count):
        for j in range(0, col_count):
            for k in range(0, mid_count):
                res_array[i][j] += array1[i][k] * array2[k][j]
    res = Matrix()
    res.initialize(res_array)
    return res


def inverse_matrix(array: list) -> Matrix:
    order = array.__len__()
    res_array = [[0.0 for _ in range(0, order)] for _ in range(0, order)]
    det = determinant(array)
    for i in range(0, order):
        for j in range(0, order):
            res_array[i][j] = (-1) ** (j % 2) * determinant(sub_matrix(array, i, j)) / det
    res = Matrix()
    res.initialize(res_array)
    return res


class MultipleLinearRegression:
    def __init__(self, train_X: Matrix, train_y: Matrix):
        self.sample_X = train_X
        self.sample_y = train_y
        self.coefficient_matrix = Matrix()
        self.calc_coefficient()

    def calc_coefficient(self) -> None:
        transpose_matrix = transpose(self.sample_X.matrix).matrix
        tmp = inverse_matrix(multiply(transpose_matrix, self.sample_X.matrix).matrix).matrix
        self.coefficient_matrix = multiply(multiply(tmp, transpose_matrix).matrix, self.sample_y.matrix)

    def predict(self, input_x: list) -> float:
        input_x_m = Matrix()
        input_x_m.initialize([input_x])
        return multiply(input_x_m.matrix, self.coefficient_matrix.matrix)[0][0]


class LinearRegressionModel:
    def __init__(self, filename: str = "test.csv"):
        self.input_X = Matrix()
        self.input_y = Matrix()
        self.train_X = Matrix()
        self.train_y = Matrix()
        self.test_X = Matrix()
        self.test_y = Matrix()
        self.case_count = 0
        self.training_case_count = 0
        self.mse = 0.0
        self.rmse = 0.0
        self.test_y_var = 0.0
        self.r_square = 0.0
        self.report = list()

        self.read_from_csv(filename)
        self.regression_train = None
        self.split_train_test(training_cases_ratio=0.8)

    def read_from_csv(self, filename: str) -> bool:
        self.input_X.matrix = list()
        self.input_y.matrix = list()
        try:
            with open(filename, "r") as file:
                lines = file.readlines()
            for line in lines:
                split_line = line.strip().split(",")
                self.input_X.matrix.append(list(map(lambda x: float(x), split_line[0:-1])))
                self.input_y.matrix.append([float(split_line[-1])])
            self.input_X.row_count = self.input_X.matrix.__len__()
            self.input_X.col_count = self.input_X.matrix[0].__len__()
            self.input_y.row_count = self.input_y.matrix.__len__()
            self.input_y.col_count = 1
            self.input_X.add_ones_column()
        except FileNotFoundError:
            print("[Error] File not found!")
            return False
        except ValueError:
            print("[Error] Data invalid!")
            return False
        return True

    def split_train_test(self, training_cases_ratio: float = 0.8):
        tmp = list(zip(self.input_X, self.input_y))
        self.case_count = tmp.__len__()
        random.shuffle(tmp)
        self.training_case_count = int(self.case_count * training_cases_ratio)
        if self.training_case_count < 50:
            print("[Warning] Training cases less than 50!")

        self.input_X.matrix = list(map(lambda x: x[0], tmp))
        self.input_y.matrix = list(map(lambda x: x[1], tmp))
        self.train_X.initialize(self.input_X.matrix[:self.training_case_count])
        self.train_y.initialize(self.input_y.matrix[:self.training_case_count])
        self.test_X.initialize(self.input_X.matrix[self.training_case_count:])
        self.test_y.initialize(self.input_y.matrix[self.training_case_count:])
        self.regression_train = MultipleLinearRegression(self.train_X, self.train_y)

    def test_case_predict(self):
        for i in zip(self.test_X, self.test_y):
            self.report.append([i[1][0], self.regression_train.predict(i[0])])
        self.get_rmse()
        self.get_r_square()
        self.report_prediction()

    def get_rmse(self):
        self.mse, self.rmse = 0.0, 0.0
        for i in range(0, self.case_count - self.training_case_count):
            self.mse += (self.report[i][1] - self.report[i][0]) ** 2
        self.mse /= self.training_case_count
        self.rmse = math.sqrt(self.mse)

    def get_r_square(self):
        test_y_avg, self.test_y_var, self.r_square = 0.0, 0.0, 0.0
        for i in range(0, self.case_count - self.training_case_count):
            test_y_avg += self.report[i][1]
        test_y_avg /= (self.case_count - self.training_case_count)
        for i in range(0, self.case_count - self.training_case_count):
            self.test_y_var += (self.report[i][1] - test_y_avg) ** 2
        self.test_y_var /= (self.case_count - self.training_case_count)
        self.r_square = 1 - self.mse / self.test_y_var

    def report_prediction(self):
        print("[Coefficients]", self.regression_train.coefficient_matrix.matrix)
        print("[A sample (Real vs Predict)]", self.report[0])
        print("[MSE]", self.mse)
        print("[RMSE]", self.rmse)
        print("[R square]", self.r_square)
        if self.regression_train.coefficient_matrix.matrix.__len__() == 2:
            print("[Plot for 2-D]")
            self.plot_result()

    def plot_result(self):
        x = [i for i in range(0, 80)]
        k = self.regression_train.coefficient_matrix[0][0]
        b = self.regression_train.coefficient_matrix[1][0]
        y = [k * x[i] + b for i in range(0, 80)]
        matplotlib.pyplot.scatter([i[0] for i in self.input_X.matrix], self.input_y.matrix)
        matplotlib.pyplot.plot(x, y, color="orange")
        matplotlib.pyplot.title("Prediction")
        matplotlib.pyplot.show()


if __name__ == "__main__":
    linear_regression_model = LinearRegressionModel("test2.csv")
    linear_regression_model.test_case_predict()
