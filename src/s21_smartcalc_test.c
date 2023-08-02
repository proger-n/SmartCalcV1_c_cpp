#include "s21_smartcalc_test.h"

START_TEST(test_1) {
  double answer = 0.0;
  char str_1[255] = "2 + 2 + 2 + 2 + 2 + 2 + 2 + 2 + 2 + 2";
  int correct = head_calc(str_1, 0, 0, &answer);
  ck_assert_int_eq(0, correct);
  ck_assert_int_eq(20, answer);
}
END_TEST

START_TEST(test_2) {
  double answer = 0.0;
  char str[255] =
      "15/(7-(1+1))*3-(2+(1+1))*15/(7-(200+1))*3-(2+(1+1))*(15/"
      "(7-(1+1))*3-(2+(1+1))+15/(7-(1+1))*3-(2+(1+1)))";
  int correct = head_calc(str, 0, 0, &answer);
  ck_assert_int_eq(0, correct);
  ck_assert_double_eq_tol(-30.072164948, answer, 1e-2);
}
END_TEST

START_TEST(test_3) {
  double answer = 0.0;
  char str_1[255] = "sin(0.3) + cos(0.7) - 2^4 - (7mod5)*sqrt(4)";
  int correct = head_calc(str_1, 0, 0, &answer);
  ck_assert_int_eq(0, correct);
  ck_assert_double_eq_tol(-18.939637, answer, 1e-2);
}
END_TEST

START_TEST(test_4) {
  double answer = 0.0;
  char str_1[255] =
      "asin(0.3) + acos(0.7) - ln(10)*log(10) + tan(0.4)*atan(0.4)";
  int correct = head_calc(str_1, 0, 0, &answer);
  ck_assert_int_eq(0, correct);
  ck_assert_double_eq_tol(-1.0416180, answer, 1e-2);
}
END_TEST

START_TEST(test_5) {
  double answer = 0.0;
  char str_1[255] = "assin(0.3)";
  int correct = head_calc(str_1, 0, 0, &answer);
  ck_assert_int_eq(1, correct);
}
END_TEST

START_TEST(test_6) {
  double answer = 0.0;
  char str_1[255] = "-asin(0.3)";
  int correct = head_calc(str_1, 0, 0, &answer);
  ck_assert_int_eq(0, correct);
  ck_assert_double_eq_tol(-0.304692, answer, 1e-2);
}
END_TEST

START_TEST(test_x) {
  double answer = 0.0;
  int value = 2;

  char str_1[255] = "x + x + x + x + x + x + x + x + x + x";
  int correct = head_calc(str_1, 1, value, &answer);
  ck_assert_int_eq(0, correct);
  ck_assert_int_eq(20, answer);
}
END_TEST

Suite *calc(void) {
  Suite *s = suite_create("mitcheld");
  TCase *tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_1);
  tcase_add_test(tc_core, test_2);
  tcase_add_test(tc_core, test_3);
  tcase_add_test(tc_core, test_4);
  tcase_add_test(tc_core, test_5);
  tcase_add_test(tc_core, test_6);
  tcase_add_test(tc_core, test_x);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  Suite *s;
  SRunner *runner;

  s = calc();
  runner = srunner_create(s);

  srunner_run_all(runner, CK_NORMAL);
  srunner_free(runner);
  return 0;
}
