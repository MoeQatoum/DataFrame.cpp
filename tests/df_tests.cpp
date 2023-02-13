#include <dataframe.h>
#undef clog

#include <gtest/gtest.h>

#define TEST_COL_COUNT 10
#define TEST_ROW_COUNT 10

using namespace df;

DataFrame<i32> create_dataframe() {
  StringList col_names{};
  for (std::size_t i = 1; i <= TEST_COL_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    col_names.push_back(String("col-%1").arg(i));
#else
    col_names.push_back(String{"col-" + std::to_string(i)});
#endif
  }

  StringList row_names{};
  for (std::size_t i = 1; i <= TEST_ROW_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    row_names.push_back(String("row-%1").arg(i));
#else
    row_names.push_back(String{"row-" + std::to_string(i)});
#endif
  }

  DataFrame<i32> df{col_names, row_names};

  return df;
}

TEST(df_iter_tests, dfIter) {
  DataFrame<i32> df = create_dataframe();

  for (ui32 i = 0; i < df.size(); i++) {
    df[i] = (i32)i;
  }

  ui32 value = 0;
  for (auto c : df) {
    EXPECT_EQ(value, c.value);
    value++;
  }
}

TEST(df_iter_tests, dfRowIter) {
  DataFrame<i32> df = create_dataframe();

  for (ui32 i = 0; i < df.size(); i++) {
    df[i] = (i32)i;
  }

  ui32 idx   = 0;
  ui32 value = 0;
  for (DataFrameRowIterator<DataFrame<i32>> row_iterator = df.iter_rows(); row_iterator < df.end(); row_iterator++) {
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
  DataFrame<i32> df = create_dataframe();

  for (ui32 i = 0; i < df.size(); i++) {
    df[i] = (i32)i;
  }

  for (DataFrameColIterator<DataFrame<i32>> col_iterator = df.iter_cols(); col_iterator < df.end(); col_iterator++) {
    for (auto c : col_iterator.current_col()) {
      EXPECT_EQ(c, &df[c->idx.global_idx]);
      EXPECT_EQ(c->value, df[c->idx.global_idx].value);
    }
  }
}

TEST(df_copy_tests, dfCopyShapeEquality) {
  DataFrame<i32> df_orig = create_dataframe();
  DataFrame<i32> df_copy = df_orig.copy();

  EXPECT_EQ(df_orig.shape().col_count, df_copy.shape().col_count);
  EXPECT_EQ(df_orig.shape().row_count, df_copy.shape().row_count);
  EXPECT_EQ(df_orig.size(), df_copy.size());
  EXPECT_EQ(df_orig.col_size(), df_copy.col_size());
  EXPECT_EQ(df_orig.row_size(), df_copy.row_size());
}

TEST(df_copy_tests, dfCopyColNamesEquality) {
  DataFrame<i32> df_orig = create_dataframe();
  DataFrame<i32> df_copy = df_orig.copy();

  for (ui32 i = 0; i < df_orig.col_size(); i++) {
    EXPECT_EQ(df_orig.get_col_name(i), df_copy.get_col_name(i));
  }
}

TEST(df_copy_tests, dfCopyRowNamesEquality) {
  DataFrame<i32> df_orig = create_dataframe();
  DataFrame<i32> df_copy = df_orig.copy();

  for (ui32 i = 0; i < df_orig.row_size(); i++) {
    EXPECT_EQ(df_orig.get_row_name(i), df_copy.get_row_name(i));
  }
}

TEST(df_copy_tests, dfCopy_verifyDataAddr) {
  DataFrame<i32> df_orig = create_dataframe();
  DataFrame<i32> df_copy = df_orig.copy();

  for (ui32 i = 0; i < df_orig.size(); i++) {
    EXPECT_NE(&df_orig[i], &df_copy[i]);
  }
}

TEST(df_sort, dfRowsAscendingSort) {
  DataFrame<i32> df = create_dataframe();

  for (ui32 i = 0; i < df.size(); i++) {
    df[i] = (i32)rand();
  }

  ui32   col_idx  = 2;
  String col_name = df.get_col_name(col_idx).value();

  List<RowSeries<i32>> sorted_rows = asc_sort_rows(df, col_name);

  EXPECT_EQ(sorted_rows.size(), df.row_count());

  i32 sorted_values[sorted_rows.size()];

  for (ui32 idx = 0; idx < sorted_rows.size(); idx++) {
    sorted_values[idx] = sorted_rows[idx][col_idx]->value;
  }
  for (RowSeries<i32> row : sorted_rows) {
    for (auto c : row) {
      EXPECT_EQ(c, &df[c->idx.global_idx]);
      EXPECT_EQ(c->value, df[c->idx.global_idx].value);
    }
  }
}