/***************************************************************************
**                                                                        **
**  Matrix - класс для работы с двумерными матрицами                      **
**                                                                        **
****************************************************************************
**            Версия: 2.0.1                                               **
**              Дата: 18.04.2017                                          **
**  Список изменений:                                                     **
**      1. Переработаны классы для работы с исключениями.                 **
****************************************************************************
**    Прошлые версии:                                                     **
****************************************************************************
**            Версия: 2.0.0                                               **
**              Дата: 15.12.2016                                          **
**  Список изменений:                                                     **
**      1. Класс сделан шаблонным.                                        **
**      2. Добавлено приведение к базовому типу матрицы.                  **
**      3. Удален метод приведения к типу bool                            **
**          (остался в версии no_template).                               **
**      4. Добавлена операция деления матрицы на элемент базового типа.   **
****************************************************************************
**            Версия: 1.5.0                                               **
**              Дата: 03.12.2016                                          **
**  Список изменений:                                                     **
**      1. Добавлена поддержка операций перемещения                       **
**          (конструктор и присваивание).                                 **
**      2. Добавлены исключения.                                          **
**      3. Доработаны прототипы некоторых методов и                       **
**          дружественных функций.                                        **
**      4. Добавлено приведение к типу bool.                              **
**      5. Добавлены комментарии, назначения функций.                     **
**      6. Изменен стиль именования                                       **
**          а) Нижние подчеркивания                                       **
**          б) "тип* имя" вместо "тип *имя"                               **
***************************************************************************/

#pragma once
#include <iostream>
#include <string>
#include <exception>

template<typename T>
class Matrix
{
private:
    T** matrix_;
    int row_;
    int col_;
public:
    // Классы для работы с исключениями
    class Index_error : public std::out_of_range
    {
    private:
        int row_;
        int col_;
    public:
        Index_error(int row, int col) :
            std::out_of_range("invalid matrix index"/*: (" + std::to_string(row) + "," + std::to_string(col) + ")"*/),
            row_(row), col_(col) {}

        int row() const { return row_; }
        int col() const { return col_; }
    };

    class Size_error : public std::range_error
    {
    private:
        int row1_;
        int col1_;
        int row2_;
        int col2_;
    public:
        Size_error(int row1, int col1, int row2, int col2) :
            std::range_error("incorrect size of the matrix"),
            row1_(row1), col1_(col1), row2_(row2), col2_(col2) {}

        int row1() { return row1_; }
        int col1() { return col1_; }
        int row2() { return row2_; }
        int col2() { return col2_; }
    };

    // Конструкторы и деструктор
    Matrix() = delete;
    Matrix(int row, int col);
    Matrix(const Matrix<T>& matr);
    Matrix(Matrix<T>&& matr);
    ~Matrix();

    // Методы
    inline int row() const { return row_; }
    inline int col() const { return col_; }
    const T& operator()(int row, int col) const;
    T& operator()(int row, int col);

    Matrix<T>& operator=(const Matrix<T>& matr);
    Matrix<T>& operator=(Matrix<T>&& matr);

    //operator bool() const;    // Удалено
    operator T() const;
    Matrix<T> transposition();

    // Дружественные функции
    template<typename TT>
    friend std::istream& operator>>(std::istream& ist, Matrix<TT>& matr);
    template<typename TT>
    friend std::ostream& operator<<(std::ostream& ost, const Matrix<TT>& matr);

    template<typename TT>
    friend Matrix<TT> operator+(const Matrix<TT>& lmatr, const Matrix<TT>& rmatr);
    template<typename TT>
    friend Matrix<TT> operator-(const Matrix<TT>& lmatr, const Matrix<TT>& rmatr);
    template<typename TT>
    friend Matrix<TT> operator*(const Matrix<TT>& lmatr, const Matrix<TT>& rmatr);

    template<typename TT>
    friend Matrix<TT> operator*(const Matrix<TT>& matr, const TT& coef);
    template<typename TT>
    friend Matrix<TT> operator*(const TT& coef, const Matrix<TT>& matr);
    template<typename TT>
    friend Matrix<TT> operator/(const Matrix<TT>& matr, const TT& coef);
};



