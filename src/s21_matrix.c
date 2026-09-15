#include "s21_matrix.h"

#include <stdio.h>

static int s21_is_correct_matrix(s21_matrix* A) {
  return (A != NULL && A->matrix != NULL && A->rows > 0 && A->columns > 0);
}

static int s21_get_minor(s21_matrix* A, int row, int col, s21_matrix* minor) {
  int res = s21_create_matrix(A->rows - 1, A->columns - 1, minor);
  if (res != 0) return res;

  int minor_row = 0;
  for (int i = 0; i < A->rows; i++) {
    if (i == row) continue;
    int minor_col = 0;
    for (int j = 0; j < A->columns; j++) {
      if (j == col) continue;
      minor->matrix[minor_row][minor_col] = A->matrix[i][j];
      minor_col++;
    }
    minor_row++;
  }
  return 0;
}

static double s21_determinant_recursive(s21_matrix* A) {
  double result = 0.0;
  if (A->rows == 1) {
    result = A->matrix[0][0];
  } else if (A->rows == 2) {
    result =
        A->matrix[0][0] * A->matrix[1][1] - A->matrix[0][1] * A->matrix[1][0];
  } else {
    s21_matrix minor;
    for (int j = 0; j < A->columns; j++) {
      s21_get_minor(A, 0, j, &minor);
      double sign = (j % 2 == 0) ? 1.0 : -1.0;
      result += sign * A->matrix[0][j] * s21_determinant_recursive(&minor);
      s21_remove_matrix(&minor);
    }
  }
  return result;
}

// ====================== Основные функции ======================

int s21_create_matrix(int rows, int columns, s21_matrix* result) {
  int res = 0;
  if (rows < 1 || columns < 1 || result == NULL) {
    res = 1;
  } else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = calloc(rows, sizeof(double*));
    if (result->matrix == NULL) {
      res = 1;
    } else {
      for (int i = 0; i < rows; i++) {
        result->matrix[i] = calloc(columns, sizeof(double));
        if (result->matrix[i] == NULL) {
          // Откат при ошибке выделения памяти
          for (int k = 0; k < i; k++) {
            free(result->matrix[k]);
          }
          free(result->matrix);
          result->matrix = NULL;
          result->rows = 0;
          result->columns = 0;
          res = 1;
          break;
        }
      }
    }
  }
  return res;
}

void s21_remove_matrix(s21_matrix* A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) {
      free(A->matrix[i]);
    }
    free(A->matrix);
    A->matrix = NULL;
    A->rows = 0;
    A->columns = 0;
  }
}

int s21_eq_matrix(s21_matrix* A, s21_matrix* B) {
  int res = SUCCESS;
  if (!s21_is_correct_matrix(A) || !s21_is_correct_matrix(B) ||
      A->rows != B->rows || A->columns != B->columns) {
    res = FAILURE;
  } else {
    for (int i = 0; i < A->rows && res == SUCCESS; i++) {
      for (int j = 0; j < A->columns && res == SUCCESS; j++) {
        double diff = fabs(A->matrix[i][j] - B->matrix[i][j]);
        if (diff > 1e-6) {
          res = FAILURE;
        }
      }
    }
  }
  return res;
}

int s21_sum_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result) {
  int res = 0;
  if (!s21_is_correct_matrix(A) || !s21_is_correct_matrix(B)) {
    res = 1;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    res = 2;
  } else {
    res = s21_create_matrix(A->rows, A->columns, result);
    if (res == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] + B->matrix[i][j];
        }
      }
    }
  }
  return res;
}

int s21_sub_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result) {
  int res = 0;
  if (!s21_is_correct_matrix(A) || !s21_is_correct_matrix(B)) {
    res = 1;
  } else if (A->rows != B->rows || A->columns != B->columns) {
    res = 2;
  } else {
    res = s21_create_matrix(A->rows, A->columns, result);
    if (res == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] - B->matrix[i][j];
        }
      }
    }
  }
  return res;
}

int s21_mult_number(s21_matrix* A, double number, s21_matrix* result) {
  int res = 0;
  if (!s21_is_correct_matrix(A)) {
    res = 1;
  } else {
    res = s21_create_matrix(A->rows, A->columns, result);
    if (res == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[i][j] = A->matrix[i][j] * number;
        }
      }
    }
  }
  return res;
}

int s21_mult_matrix(s21_matrix* A, s21_matrix* B, s21_matrix* result) {
  int res = 0;
  if (!s21_is_correct_matrix(A) || !s21_is_correct_matrix(B)) {
    res = 1;
  } else if (A->columns != B->rows) {
    res = 2;
  } else {
    res = s21_create_matrix(A->rows, B->columns, result);
    if (res == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < B->columns; j++) {
          double sum = 0.0;
          for (int k = 0; k < A->columns; k++) {
            sum += A->matrix[i][k] * B->matrix[k][j];
          }
          result->matrix[i][j] = sum;
        }
      }
    }
  }
  return res;
}

int s21_transpose(s21_matrix* A, s21_matrix* result) {
  int res = 0;
  if (!s21_is_correct_matrix(A)) {
    res = 1;
  } else {
    res = s21_create_matrix(A->columns, A->rows, result);
    if (res == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          result->matrix[j][i] = A->matrix[i][j];
        }
      }
    }
  }
  return res;
}

int s21_calc_complements(s21_matrix* A, s21_matrix* result) {
  int res = 0;
  if (!s21_is_correct_matrix(A)) {
    res = 1;
  } else if (A->rows != A->columns) {
    res = 2;
  } else {
    res = s21_create_matrix(A->rows, A->columns, result);
    if (res == 0) {
      for (int i = 0; i < A->rows; i++) {
        for (int j = 0; j < A->columns; j++) {
          s21_matrix minor;
          s21_get_minor(A, i, j, &minor);
          double det = s21_determinant_recursive(&minor);
          double sign = ((i + j) % 2 == 0) ? 1.0 : -1.0;
          result->matrix[i][j] = sign * det;
          s21_remove_matrix(&minor);
        }
      }
    }
  }
  return res;
}

int s21_determinant(s21_matrix* A, double* result) {
  int res = 0;
  if (!s21_is_correct_matrix(A) || result == NULL) {
    res = 1;
  } else if (A->rows != A->columns) {
    res = 2;
  } else {
    *result = s21_determinant_recursive(A);
  }
  return res;
}

int s21_inverse_matrix(s21_matrix* A, s21_matrix* result) {
  int res = 0;
  if (!s21_is_correct_matrix(A)) {
    res = 1;
  } else if (A->rows != A->columns) {
    res = 2;
  } else {
    double det = 0.0;
    res = s21_determinant(A, &det);
    if (res == 0) {
      if (fabs(det) < 1e-6) {
        res = 2;
      } else {
        // Специальный случай для матрицы 1x1
        if (A->rows == 1) {
          res = s21_create_matrix(1, 1, result);
          if (res == 0) {
            result->matrix[0][0] = 1.0 / det;
          }
        } else {
          s21_matrix complements;
          s21_matrix transposed;
          res = s21_calc_complements(A, &complements);
          if (res == 0) {
            res = s21_transpose(&complements, &transposed);
            if (res == 0) {
              res = s21_mult_number(&transposed, 1.0 / det, result);
              s21_remove_matrix(&transposed);
            }
            s21_remove_matrix(&complements);
          }
        }
      }
    }
  }
  return res;
}