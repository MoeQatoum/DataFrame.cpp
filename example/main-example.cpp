#include <array>
#include <iostream>
#include <memory>
#include <stdlib.h>

#include "utils.h"
#include <dataframe.h>

int main() {
#define TEST_COL_COUNT 3
#define TEST_ROW_COUNT 5
  std::vector<std::string> col_names{};
  for (std::size_t i = 0; i < TEST_COL_COUNT; i++) {
    col_names.push_back(std::string{"col-" + std::to_string(i + 1)});
  }

  std::vector<std::string> row_names{};
  for (std::size_t i = 0; i < TEST_ROW_COUNT; i++) {
    row_names.push_back(std::string{"row-" + std::to_string(i + 1)});
  }

  df::DataFrame<df_i32> df{col_names, row_names};
  int                   i = 0;
  for (std::size_t i = 0; i < df.size(); ++i) {
    df[i] = static_cast<df_i32>(i);
  }
  df.print();

  for (auto col_iter = df.iter_cols(); col_iter < df.end(); ++col_iter) {
    for (auto& c : col_iter.current_col()) {
      c->value = 456;
    }
    // i.column()[0]->value = 123;
    auto d = col_iter.current_col().copy_data();
    for (int i = 0; i < d.size; i++) {
      std::cout << d.data[i] << ", ";
    }
    std::cout << "\n";
  }
  df.print();

  for (auto row_iter = df.iter_rows(); row_iter < df.end(); row_iter++) {
    for (auto& c : row_iter.current_row()) {
      c->value = 123;
    }
    // i.row()[0]->value = 123;
    auto d = row_iter.current_row().copy_data();
    for (int i = 0; i < d.size; i++) {
      std::cout << d.data[i] << ", ";
    }
    std::cout << "\n";
  }

  df.print();

  //////////
  // copy //
  //////////

  std::cout << "copy:\n";

  std::vector<std::string> n_col_names{};
  for (std::size_t i = 0; i < TEST_COL_COUNT; i++) {
    n_col_names.push_back(std::string{"col-" + std::to_string(i + 1)});
  }

  std::vector<std::string> n_row_names{};
  for (std::size_t i = 0; i < TEST_ROW_COUNT; i++) {
    n_row_names.push_back(std::string{"row-" + std::to_string(i + 1)});
  }

  df::DataFrame<df_i32> n_df{n_col_names, n_row_names};

  for (std::size_t i = 0; i < n_df.size(); ++i) {
    n_df[i] = static_cast<df_i32>(i);
  }

  n_df.print();
  df::DataFrame<df_i32> new_df = n_df.copy();
  n_df.print();
  std::cout << "kkkkkkkkkkkk" << n_df.copy().get_row_name(1).value() << "\n";
  std::cout << "n_df_cpy[3] " << n_df.copy()[3] << "\n";
  for (size_t i = 0; i < new_df.size(); i++) {
    std::cout << new_df[i].value << ", ";
    new_df[i].value = 999;
  }
  std::cout << "\n";
  for (size_t i = 0; i < new_df.col_size(); i++) {
    std::cout << new_df.get_row_name(i).value() << ", ";
  }
  std::cout << "\n";
  for (size_t i = 0; i < new_df.row_size(); i++) {
    std::cout << new_df.get_col_name(i).value() << ", ";
  }
  std::cout << "\n"
            << "df addr: " << &n_df << " size: " << sizeof(n_df) << ", new_df addr: " << &new_df
            << " size: " << sizeof(new_df) << "\n";
  n_df.print();
  new_df.print();

  //////////
  // sort //
  //////////

  std::cout << "sort:\n";

  df::DataFrame<df_i32> unsorted_df = df.copy();
  for (size_t i = 0; i < unsorted_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<df_i32>(unsorted_df.size());
  }

  unsorted_df.print();

  std::vector<df::RowSeries<df_i32>> sorted_rows = df::utils::asc_sort_rows(unsorted_df, "col-2");

  // dont modify the original df from sorted rows
  df::DataFrame<df_i32> sorted_df(unsorted_df);
  size_t                idx = 0;
  for (auto row : sorted_rows) {
    for (auto c : row) {
      sorted_df[idx].value = c->value;
      idx++;
    }
  }

  sorted_df.print();

  std::cout << "---------------------------------------------------------------------\ninplace sort:\n";

  df::DataFrame<df_i32> inplace_sort_df = df.copy();
  for (size_t i = 0; i < inplace_sort_df.size(); i++) {
    inplace_sort_df[i] = rand() % static_cast<df_i32>(unsorted_df.size());
  }

  for (size_t i = 0; i < unsorted_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<df_i32>(unsorted_df.size());
  }

  inplace_sort_df.print();
  inplace_sort_df.sort_rows("col-2").print();

  return 0;
}
