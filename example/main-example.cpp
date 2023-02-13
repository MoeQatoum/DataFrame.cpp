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

  DataFrame<ui32> df{col_names, row_names};
  int             i = 0;
  for (size_t i = 0; i < df.size(); ++i) {
    df[i] = static_cast<ui32>(i);
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

  //////////
  // copy //
  //////////

  clog << "copy:\n";

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

  DataFrame<ui32> n_df{n_col_names, n_row_names};

  for (size_t i = 0; i < n_df.size(); ++i) {
    n_df[i] = static_cast<ui32>(i);
  }

  n_df.print();
  DataFrame<ui32> new_df = n_df.copy();
  n_df.print();
  clog << "kkkkkkkkkkkk" << n_df.copy().get_row_name(1).value() << "\n";
  clog << "n_df_cpy[3] " << n_df.copy()[3] << "\n";
  for (size_t i = 0; i < new_df.size(); i++) {
    clog << new_df[i].value << ", ";
    new_df[i].value = 999;
  }
  clog << "\n";
  for (size_t i = 0; i < new_df.col_size(); i++) {
    clog << new_df.get_row_name(i).value() << ", ";
  }
  clog << "\n";
  for (size_t i = 0; i < new_df.row_size(); i++) {
    clog << new_df.get_col_name(i).value() << ", ";
  }
  clog << "\n"
       << "df addr: " << &n_df << " size: " << sizeof(n_df) << ", new_df addr: " << &new_df
       << " size: " << sizeof(new_df) << "\n";
  n_df.print();
  new_df.print();

  //////////
  // sort //
  //////////

  clog << "---------------------------------------------------------------------\nsort:\n";

  DataFrame<ui32> unsorted_df = df.copy();
  for (size_t i = 0; i < unsorted_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<ui32>(unsorted_df.size());
  }

  unsorted_df.print();

  List<RowSeries<ui32>> sorted_rows = asc_sort_rows(unsorted_df, "col-2");

  // dont modify the original df from sorted rows
  DataFrame<ui32> sorted_df(unsorted_df);
  size_t          idx = 0;
  for (auto row : sorted_rows) {
    for (auto c : row) {
      sorted_df[idx].value = c->value;
      idx++;
    }
  }

  sorted_df.print();

  clog << "---------------------------------------------------------------------\ninplace sort:\n";

  DataFrame<ui32> inplace_sort_df = df.copy();
  for (size_t i = 0; i < inplace_sort_df.size(); i++) {
    inplace_sort_df[i] = rand() % static_cast<ui32>(unsorted_df.size());
  }

  for (size_t i = 0; i < unsorted_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<ui32>(unsorted_df.size());
  }

  inplace_sort_df.print();
  inplace_sort_df.sort_rows("col-2").print();

  return 0;
}