/* Конструкторы и деструкторы */

template<typename T>
Matrix<T>::Matrix(int row, int col) : row_(row), col_(col)
{
    //std::cout << "create" << std::endl;
    // Выделение памяти под матрицу размера row_ на col_
    matrix_ = new T*[row_];
    for (int i = 0; i < row_; i++)
        matrix_[i] = new T[col_];

    // Обнуление элементов матрицы
    // В будущем может быть переделано или удалено
    for (int i = 0; i < row_; i++)
        for (int j = 0; j < col_; j++)
            matrix_[i][j] = {};
}

/*! Конструктор копирования */
template<typename T>
Matrix<T>::Matrix(const Matrix<T>& matr) : row_(matr.row_), col_(matr.col_)
{
    //std::cout << "copy" << std::endl;
    // Выделение памяти под матрицу размера row_ на col_
    matrix_ = new T*[row_];
    for (int i = 0; i < row_; i++)
        matrix_[i] = new T[col_];

    // Почленное копирование элементов матрицы
    for (int i = 0; i < row_; i++)
        for (int j = 0; j < col_; j++)
            matrix_[i][j] = matr.matrix_[i][j];
}

/*! Конструктор перемещения */
template<typename T>
Matrix<T>::Matrix(Matrix<T>&& matr)
    : matrix_(matr.matrix_), row_(matr.row_), col_(matr.col_)
{
    //std::cout << "move" << std::endl;
    matr.row_ = 0;
    matr.col_ = 0;
    matr.matrix_ = nullptr;
}

template<typename T>
Matrix<T>::~Matrix()
{
    //std::cout << "delete" << std::endl;
    // Освобождение памяти, занимаемой матрицей
    for (int i = 0; i < row_; i++)
        delete[] matrix_[i];
    delete[] matrix_;
}



/* Методы */

/*! Оператор присваивания с копированием */
template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& matr)
{
    //std::cout << "copy=" << std::endl;
    // Защита от присваивания матрицы самой себе
    if (&matr == this)
        return *this;

    // Освобождение памяти, занимаемой вызывающей матрицей
    for (int i = 0; i < row_; i++)
        delete[] matrix_[i];
    delete[] matrix_;

    // Установка новых размеров
    row_ = matr.row_;
    col_ = matr.col_;

    // Выделение памяти для матрицы нового размера
    matrix_ = new T*[row_];
    for (int i = 0; i < row_; i++)
        matrix_[i] = new T[col_];

    // Почленное копирование элементов матрицы
    for (int i = 0; i < row_; i++)
        for (int j = 0; j < col_; j++)
            matrix_[i][j] = matr.matrix_[i][j];

    // Возврат ссылки на вызывающую матрицу для возможности цепного присваивания
    return *this;
}

/*! Оператор присваивания с перемещением */
template<typename T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& matr)
{
    //std::cout << "move=" << std::endl;
    // Освобождение памяти, занимаемой вызывающей матрицей
    for (int i = 0; i < row_; i++)
        delete[] matrix_[i];
    delete[] matrix_;

    // Установка новых размеров
    row_ = matr.row_;
    col_ = matr.col_;

    // Копирование указателя на данные
    matrix_ = matr.matrix_;

    // Делаем старую матрицу пустой
    matr.matrix_ = nullptr;
    matr.row_ = 0;
    matr.col_ = 0;

    // Возврат ссылки на вызывающую матрицу
    return *this;
}

template<typename T>
const T& Matrix<T>::operator()(int row, int col) const
{
    // Если индексы некорректны, генерируется исключение
    if (row < 0 || row >= row_ ||
        col < 0 || col >= col_)
        throw Index_error(row, col);

    return matrix_[row][col];
}

template<typename T>
T& Matrix<T>::operator()(int row, int col)
{
    // Если индексы некорректны, генерируется исключение
    if (row < 0 || row >= row_ ||
        col < 0 || col >= col_)
        throw Index_error(row, col);

    return matrix_[row][col];
}

/*! Приведение к базовому типу матрицы */
template<typename T>
Matrix<T>::operator T() const
{
    return matrix_[0][0];
}

