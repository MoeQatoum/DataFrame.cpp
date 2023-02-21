#ifndef COLUMN_TESTS_H
#define COLUMN_TESTS_H

#include <dataframe.h>
#undef clog

#include "test_utils.h"

#include <gtest/gtest.h>

using namespace df;

// 1 2 3
// 4 5 6
// 7 8 9

TEST(col_operators_tests, is_equal) {
  constexpr int N = 9;

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  Series<bool> result_1 = (df.column(0) == df.column(0));
  Series<bool> result_2 = (df.column(0) == Series<int>{1, 4, 7});
  Series<bool> result_3 = (df.column(0) == 1);
  Series<bool> result_5 = (df.column(1) == 2);
  Series<bool> result_6 = (1 == df.column(0));
  Series<bool> result_7 = (2 == df.column(1));

  bool expected_1[3] = {true, true, true};
  bool expected_2[3] = {true, false, false};

  for (sizetype i = 0; i < df.col_size(); i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
    EXPECT_EQ(result_3[i], expected_2[i]);
    EXPECT_EQ(result_5[i], expected_2[i]);
    EXPECT_EQ(result_6[i], expected_2[i]);
    EXPECT_EQ(result_7[i], expected_2[i]);
  }
}

TEST(col_operators_tests, is_not_equal_operator) {
  constexpr int N = 9;

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  Series<bool> result_1 = (df.column(1) != df.column(1));
  Series<bool> result_2 = (df.column(0) != Series<int>{1, 4, 7});
  Series<bool> result_3 = (df.column(0) != 1);
  Series<bool> result_4 = (df.column(1) != 2);
  Series<bool> result_5 = (1 != df.column(0));
  Series<bool> result_6 = (2 != df.column(1));

  bool expected_1[3] = {false, false, false};
  bool expected_2[3] = {false, true, true};

  for (sizetype i = 0; i < df.col_size(); i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
    EXPECT_EQ(result_3[i], expected_2[i]);
    EXPECT_EQ(result_4[i], expected_2[i]);
    EXPECT_EQ(result_5[i], expected_2[i]);
    EXPECT_EQ(result_6[i], expected_2[i]);
  }
}

TEST(col_operators_tests, grater_or_equal_operator) {
  constexpr int N = 9;
  // 1 2 3
  // 4 5 6
  // 7 8 9

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  Series<bool> result_1 = (df.column(1) >= df.column(1));
  Series<bool> result_2 = (df.column(0) >= Series<int>{1, 4, 7});
  Series<bool> result_3 = (df.column(0) >= 1);
  Series<bool> result_4 = (2 >= df.column(1));

  bool expected_1[3] = {true, true, true};
  bool expected_2[3] = {true, true, true};
  bool expected_3[3] = {true, false, false};

  for (sizetype i = 0; i < df.col_size(); i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
    EXPECT_EQ(result_3[i], expected_2[i]);
    EXPECT_EQ(result_4[i], expected_3[i]);
  }
}

TEST(col_operators_tests, less_than_or_equal_operator) {
  constexpr int N = 9;
  // 1 2 3
  // 4 5 6
  // 7 8 9

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  Series<bool> result_1 = (df.column(1) <= df.column(1));
  Series<bool> result_2 = (df.column(0) <= Series<int>{1, 4, 7});
  Series<bool> result_3 = (df.column(0) <= 1);
  Series<bool> result_4 = (2 <= df.column(1));

  bool expected_1[3] = {true, true, true};
  bool expected_2[3] = {true, false, false};
  bool expected_3[3] = {true, true, true};

  for (sizetype i = 0; i < df.col_size(); i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
    EXPECT_EQ(result_3[i], expected_2[i]);
    EXPECT_EQ(result_4[i], expected_3[i]);
  }
}

TEST(col_operators_tests, less_than_operator) {
  constexpr int N = 9;
  // 1 2 3
  // 4 5 6
  // 7 8 9

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  Series<bool> result_1 = (df.column(1) < df.column(1));
  Series<bool> result_2 = (df.column(0) < Series<int>{1, 4, 7});
  Series<bool> result_3 = (df.column(0) < 1);
  Series<bool> result_4 = (df.column(0) < 3);
  Series<bool> result_5 = (2 < df.column(1));

  bool expected_1[3] = {false, false, false};
  bool expected_2[3] = {false, false, false};
  bool expected_3[3] = {true, false, false};
  bool expected_4[3] = {false, true, true};

  for (sizetype i = 0; i < df.col_size(); i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_1[i]);
    EXPECT_EQ(result_3[i], expected_2[i]);
    EXPECT_EQ(result_4[i], expected_3[i]);
    EXPECT_EQ(result_5[i], expected_4[i]);
  }
}

