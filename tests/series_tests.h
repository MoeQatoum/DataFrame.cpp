#ifndef SERIES_TESTS_H
#define SERIES_TESTS_H

#include <dataframe.h>
#undef clog

#include "test_utils.h"

#include <gtest/gtest.h>

#define TEST_COL_COUNT 10
#define TEST_ROW_COUNT 10

using namespace df;

TEST(series_operators_tests, is_equal) {
  constexpr int N = 10;

  df::Series<int> series_1 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_2 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_3 = make_series<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0});

  df::Series<bool> result_1 = (series_1 == series_1);
  df::Series<bool> result_2 = (series_1 == series_2);
  df::Series<bool> result_3 = (series_2 == series_3);
  df::Series<bool> result_4 = (series_1 == 1);
  df::Series<bool> result_5 = (1 == series_2);

  bool expected_1[10] = {true, true, true, true, true, true, true, true, true, true};
  bool expected_2[10] = {false, true, false, false, false, false, false, false, false, false};

  for (sizetype i = 0; i < N; i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
    EXPECT_NE(result_3[i], expected_1[i]);
    EXPECT_EQ(result_4[i], expected_2[i]);
    EXPECT_EQ(result_5[i], expected_2[i]);
  }
}

TEST(series_operators_tests, is_not_equal_operator) {
  constexpr int N = 10;

  df::Series<int> series_1 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_2 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_3 = make_series<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0});

  df::Series<bool> result_1 = (series_1 != series_1);
  df::Series<bool> result_2 = (series_1 != series_2);
  df::Series<bool> result_3 = (series_2 != series_3);
  df::Series<bool> result_4 = (series_1 != 1);
  df::Series<bool> result_5 = (1 != series_2);

  bool expected_1[10] = {false, false, false, false, false, false, false, false, false, false};
  bool expected_2[10] = {true, false, true, true, true, true, true, true, true, true};

  for (sizetype i = 0; i < N; i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
    EXPECT_NE(result_3[i], expected_1[i]);
    EXPECT_EQ(result_4[i], expected_2[i]);
    EXPECT_EQ(result_5[i], expected_2[i]);
  }
}

TEST(series_operators_tests, grater_or_equal_operator) {
  constexpr int N = 10;

  df::Series<int> series_1 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_2 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_3 = make_series<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0});

  df::Series<bool> result_1 = (series_1 >= series_1);
  df::Series<bool> result_2 = (series_1 >= series_2);
  df::Series<bool> result_3 = (series_2 >= series_3);
  df::Series<bool> result_4 = (series_1 >= 1);
  df::Series<bool> result_5 = (3 >= series_2);

  bool expected_1[10] = {true, true, true, true, true, true, true, true, true, true};
  bool expected_2[10] = {false, false, false, false, false, true, true, true, true, true};
  bool expected_3[10] = {false, true, true, true, true, true, true, true, true, true};
  bool expected_4[10] = {true, true, true, true, false, false, false, false, false, false};

  for (sizetype i = 0; i < N; i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
    EXPECT_EQ(result_3[i], expected_2[i]);
    EXPECT_EQ(result_4[i], expected_3[i]);
    EXPECT_EQ(result_5[i], expected_4[i]);
  }
}

TEST(series_operators_tests, less_than_or_equal_operator) {
  constexpr int N = 10;

  df::Series<int> series_1 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_2 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_3 = make_series<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0});

  df::Series<bool> result_1 = (series_1 <= series_1);
  df::Series<bool> result_2 = (series_1 <= series_2);
  df::Series<bool> result_3 = (series_2 <= series_3);
  df::Series<bool> result_4 = (series_1 <= 1);
  df::Series<bool> result_5 = (3 <= series_2);

  bool expected_1[10] = {true, true, true, true, true, true, true, true, true, true};
  bool expected_2[10] = {true, true, true, true, true, false, false, false, false, false};
  bool expected_3[10] = {true, true, false, false, false, false, false, false, false, false};
  bool expected_4[10] = {false, false, false, true, true, true, true, true, true, true};

  for (sizetype i = 0; i < N; i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
    EXPECT_EQ(result_3[i], expected_2[i]);
    EXPECT_EQ(result_4[i], expected_3[i]);
    EXPECT_EQ(result_5[i], expected_4[i]);
  }
}