/*! Транспонирование матрицы */
template<typename T>
Matrix<T> Matrix<T>::transposition ()
{
    Matrix<T> result(col_, row_);

    for (int i = 0; i < result.row_; i++)
        for (int j = 0; j < result.col_; j++)
            result.matrix_[i][j] = matrix_[j][i];

    return result;
}



/* Дружественные функции */

/*! Ввод  матрицы из потока ist */
template<typename T>
std::istream& operator>>(std::istream& ist, Matrix<T>& matr)
{
    // Поэлементный ввод из потока ist
    for (int i = 0; i < matr.row_; i++)
        for (int j = 0; j < matr.col_; j++)
            ist >> matr.matrix_[i][j];

    // Возврат ссылки на поток ist для возможности цепного ввода
    return ist;
}

/*! Вывод матрицы в поток ost */
template<typename T>
std::ostream& operator<<(std::ostream& ost, const Matrix<T>& matr)
{
    // Поэлементный вывод в поток ost с нужным форматированием
    for (int i = 0; i < matr.row_; i++)
    {
        for (int j = 0; j < matr.col_; j++)
            ost << matr.matrix_[i][j] << ' ';
        ost << std::endl;
    }

    // Возврат ссылки на поток ost для возможности цепного ввода
    return ost;
}

/*! Сложение матриц */
template<typename T>
Matrix<T> operator+(const Matrix<T>& lmatr, const Matrix<T>& rmatr)
{
    // В случае несовпадения размеров матриц генерируется исключение
    if (lmatr.row_ != rmatr.row_ || lmatr.col_ != rmatr.col_)
        throw Matrix<T>::Size_error(lmatr.row_, lmatr.col_, rmatr.row_, rmatr.col_);

    Matrix<T> result(lmatr);

    for (int i = 0; i < result.row_; i++)
        for (int j = 0; j < result.col_; j++)
            result.matrix_[i][j] += rmatr.matrix_[i][j];

    return result;
}

/*! Разность матриц */
template<typename T>
Matrix<T> operator-(const Matrix<T>& lmatr, const Matrix<T>& rmatr)
{
    // В случае несовпадения размеров матриц генерируется исключение
    if (lmatr.row_ != rmatr.row_ || lmatr.col_ != rmatr.col_)
        throw Matrix<T>::Size_error(lmatr.row_, lmatr.col_, rmatr.row_, rmatr.col_);

    Matrix<T> result(lmatr);

    for (int i = 0; i < result.row_; i++)
        for (int j = 0; j < result.col_; j++)
            result.matrix_[i][j] -= rmatr.matrix_[i][j];

    return result;
}

/*! Умножение матриц */
template<typename T>
Matrix<T> operator*(const Matrix<T>& lmatr, const Matrix<T>& rmatr)
{
    // В случае неподходящих размеров генерируется исключение
    if (lmatr.col_ != rmatr.row_)
        throw Matrix<T>::Size_error(lmatr.row_, lmatr.col_, rmatr.row_, rmatr.col_);

    Matrix<T> result(lmatr.row_, rmatr.col_);

    for (int i = 0; i < result.row_; i++)
        for (int j = 0; j < result.col_; j++)
            for (int k = 0; k < lmatr.col_; k++)
                result.matrix_[i][j] += lmatr.matrix_[i][k] * rmatr.matrix_[k][j];
    std::cout << result <<std::endl;
    return result;
}

/*! Умножение матрицы на элемент базового типа */
template<typename T>
Matrix<T> operator*(const Matrix<T>& matr, const T& coef)
{
    Matrix<T> result(matr);

    for (int i = 0; i < result.row_; i++)
        for (int j = 0; j < result.col_; j++)
            result.matrix_[i][j] = result.matrix_[i][j] * coef;

    return result;
}

/*! Умножение элемента базового типа на матрицу */
template<typename T>
Matrix<T> operator*(const T& coef, const Matrix<T>& matr)
{
    return matr * coef;
}

/*! Деление матрицы на элемент базового типа */
template<typename T>
Matrix<T> operator/(const Matrix<T>& matr, const T& coef)
{
    return matr * (1 / coef);
}
