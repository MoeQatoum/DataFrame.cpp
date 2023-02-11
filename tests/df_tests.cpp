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

TEST(df_copy, dfCopyShapeEquality) {
  DataFrame<df_i32> df_orig = create_dataframe();
  DataFrame<df_i32> df_copy = df_orig.copy();

  EXPECT_EQ(df_orig.shape().col_count, df_copy.shape().col_count);
  EXPECT_EQ(df_orig.shape().row_count, df_copy.shape().row_count);
  EXPECT_EQ(df_orig.size(), df_copy.size());
  EXPECT_EQ(df_orig.col_size(), df_copy.col_size());
  EXPECT_EQ(df_orig.row_size(), df_copy.row_size());
}

TEST(df_copy, dfCopyColNamesEquality) {
  DataFrame<df_i32> df_orig = create_dataframe();
  DataFrame<df_i32> df_copy = df_orig.copy();

  for (size_t i = 0; i < df_orig.col_size(); i++) {
    EXPECT_EQ(df_orig.get_col_name(i), df_copy.get_col_name(i));
  }
}

TEST(df_copy, dfCopyRowNamesEquality) {
  DataFrame<df_i32> df_orig = create_dataframe();
  DataFrame<df_i32> df_copy = df_orig.copy();

  for (size_t i = 0; i < df_orig.row_size(); i++) {
    EXPECT_EQ(df_orig.get_row_name(i), df_copy.get_row_name(i));
  }
}