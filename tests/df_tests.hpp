#ifndef DATA_FRAME_TESTS_H
#define DATA_FRAME_TESTS_H

#include "test_utils.hpp"
#include <dataframe>
#include <gtest/gtest.h>

using namespace df;

TEST(df_iter_tests, dfIter) {
    DataFrame<int> df = create_dataframe<int, 10, 10>();

    for (std::size_t i = 0; i < df.size(); i++) {
        df[(std::size_t)i] = i;
    }

    int value = 0;
    for (auto c : df) {
        EXPECT_EQ(value, c.value);
        value++;
    }
}

TEST(df_iter_tests, dfIterModifyValues) {
    DataFrame<int> df = create_dataframe<int, 10, 10>();

    for (std::size_t i = 0; i < df.size(); i++) {
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
    DataFrame<int> df = create_dataframe<int, 10, 10>();

    for (std::size_t i = 0; i < df.size(); i++) {
        df[i] = (long)i;
    }

    std::size_t idx   = 0;
    std::size_t value = 0;
    for (auto row_iterator = df.iter_rows(); row_iterator < df.end(); row_iterator++) {
        for (auto& c : row_iterator.current_row()) {
            EXPECT_EQ(c, &df[idx]);
            EXPECT_EQ(c->value, df[idx].value);
            EXPECT_EQ(c->value, value);
            idx++;
            value++;
        }
    }
}

TEST(df_iter_tests, dfColIter) {
    DataFrame<int> df = create_dataframe<int, 10, 10>();

    for (std::size_t i = 0; i < df.size(); i++) {
        df[i] = (long)i;
    }

    std::size_t idx = 0;
    for (auto col_iterator = df.iter_cols(); col_iterator < df.end(); col_iterator++) {
        idx = col_iterator.current_col_idx();
        for (auto c : col_iterator.current_col()) {
            EXPECT_EQ(c, &df[idx]);
            EXPECT_EQ(c, &df[c->idx.global_idx]);
            EXPECT_EQ(c->value, df[c->idx.global_idx].value);
            idx += df.row_size();
        }
    }
}

TEST(df_copy_tests, dfCopyShapeEquality) {
    DataFrame<int> df_orig = create_dataframe<int, 10, 10>();
    DataFrame<int> df_copy = df_orig.copy();

    EXPECT_EQ(df_orig.shape().col_count, df_copy.shape().col_count);
    EXPECT_EQ(df_orig.shape().row_count, df_copy.shape().row_count);
    EXPECT_EQ(df_orig.size(), df_copy.size());
    EXPECT_EQ(df_orig.col_size(), df_copy.col_size());
    EXPECT_EQ(df_orig.row_size(), df_copy.row_size());
}

TEST(df_copy_tests, dfCopyColNamesEquality) {
    DataFrame<int> df_orig = create_dataframe<int, 10, 10>();
    DataFrame<int> df_copy = df_orig.copy();

    for (std::size_t i = 0; i < df_orig.col_size(); i++) {
        EXPECT_EQ(df_orig.get_col_name(i), df_copy.get_col_name(i));
    }
}

TEST(df_copy_tests, dfCopyRowNamesEquality) {
    DataFrame<int> df_orig = create_dataframe<int, 10, 10>();
    DataFrame<int> df_copy = df_orig.copy();

    for (std::size_t i = 0; i < df_orig.row_size(); i++) {
        EXPECT_EQ(df_orig.get_row_name(i), df_copy.get_row_name(i));
    }
}

TEST(df_copy_tests, dfCopy_verifyDataAddr) {
    DataFrame<int> df_orig = create_dataframe<int, 10, 10>();
    DataFrame<int> df_copy = df_orig.copy();

    for (std::size_t i = 0; i < df_orig.size(); i++) {
        EXPECT_NE(&df_orig[i], &df_copy[i]);
    }
}

TEST(df_sort, dfRowsAscendingSort) {
    DataFrame<int> df = create_dataframe<int, 10, 10>();

    for (std::size_t i = 0; i < df.size(); i++) {
        df[i] = (int)rand() % 100;
    }

    std::size_t col_idx  = 2;
    std::string col_name = df.get_col_name(col_idx);

    RowGroup<DataFrame<int>::row_type> sorted_rows = df.sort(col_name, true);

    EXPECT_EQ(sorted_rows.size(), df.row_count());

    std::vector<int> sorted_values(sorted_rows.size());

    for (std::size_t idx = 0; idx < sorted_rows.size(); idx++) {
        sorted_values[idx] = sorted_rows[idx][col_idx]->value;
    }

    for (const auto& row : sorted_rows) {
        for (auto c : row) {
            EXPECT_EQ(c, &df[c->idx.global_idx]);
            EXPECT_EQ(c->value, df[c->idx.global_idx].value);
        }
    }
}

#endif // DATA_FRAME_TESTS_H
