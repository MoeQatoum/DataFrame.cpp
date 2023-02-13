#include <array>
#include <iostream>

#include <dataframe.h>

using namespace df;

int main() {

#define TEST_COL_COUNT 3
#define TEST_ROW_COUNT 10
  StringList col_names{};
  for (size_t i = 1; i <= TEST_COL_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    col_names.push_back(String{"col-%1"}.arg(i));
#else
    col_names.push_back(String{"col-" + std::to_string(i)});
#endif
  }

  StringList row_names{};
  for (size_t i = 1; i <= TEST_ROW_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    row_names.push_back(String{"row-%1"}.arg(i));
#else
    row_names.push_back(String{"row-" + std::to_string(i)});
#endif
  }

  DataFrame<i32> df{col_names, row_names};
  int            i = 0;
  for (size_t i = 0; i < df.size(); ++i) {
    df[i] = static_cast<i32>(i);
  }
  df.print();

  for (auto col_iter = df.iter_cols(); col_iter < df.end(); ++col_iter) {
    for (auto& c : col_iter.current_col()) {
      c->value = 456;
    }
    // i.column()[0]->value = 123;
    auto d = col_iter.current_col().copy_data();
    for (int i = 0; i < d.size; i++) {
      clog << d.data[i] << ", ";
    }
    clog << "\n";
  }
  df.print();

  for (auto row_iter = df.iter_rows(); row_iter < df.end(); row_iter++) {
    for (auto& c : row_iter.current_row()) {
      c->value = 123;
    }
    // i.row()[0]->value = 123;
    auto d = row_iter.current_row().copy_data();
    for (int i = 0; i < d.size; i++) {
      clog << d.data[i] << ", ";
    }
    clog << "\n";
  }

  df.print();

  clog << "---------------------------------------------------------------------\ncopy:\n";
  StringList n_col_names{};
  for (size_t i = 1; i <= TEST_COL_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    n_col_names.push_back(String{"row-%1"}.arg(i));
#else
    n_col_names.push_back(String{"row-" + std::to_string(i)});
#endif
  }

  StringList n_row_names{};
  for (size_t i = 1; i <= TEST_ROW_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    n_row_names.push_back(String{"row-%1"}.arg(i));
#else
    n_row_names.push_back(String{"row-" + std::to_string(i)});
#endif
  }

  DataFrame<i32> n_df{n_col_names, n_row_names};

  for (size_t i = 0; i < n_df.size(); ++i) {
    n_df[i] = static_cast<i32>(i);
  }

  n_df.print();
  DataFrame<i32> new_df = n_df.copy();
  n_df.print();
  for (size_t i = 0; i < new_df.size(); i++) {
    clog << new_df[i].value << ", ";
    new_df[i].value = 999;
  }

  n_df.print();
  new_df.print();

  clog << "---------------------------------------------------------------------\naec sort:\n";
  DataFrame<i32> unsorted_df = df.copy();
  for (size_t i = 0; i < unsorted_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<i32>(unsorted_df.size());
  }

  unsorted_df.print();

  List<RowSeries<i32>> sorted_rows = asc_sort_rows(unsorted_df, "col-2");

  // dont modify the original df from sorted rows
  DataFrame<i32> sorted_df(unsorted_df);
  size_t         idx = 0;
  for (auto row : sorted_rows) {
    for (auto c : row) {
      sorted_df[idx].value = c->value;
      idx++;
    }
  }

  sorted_df.print();

  clog << "---------------------------------------------------------------------\ndec sort:\n";
  DataFrame<i32> dec_unsorted_df = df.copy();
  for (size_t i = 0; i < dec_unsorted_df.size(); i++) {
    dec_unsorted_df[i] = rand() % static_cast<i32>(dec_unsorted_df.size());
  }

  dec_unsorted_df.print();

  List<RowSeries<i32>> dec_sorted_rows = dec_sort_rows(dec_unsorted_df, "col-2");

  // dont modify the original df from sorted rows
  DataFrame<i32> dec_sorted_df(dec_unsorted_df);
  size_t         dec_idx = 0;
  for (auto row : dec_sorted_rows) {
    for (auto c : row) {
      sorted_df[dec_idx].value = c->value;
      dec_idx++;
    }
  }

  sorted_df.print();

  clog << "---------------------------------------------------------------------\ninplace aec sort:\n";
  DataFrame<i32> inplace_sort_df = df.copy();
  for (size_t i = 0; i < inplace_sort_df.size(); i++) {
    inplace_sort_df[i] = rand() % static_cast<i32>(unsorted_df.size());
  }

  for (size_t i = 0; i < unsorted_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<i32>(unsorted_df.size());
  }

  inplace_sort_df.print();
  inplace_sort_df.aec_sort_rows("col-2").print();

  clog << "---------------------------------------------------------------------\ninplace dec sort:\n";
  DataFrame<i32> dec_inplace_sort_df = df.copy();
  for (size_t i = 0; i < dec_inplace_sort_df.size(); i++) {
    dec_inplace_sort_df[i] = rand() % static_cast<i32>(unsorted_df.size());
  }

  for (size_t i = 0; i < dec_inplace_sort_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<i32>(unsorted_df.size());
  }

  dec_inplace_sort_df.print();
  dec_inplace_sort_df.dec_sort_rows("col-2").print();

  clog << "---------------------------------------------------------------------\nhead:\n";
  dec_inplace_sort_df.print(3, 0);

  clog << "---------------------------------------------------------------------\ntail:\n";
  dec_inplace_sort_df.print(0, 3);

  clog << "---------------------------------------------------------------------\nhead and tail:\n";
  dec_inplace_sort_df.print(3, 3);

  return 0;
}