TEST(series_operators_tests, less_than_operator) {
  constexpr int N = 10;

  df::Series<int> series_1 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_2 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_3 = make_series<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0});

  df::Series<bool> result_1 = (series_1 < series_1);
  df::Series<bool> result_2 = (series_1 < series_2);
  df::Series<bool> result_3 = (series_2 < series_3);
  df::Series<bool> result_4 = (series_1 < 1);
  df::Series<bool> result_5 = (3 < series_2);

  bool expected_1[10] = {false, false, false, false, false, false, false, false, false, false};
  bool expected_2[10] = {false, false, false, false, false, false, false, false, false, false};
  bool expected_3[10] = {true, true, true, true, true, false, false, false, false, false};
  bool expected_4[10] = {true, false, false, false, false, false, false, false, false, false};
  bool expected_5[10] = {false, false, false, false, true, true, true, true, true, true};

  for (sizetype i = 0; i < N; i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_2[i]);
    EXPECT_EQ(result_3[i], expected_3[i]);
    EXPECT_EQ(result_4[i], expected_4[i]);
    EXPECT_EQ(result_5[i], expected_5[i]);
  }
}

TEST(series_operators_tests, grater_than_operator) {
  constexpr int N = 10;

  df::Series<int> series_1 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_2 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
  df::Series<int> series_3 = make_series<int>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0});

  df::Series<bool> result_1 = (series_1 > series_1);
  df::Series<bool> result_2 = (series_1 > series_2);
  df::Series<bool> result_3 = (series_2 > series_3);
  df::Series<bool> result_4 = (series_1 > 1);
  df::Series<bool> result_5 = (3 > series_2);

  bool expected_1[10] = {false, false, false, false, false, false, false, false, false, false};
  bool expected_2[10] = {false, false, false, false, false, false, false, false, false, false};
  bool expected_3[10] = {false, false, false, false, false, true, true, true, true, true};
  bool expected_4[10] = {false, false, true, true, true, true, true, true, true, true};
  bool expected_5[10] = {true, true, true, false, false, false, false, false, false, false};

  for (sizetype i = 0; i < N; i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_2[i]);
    EXPECT_EQ(result_3[i], expected_3[i]);
    EXPECT_EQ(result_4[i], expected_4[i]);
    EXPECT_EQ(result_5[i], expected_5[i]);
  }
}

TEST(series_operators_tests, addition_operators) {
  constexpr int N = 10;

  df::Series<int> series_1 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

  df::Series<int> result_1 = series_1++;
  df::Series<int> result_2 = ++series_1;
  series_1 += 1;
  df::Series<int> result_4 = series_1 + 1;
  df::Series<int> result_5 = 1 + series_1;

  int expected_1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int expected_2[10] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  int expected_3[10] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
  int expected_4[10] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};

  for (sizetype i = 0; i < N; i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_2[i]);
    EXPECT_EQ(series_1[i], expected_3[i]);
    EXPECT_EQ(result_4[i], expected_4[i]);
    EXPECT_EQ(result_5[i], expected_4[i]);
  }
}

TEST(series_operators_tests, multiplication_operators) {
  constexpr int N = 10;

  df::Series<int> series_1 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

  df::Series<int> result_1 = series_1 * 2;
  df::Series<int> result_2 = 2 * series_1;

  int expected_1[10] = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};

  for (sizetype i = 0; i < N; i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
  }
}

TEST(series_operators_tests, subtraction_operators) {
  constexpr int N = 10;

  df::Series<int> series_1 = make_series<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

  df::Series<int> result_1 = series_1--;
  df::Series<int> result_2 = --series_1;
  series_1 -= 1;
  df::Series<int> result_4 = series_1 - 1;
  df::Series<int> result_5 = 1 - series_1;

  int expected_1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int expected_2[10] = {-2, -1, 0, 1, 2, 3, 4, 5, 6, 7};
  int expected_3[10] = {-3, -2, -1, 0, 1, 2, 3, 4, 5, 6};
  int expected_4[10] = {-4, -3, -2, -1, 0, 1, 2, 3, 4, 5};
  int expected_5[10] = {4, 3, 2, 1, 0, -1, -2, -3, -4, -5};

  for (sizetype i = 0; i < N; i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_2[i]);
    EXPECT_EQ(series_1[i], expected_3[i]);
    EXPECT_EQ(result_4[i], expected_4[i]);
    EXPECT_EQ(result_5[i], expected_5[i]);
  }
}

TEST(series_operators_tests, division_operators) {
  df::Series<int> series_1 = make_series<int>({1, 2, 3, 4, 5, 6, 7, 8, 9});

  df::Series<int> result_1 = series_1 / 2;
  df::Series<int> result_2 = 2 / series_1;

  int expected_1[9] = {0, 1, 1, 2, 2, 3, 3, 4, 4};
  int expected_2[9] = {2, 1, 0, 0, 0, 0, 0, 0, 0};

  for (sizetype i = 0; i < series_1.size(); i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_2[i]);
  }
}

#endif // SERIES_TESTS_H
