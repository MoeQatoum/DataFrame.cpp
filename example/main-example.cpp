#include <array>
#include <dataframe.h>

using namespace df;

int main() {

#define TEST_COL_COUNT 3
#define TEST_ROW_COUNT 10
  StringList col_names{};
  for (sizetype i = 1; i <= TEST_COL_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    col_names.push_back(String{"col-%1"}.arg(i));
#else
    col_names.push_back(String{"col-" + std::to_string(i)});
#endif
  }

  StringList row_names{};
  for (sizetype i = 1; i <= TEST_ROW_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    row_names.push_back(String{"row-%1"}.arg(i));
#else
    row_names.push_back(String{"row-" + std::to_string(i)});
#endif
  }

  DataFrame<long> df{col_names, row_names};
  int             i = 0;
  for (sizetype i = 0; i < df.size(); ++i) {
    df[i] = static_cast<long>(i);
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
  for (sizetype i = 1; i <= TEST_COL_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    n_col_names.push_back(String{"row-%1"}.arg(i));
#else
    n_col_names.push_back(String{"col-" + std::to_string(i)});
#endif
  }

  StringList n_row_names{};
  for (sizetype i = 1; i <= TEST_ROW_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    n_row_names.push_back(String{"row-%1"}.arg(i));
#else
    n_row_names.push_back(String{"row-" + std::to_string(i)});
#endif
  }

  DataFrame<long> n_df{n_col_names, n_row_names};

  for (sizetype i = 0; i < n_df.size(); ++i) {
    n_df[i] = static_cast<long>(i);
  }

  n_df.print();
  DataFrame<long> new_df = n_df.copy();
  n_df.print();
  for (sizetype i = 0; i < new_df.size(); i++) {
    clog << new_df[i].value << ", ";
    new_df[i].value = 999;
  }

  n_df.print();
  new_df.print();

  clog << "---------------------------------------------------------------------\naec sort:\n";
  DataFrame<long> unsorted_df = df.copy();
  for (::sizetype i = 0; i < unsorted_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<long>(unsorted_df.size());
  }

  unsorted_df.print();

  List<RowSeries<long>> sorted_rows = asc_sort_rows(unsorted_df, "col-2");

  // dont modify the original df from sorted rows
  DataFrame<long> sorted_df(unsorted_df);
  sizetype        idx = 0;
  for (auto row : sorted_rows) {
    for (auto c : row) {
      sorted_df[idx].value = c->value;
      idx++;
    }
  }

  sorted_df.print();

  clog << "---------------------------------------------------------------------\ndec sort:\n";
  DataFrame<long> dec_unsorted_df = df.copy();
  for (sizetype i = 0; i < dec_unsorted_df.size(); i++) {
    dec_unsorted_df[i] = rand() % static_cast<long>(dec_unsorted_df.size());
  }

  dec_unsorted_df.print();

  List<RowSeries<long>> dec_sorted_rows = dec_sort_rows(dec_unsorted_df, "col-2");

  // dont modify the original df from sorted rows
  DataFrame<long> dec_sorted_df(dec_unsorted_df);
  sizetype        dec_idx = 0;
  for (auto row : dec_sorted_rows) {
    for (auto c : row) {
      sorted_df[dec_idx].value = c->value;
      dec_idx++;
    }
  }

  sorted_df.print();

  clog << "---------------------------------------------------------------------\ninplace aec sort:\n";
  DataFrame<long> inplace_sort_df = df.copy();
  for (sizetype i = 0; i < inplace_sort_df.size(); i++) {
    inplace_sort_df[i] = rand() % static_cast<long>(unsorted_df.size());
  }

  for (sizetype i = 0; i < unsorted_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<long>(unsorted_df.size());
  }

  inplace_sort_df.print();
  inplace_sort_df.asc_sort_rows("col-2").print();

  clog << "---------------------------------------------------------------------\ninplace dec sort:\n";
  DataFrame<long> dec_inplace_sort_df = df.copy();
  for (sizetype i = 0; i < dec_inplace_sort_df.size(); i++) {
    dec_inplace_sort_df[i] = rand() % static_cast<long>(unsorted_df.size());
  }

  for (sizetype i = 0; i < dec_inplace_sort_df.size(); i++) {
    unsorted_df[i] = rand() % static_cast<long>(unsorted_df.size());
  }

  dec_inplace_sort_df.print();
  dec_inplace_sort_df.dec_sort_rows("col-2").print();

  clog << "---------------------------------------------------------------------\nhead:\n";
  dec_inplace_sort_df.print(3, 0);

  clog << "---------------------------------------------------------------------\ntail:\n";
  dec_inplace_sort_df.print(0, 3);

  clog << "---------------------------------------------------------------------\nhead and tail:\n";
  dec_inplace_sort_df.print(3, 3);
  clog << dec_inplace_sort_df;

  return 0;
}
