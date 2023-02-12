#include <dataframe.h>
#include <gtest/gtest.h>

#define TEST_COL_COUNT 10
#define TEST_ROW_COUNT 10

using namespace df;

DataFrame<df_i32> create_dataframe() {
  std::vector<std::string> col_names{};
  for (std::size_t i = 0; i < TEST_COL_COUNT; i++) {
    col_names.push_back(std::string{"col-" + std::to_string(i + 1)});
  }

  std::vector<std::string> row_names{};
  for (std::size_t i = 0; i < TEST_ROW_COUNT; i++) {
    row_names.push_back(std::string{"row-" + std::to_string(i + 1)});
  }

  DataFrame<df_i32> df{col_names, row_names};

  return df;
}

TEST(df_iter_tests, dfIter) {
  DataFrame<df_i32> df = create_dataframe();

  for (df_ui32 i = 0; i < df.size(); i++) {
    df[i] = (df_i32)i;
  }

  df_i32 value = 0;
  for (auto c : df) {
    EXPECT_EQ(value, c.value);
    value++;
  }
}

TEST(df_iter_tests, dfRowIter) {
  DataFrame<df_i32> df = create_dataframe();

  for (df_ui32 i = 0; i < df.size(); i++) {
    df[i] = (df_i32)i;
  }

  df_ui32 idx   = 0;
  df_i32  value = 0;
  for (DataFrameRowIterator<DataFrame<df_i32>> row_iterator = df.iter_rows(); row_iterator < df.end(); row_iterator++) {
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
  DataFrame<df_i32> df = create_dataframe();

  for (df_ui32 i = 0; i < df.size(); i++) {
    df[i] = (df_i32)i;
  }

  for (DataFrameColIterator<DataFrame<df_i32>> col_iterator = df.iter_cols(); col_iterator < df.end(); col_iterator++) {
    for (auto c : col_iterator.current_col()) {
      EXPECT_EQ(c, &df[c->idx.global_idx]);
      EXPECT_EQ(c->value, df[c->idx.global_idx].value);
    }
  }
}

TEST(df_copy_tests, dfCopyShapeEquality) {
  DataFrame<df_i32> df_orig = create_dataframe();
  DataFrame<df_i32> df_copy = df_orig.copy();

  EXPECT_EQ(df_orig.shape().col_count, df_copy.shape().col_count);
  EXPECT_EQ(df_orig.shape().row_count, df_copy.shape().row_count);
  EXPECT_EQ(df_orig.size(), df_copy.size());
  EXPECT_EQ(df_orig.col_size(), df_copy.col_size());
  EXPECT_EQ(df_orig.row_size(), df_copy.row_size());
}

TEST(df_copy_tests, dfCopyColNamesEquality) {
  DataFrame<df_i32> df_orig = create_dataframe();
  DataFrame<df_i32> df_copy = df_orig.copy();

  for (size_t i = 0; i < df_orig.col_size(); i++) {
    EXPECT_EQ(df_orig.get_col_name(i), df_copy.get_col_name(i));
  }
}

TEST(df_copy_tests, dfCopyRowNamesEquality) {
  DataFrame<df_i32> df_orig = create_dataframe();
  DataFrame<df_i32> df_copy = df_orig.copy();

  for (size_t i = 0; i < df_orig.row_size(); i++) {
    EXPECT_EQ(df_orig.get_row_name(i), df_copy.get_row_name(i));
  }
}

TEST(df_copy_tests, dfCopy_verifyDataAddr) {
  DataFrame<df_i32> df_orig = create_dataframe();
  DataFrame<df_i32> df_copy = df_orig.copy();

  for (size_t i = 0; i < df_orig.size(); i++) {
    EXPECT_NE(&df_orig[i], &df_copy[i]);
  }
}