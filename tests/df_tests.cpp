#include <dataframe.h>
#undef clog

#include <gtest/gtest.h>

#define TEST_COL_COUNT 10
#define TEST_ROW_COUNT 10

using namespace df;

template<typename T>
DataFrame<T> create_dataframe() {
  StringList col_names{};
  for (sizetype i = 1; i <= TEST_COL_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    col_names.push_back(String("col-%1").arg(i));
#else
    col_names.push_back(String{"col-" + std::to_string(i)});
#endif
  }

  StringList row_names{};
  for (sizetype i = 1; i <= TEST_ROW_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    row_names.push_back(String("row-%1").arg(i));
#else
    row_names.push_back(String{"row-" + std::to_string(i)});
#endif
  }

  DataFrame<T> df{col_names, row_names};

  return df;
}

TEST(df_iter_tests, dfIter) {
  DataFrame<int> df = create_dataframe<int>();

  for (int i = 0; i < df.size(); i++) {
    df[(sizetype)i] = i;
  }

  int value = 0;
  for (auto c : df) {
    EXPECT_EQ(value, c.value);
    value++;
  }
}

TEST(df_iter_tests, dfIterModifyValues) {
  DataFrame<int> df = create_dataframe<int>();

  for (int i = 0; i < df.size(); i++) {
    df[i] = i;
  }

  for (auto& c : df) {
    c.value = 999;
  }

  for (auto& c : df) {
    EXPECT_EQ(999, c.value);
  }
}

TEST(df_iter_tests, dfRowIter) {
  DataFrame<long> df = create_dataframe<long>();

  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = (long)i;
  }

  sizetype idx   = 0;
  sizetype value = 0;
  for (DataFrameRowIterator<DataFrame<long>> row_iterator = df.iter_rows(); row_iterator < df.end(); row_iterator++) {
    for (auto c : row_iterator.current_row()) {
      EXPECT_EQ(c, &df[idx]);
      EXPECT_EQ(c->value, df[idx].value);
      EXPECT_EQ(c->value, value);
      idx++;
      value++;
    }
  }
}

TEST(df_iter_tests, dfColIter) {
  DataFrame<long> df = create_dataframe<long>();

  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = (long)i;
  }

  for (DataFrameColIterator<DataFrame<long>> col_iterator = df.iter_cols(); col_iterator < df.end(); col_iterator++) {
    for (auto c : col_iterator.current_col()) {
      EXPECT_EQ(c, &df[c->idx.global_idx]);
      EXPECT_EQ(c->value, df[c->idx.global_idx].value);
    }
  }
}

TEST(df_copy_tests, dfCopyShapeEquality) {
  DataFrame<long> df_orig = create_dataframe<long>();
  DataFrame<long> df_copy = df_orig.copy();

  EXPECT_EQ(df_orig.shape().col_count, df_copy.shape().col_count);
  EXPECT_EQ(df_orig.shape().row_count, df_copy.shape().row_count);
  EXPECT_EQ(df_orig.size(), df_copy.size());
  EXPECT_EQ(df_orig.col_size(), df_copy.col_size());
  EXPECT_EQ(df_orig.row_size(), df_copy.row_size());
}

TEST(df_copy_tests, dfCopyColNamesEquality) {
  DataFrame<long> df_orig = create_dataframe<long>();
  DataFrame<long> df_copy = df_orig.copy();

  for (sizetype i = 0; i < df_orig.col_size(); i++) {
    EXPECT_EQ(df_orig.get_col_name(i), df_copy.get_col_name(i));
  }
}

TEST(df_copy_tests, dfCopyRowNamesEquality) {
  DataFrame<long> df_orig = create_dataframe<long>();
  DataFrame<long> df_copy = df_orig.copy();

  for (sizetype i = 0; i < df_orig.row_size(); i++) {
    EXPECT_EQ(df_orig.get_row_name(i), df_copy.get_row_name(i));
  }
}

TEST(df_copy_tests, dfCopy_verifyDataAddr) {
  DataFrame<long> df_orig = create_dataframe<long>();
  DataFrame<long> df_copy = df_orig.copy();

  for (sizetype i = 0; i < df_orig.size(); i++) {
    EXPECT_NE(&df_orig[i], &df_copy[i]);
  }
}

TEST(df_sort, dfRowsAscendingSort) {
  DataFrame<long> df = create_dataframe<long>();

  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = (long)rand();
  }

  sizetype col_idx  = 2;
  String   col_name = df.get_col_name(col_idx).value();

  List<RowSeries<long>> sorted_rows = asc_sort_rows(df, col_name);

  EXPECT_EQ(sorted_rows.size(), df.row_count());

  long sorted_values[sorted_rows.size()];

  for (sizetype idx = 0; idx < sorted_rows.size(); idx++) {
    sorted_values[idx] = sorted_rows[idx][col_idx]->value;
  }
  for (RowSeries<long> row : sorted_rows) {
    for (auto c : row) {
      EXPECT_EQ(c, &df[c->idx.global_idx]);
      EXPECT_EQ(c->value, df[c->idx.global_idx].value);
    }
  }
}