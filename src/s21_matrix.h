#ifndef S21_MATRIX_H
#define S21_MATRIX_H

#include <math.h>
#include <stdlib.h>

#define SUCCESS 1
#define FAILURE 0

typedef struct matrix_struct {
  double** matrix;
  int rows;
  int columns;
} s21_matrix;

// Создание матрицы
int s21_create_matrix(int rows, int columns, s21_matrix* result);
// Очистка матрицы
void s21_remove_matrix(s21_matrix* A);
// Сравнение матриц
int s21_eq_matrix(s21_matrix* A, s21_matrix* B);
// Сложение матриц
int s21_sum_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result);
// Вычитание матриц
int s21_sub_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result);
// Умножение матрицы на число
int s21_mult_number(s21_matrix* A, double number, s21_matrix* result);
// Умножение двух матриц
int s21_mult_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result);
// Транспонирование матрицы
int s21_transpose(s21_matrix* A, s21_matrix* result);
// Матрица алгебраических дополнений
int s21_calc_complements(s21_matrix* A, s21_matrix* result);
// Определитель матрицы
int s21_determinant(s21_matrix* A, double* result);
// Обратная матрица
int s21_inverse_matrix(s21_matrix* A, s21_matrix* result);

#endif