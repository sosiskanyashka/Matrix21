#include "s21_matrix.h"

#include <check.h>
#include <stdio.h>
#include <stdlib.h>

START_TEST(test_create_matrix_ok) {
  s21_matrix A;
  int res = s21_create_matrix(3, 4, &A);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(A.rows, 3);
  ck_assert_int_eq(A.columns, 4);
  ck_assert_ptr_nonnull(A.matrix);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_double_eq(A.matrix[i][j], 0.0);
    }
  }
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_create_matrix_fail) {
  s21_matrix A;
  ck_assert_int_eq(s21_create_matrix(0, 4, &A), 1);
  ck_assert_int_eq(s21_create_matrix(3, 0, &A), 1);
  ck_assert_int_eq(s21_create_matrix(-1, 4, &A), 1);
  ck_assert_int_eq(s21_create_matrix(3, 4, NULL), 1);
}
END_TEST

START_TEST(test_remove_matrix_ok) {
  s21_matrix A;
  s21_create_matrix(3, 3, &A);
  s21_remove_matrix(&A);
  ck_assert_ptr_null(A.matrix);
  ck_assert_int_eq(A.rows, 0);
  ck_assert_int_eq(A.columns, 0);
}
END_TEST

START_TEST(test_remove_matrix_null) {
  s21_remove_matrix(NULL);
  s21_matrix A = {NULL, 0, 0};
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_eq_matrix_equal) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = 1.0;
  A.matrix[0][1] = 2.0;
  A.matrix[1][0] = 3.0;
  A.matrix[1][1] = 4.0;
  B.matrix[0][0] = 1.0;
  B.matrix[0][1] = 2.0;
  B.matrix[1][0] = 3.0;
  B.matrix[1][1] = 4.0;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_not_equal) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = 1.0;
  B.matrix[0][0] = 2.0;
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_precision) {
  s21_matrix A, B;
  s21_create_matrix(1, 1, &A);
  s21_create_matrix(1, 1, &B);
  A.matrix[0][0] = 1.0;
  B.matrix[0][0] = 1.0 + 1e-7;  // в пределах точности
  ck_assert_int_eq(s21_eq_matrix(&A, &B), SUCCESS);
  B.matrix[0][0] = 1.0 + 1e-5;  // вне точности
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_eq_matrix_diff_size) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);
  ck_assert_int_eq(s21_eq_matrix(&A, &B), FAILURE);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_ok) {
  s21_matrix A, B, C;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;
  B.matrix[0][0] = 5;
  B.matrix[0][1] = 6;
  B.matrix[1][0] = 7;
  B.matrix[1][1] = 8;
  int res = s21_sum_matrix(&A, &B, &C);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq(C.matrix[0][0], 6);
  ck_assert_double_eq(C.matrix[1][1], 12);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(test_sum_matrix_diff_size) {
  s21_matrix A, B, C;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 3, &B);
  ck_assert_int_eq(s21_sum_matrix(&A, &B, &C), 2);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_sum_matrix_null) {
  s21_matrix A, C;
  s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(s21_sum_matrix(&A, NULL, &C), 1);
  ck_assert_int_eq(s21_sum_matrix(NULL, &A, &C), 1);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_sub_matrix_ok) {
  s21_matrix A, B, C;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(2, 2, &B);
  A.matrix[0][0] = 5;
  A.matrix[0][1] = 6;
  A.matrix[1][0] = 7;
  A.matrix[1][1] = 8;
  B.matrix[0][0] = 1;
  B.matrix[0][1] = 2;
  B.matrix[1][0] = 3;
  B.matrix[1][1] = 4;
  int res = s21_sub_matrix(&A, &B, &C);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq(C.matrix[0][0], 4);
  ck_assert_double_eq(C.matrix[1][1], 4);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(test_sub_matrix_diff_size) {
  s21_matrix A, B, C;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 2, &B);
  ck_assert_int_eq(s21_sub_matrix(&A, &B, &C), 2);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_number_ok) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;
  int res = s21_mult_number(&A, 2.0, &B);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq(B.matrix[0][0], 2);
  ck_assert_double_eq(B.matrix[1][1], 8);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_mult_number_null) {
  s21_matrix A, B;
  s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(s21_mult_number(NULL, 2.0, &B), 1);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_mult_matrix_ok) {
  s21_matrix A, B, C;
  s21_create_matrix(2, 3, &A);
  s21_create_matrix(3, 2, &B);
  // A = [[1,4],[2,5],[3,6]] (2x3)
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 4;
  A.matrix[0][2] = 0;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 0;
  // B = [[1,-1],[2,3],[0,0]] (3x2)
  B.matrix[0][0] = 1;
  B.matrix[0][1] = -1;
  B.matrix[1][0] = 2;
  B.matrix[1][1] = 3;
  B.matrix[2][0] = 0;
  B.matrix[2][1] = 0;
  int res = s21_mult_matrix(&A, &B, &C);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(C.rows, 2);
  ck_assert_int_eq(C.columns, 2);
  ck_assert_double_eq(C.matrix[0][0], 9);
  ck_assert_double_eq(C.matrix[0][1], 11);
  ck_assert_double_eq(C.matrix[1][0], 12);
  ck_assert_double_eq(C.matrix[1][1], 13);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(test_mult_matrix_incompatible) {
  s21_matrix A, B, C;
  s21_create_matrix(2, 2, &A);
  s21_create_matrix(3, 2, &B);
  ck_assert_int_eq(s21_mult_matrix(&A, &B, &C), 2);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_transpose_ok) {
  s21_matrix A, B;
  s21_create_matrix(2, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 4;
  A.matrix[0][2] = 7;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 8;
  int res = s21_transpose(&A, &B);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(B.rows, 3);
  ck_assert_int_eq(B.columns, 2);
  ck_assert_double_eq(B.matrix[0][0], 1);
  ck_assert_double_eq(B.matrix[0][1], 2);
  ck_assert_double_eq(B.matrix[1][0], 4);
  ck_assert_double_eq(B.matrix[2][1], 8);
  s21_remove_matrix(&A);
  s21_remove_matrix(&B);
}
END_TEST

START_TEST(test_transpose_null) {
  s21_matrix B;
  ck_assert_int_eq(s21_transpose(NULL, &B), 1);
}
END_TEST

START_TEST(test_determinant_1x1) {
  s21_matrix A;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 5.0;
  double det = 0;
  int res = s21_determinant(&A, &det);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq(det, 5.0);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_2x2) {
  s21_matrix A;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 3;
  A.matrix[1][1] = 4;
  double det = 0;
  int res = s21_determinant(&A, &det);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq(det, -2.0);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_3x3) {
  s21_matrix A;
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 4;
  A.matrix[1][1] = 5;
  A.matrix[1][2] = 6;
  A.matrix[2][0] = 7;
  A.matrix[2][1] = 8;
  A.matrix[2][2] = 9;
  double det = 0;
  int res = s21_determinant(&A, &det);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq(det, 0.0);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_not_square) {
  s21_matrix A;
  s21_create_matrix(2, 3, &A);
  double det = 0;
  ck_assert_int_eq(s21_determinant(&A, &det), 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_determinant_null) {
  double det = 0;
  ck_assert_int_eq(s21_determinant(NULL, &det), 1);
  s21_matrix A;
  s21_create_matrix(2, 2, &A);
  ck_assert_int_eq(s21_determinant(&A, NULL), 1);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_calc_complements_ok) {
  s21_matrix A, C;
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[0][2] = 3;
  A.matrix[1][0] = 0;
  A.matrix[1][1] = 4;
  A.matrix[1][2] = 2;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = 2;
  A.matrix[2][2] = 1;
  int res = s21_calc_complements(&A, &C);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq(C.matrix[0][0], 0);
  ck_assert_double_eq(C.matrix[0][1], 10);
  ck_assert_double_eq(C.matrix[0][2], -20);
  ck_assert_double_eq(C.matrix[1][0], 4);
  ck_assert_double_eq(C.matrix[1][1], -14);
  ck_assert_double_eq(C.matrix[1][2], 8);
  ck_assert_double_eq(C.matrix[2][0], -8);
  ck_assert_double_eq(C.matrix[2][1], -2);
  ck_assert_double_eq(C.matrix[2][2], 4);
  s21_remove_matrix(&A);
  s21_remove_matrix(&C);
}
END_TEST

START_TEST(test_calc_complements_not_square) {
  s21_matrix A, C;
  s21_create_matrix(2, 3, &A);
  ck_assert_int_eq(s21_calc_complements(&A, &C), 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_ok) {
  s21_matrix A, Inv, Check;
  s21_create_matrix(3, 3, &A);
  A.matrix[0][0] = 2;
  A.matrix[0][1] = 5;
  A.matrix[0][2] = 7;
  A.matrix[1][0] = 6;
  A.matrix[1][1] = 3;
  A.matrix[1][2] = 4;
  A.matrix[2][0] = 5;
  A.matrix[2][1] = -2;
  A.matrix[2][2] = -3;
  int res = s21_inverse_matrix(&A, &Inv);
  ck_assert_int_eq(res, 0);
  // Проверим: A * Inv ≈ E
  res = s21_mult_matrix(&A, &Inv, &Check);
  ck_assert_int_eq(res, 0);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      double expected = (i == j) ? 1.0 : 0.0;
      ck_assert_double_eq_tol(Check.matrix[i][j], expected, 1e-6);
    }
  }
  s21_remove_matrix(&A);
  s21_remove_matrix(&Inv);
  s21_remove_matrix(&Check);
}
END_TEST

START_TEST(test_inverse_matrix_1x1) {
  s21_matrix A, Inv;
  s21_create_matrix(1, 1, &A);
  A.matrix[0][0] = 2.0;
  int res = s21_inverse_matrix(&A, &Inv);
  ck_assert_int_eq(res, 0);
  ck_assert_double_eq_tol(Inv.matrix[0][0], 0.5, 1e-6);
  s21_remove_matrix(&A);
  s21_remove_matrix(&Inv);
}
END_TEST

START_TEST(test_inverse_matrix_det_zero) {
  s21_matrix A, Inv;
  s21_create_matrix(2, 2, &A);
  A.matrix[0][0] = 1;
  A.matrix[0][1] = 2;
  A.matrix[1][0] = 2;
  A.matrix[1][1] = 4;  // det = 0
  ck_assert_int_eq(s21_inverse_matrix(&A, &Inv), 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_not_square) {
  s21_matrix A, Inv;
  s21_create_matrix(2, 3, &A);
  ck_assert_int_eq(s21_inverse_matrix(&A, &Inv), 2);
  s21_remove_matrix(&A);
}
END_TEST

START_TEST(test_inverse_matrix_null) {
  s21_matrix Inv;
  ck_assert_int_eq(s21_inverse_matrix(NULL, &Inv), 1);
}
END_TEST

Suite* s21_matrix_suite(void) {
  Suite* s = suite_create("s21_matrix");
  TCase* tc;

  tc = tcase_create("create_matrix");
  tcase_add_test(tc, test_create_matrix_ok);
  tcase_add_test(tc, test_create_matrix_fail);
  suite_add_tcase(s, tc);

  tc = tcase_create("remove_matrix");
  tcase_add_test(tc, test_remove_matrix_ok);
  tcase_add_test(tc, test_remove_matrix_null);
  suite_add_tcase(s, tc);

  tc = tcase_create("eq_matrix");
  tcase_add_test(tc, test_eq_matrix_equal);
  tcase_add_test(tc, test_eq_matrix_not_equal);
  tcase_add_test(tc, test_eq_matrix_precision);
  tcase_add_test(tc, test_eq_matrix_diff_size);
  suite_add_tcase(s, tc);

  tc = tcase_create("sum_matrix");
  tcase_add_test(tc, test_sum_matrix_ok);
  tcase_add_test(tc, test_sum_matrix_diff_size);
  tcase_add_test(tc, test_sum_matrix_null);
  suite_add_tcase(s, tc);

  tc = tcase_create("sub_matrix");
  tcase_add_test(tc, test_sub_matrix_ok);
  tcase_add_test(tc, test_sub_matrix_diff_size);
  suite_add_tcase(s, tc);

  tc = tcase_create("mult_number");
  tcase_add_test(tc, test_mult_number_ok);
  tcase_add_test(tc, test_mult_number_null);
  suite_add_tcase(s, tc);

  tc = tcase_create("mult_matrix");
  tcase_add_test(tc, test_mult_matrix_ok);
  tcase_add_test(tc, test_mult_matrix_incompatible);
  suite_add_tcase(s, tc);

  tc = tcase_create("transpose");
  tcase_add_test(tc, test_transpose_ok);
  tcase_add_test(tc, test_transpose_null);
  suite_add_tcase(s, tc);

  tc = tcase_create("determinant");
  tcase_add_test(tc, test_determinant_1x1);
  tcase_add_test(tc, test_determinant_2x2);
  tcase_add_test(tc, test_determinant_3x3);
  tcase_add_test(tc, test_determinant_not_square);
  tcase_add_test(tc, test_determinant_null);
  suite_add_tcase(s, tc);

  tc = tcase_create("calc_complements");
  tcase_add_test(tc, test_calc_complements_ok);
  tcase_add_test(tc, test_calc_complements_not_square);
  suite_add_tcase(s, tc);

  tc = tcase_create("inverse_matrix");
  tcase_add_test(tc, test_inverse_matrix_ok);
  tcase_add_test(tc, test_inverse_matrix_1x1);
  tcase_add_test(tc, test_inverse_matrix_det_zero);
  tcase_add_test(tc, test_inverse_matrix_not_square);
  tcase_add_test(tc, test_inverse_matrix_null);
  suite_add_tcase(s, tc);

  return s;
}

int main(void) {
  Suite* s = s21_matrix_suite();
  SRunner* sr = srunner_create(s);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? 0 : 1;
}