TEST(col_operators_tests, grater_than_operator) {
  constexpr int N = 9;
  // 1 2 3
  // 4 5 6
  // 7 8 9

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  Series<bool> result_1 = (df.column(1) > df.column(1));
  Series<bool> result_2 = (df.column(1) > Series<int>{1, 4, 7});
  Series<bool> result_3 = (df.column(0) > 1);
  Series<bool> result_4 = (df.column(0) > 3);
  Series<bool> result_5 = (2 > df.column(1));

  bool expected_1[3] = {false, false, false};
  bool expected_2[3] = {true, true, true};
  bool expected_3[3] = {false, true, true};
  bool expected_4[3] = {false, true, true};
  bool expected_5[3] = {false, false, false};

  for (sizetype i = 0; i < df.col_size(); i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_2[i]);
    EXPECT_EQ(result_3[i], expected_3[i]);
    EXPECT_EQ(result_4[i], expected_4[i]);
    EXPECT_EQ(result_5[i], expected_5[i]);
  }
}

TEST(col_operators_tests, add_operators) {
  constexpr int N = 9;
  // 1 2 3
  // 4 5 6
  // 7 8 9

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  // Series<int> result_1 = df.column(0)++;
  // Series<int> result_2 = ++df.column(0);
  // df.column(0) += 1;
  Series<int> result_3 = df.column(0) + Series<int>{1, 2, 3};
  Series<int> result_4 = Series<int>{1, 2, 3} + df.column(0);
  Series<int> result_5 = df.column(0) + df.column(0);

  // int expected_1[3] = {1, 4, 7};
  // int expected_2[3] = {3, 6, 9};
  int expected_3[3] = {2, 6, 10};
  int expected_4[3] = {2, 6, 10};
  int expected_5[3] = {2, 8, 14};

  for (sizetype i = 0; i < df.col_size(); i++) {
    // EXPECT_EQ(result_1[i], expected_1[i]);
    // EXPECT_EQ(result_2[i], expected_2[i]);
    EXPECT_EQ(result_3[i], expected_3[i]);
    EXPECT_EQ(result_4[i], expected_4[i]);
    EXPECT_EQ(result_5[i], expected_5[i]);
  }
}

TEST(col_operators_tests, multiplication_operators) {
  constexpr int N = 9;
  // 1 2 3
  // 4 5 6
  // 7 8 9

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  Series<int> result_1 = df.column(0) * Series<int>{1, 2, 3};
  Series<int> result_2 = Series<int>{1, 2, 3} * df.column(0);
  Series<int> result_3 = df.column(0) * df.column(0);

  int expected_1[3] = {1, 8, 21};
  int expected_2[3] = {1, 8, 21};
  int expected_3[3] = {1, 16, 49};

  for (sizetype i = 0; i < df.col_size(); i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_2[i]);
    EXPECT_EQ(result_3[i], expected_3[i]);
  }
}

TEST(col_operators_tests, subtraction_operators) {
  constexpr int N = 9;
  // 1 2 3
  // 4 5 6
  // 7 8 9

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  // Series<int> result_1 = df.column(0)--;
  // Series<int> result_2 = --df.column(0);
  // df.column(0) -= 1;
  Series<int> result_3 = df.column(0) - Series<int>{1, 2, 3};
  Series<int> result_4 = Series<int>{1, 2, 3} - df.column(0);
  Series<int> result_5 = df.column(0) - df.column(0);

  // int expected_1[3] = {///////};
  // int expected_2[3] = {///////};
  int expected_3[3] = {0, 2, 4};
  int expected_4[3] = {0, -2, -4};
  int expected_5[3] = {0, 0, 0};

  for (sizetype i = 0; i < df.col_size(); i++) {
    // EXPECT_EQ(result_1[i], expected_1[i]);
    // EXPECT_EQ(result_2[i], expected_2[i]);
    EXPECT_EQ(result_3[i], expected_3[i]);
    EXPECT_EQ(result_4[i], expected_4[i]);
    EXPECT_EQ(result_5[i], expected_5[i]);
  }
}

TEST(col_operators_tests, division_operators) {
  constexpr int N = 9;
  // 1 2 3
  // 4 5 6
  // 7 8 9

  DataFrame<int> df = create_dataframe<int, 3, 3>();

  int values[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = values[i];
  }

  Series<int> result_1 = df.column(0) / Series<int>{1, 4, 7};
  Series<int> result_2 = Series<int>{1, 4, 7} / df.column(0);
  Series<int> result_3 = df.column(1) / df.column(0);

  int expected_1[3] = {1, 1, 1};
  int expected_2[3] = {1, 1, 1};
  int expected_3[3] = {2, 1, 1};

  for (sizetype i = 0; i < df.col_size(); i++) {
    EXPECT_EQ(result_1[i], expected_1[i]);
    EXPECT_EQ(result_2[i], expected_2[i]);
    EXPECT_EQ(result_3[i], expected_3[i]);
  }
}

#endif // COLUMN_TESTS_H