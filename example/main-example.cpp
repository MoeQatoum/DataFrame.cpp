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

  DataFrame<int> df{col_names, row_names};
  int            i = 0;
  for (sizetype i = 0; i < df.size(); ++i) {
    df[i] = static_cast<int>(i);
  }
  df.log();

  for (auto col_iter = df.iter_cols(); col_iter < df.end(); ++col_iter) {
    for (auto& c : col_iter.current_col()) {
      c->value = 456;
    }
    // i.column()[0]->value = 123;
    auto d = col_iter.current_col().copy_data();
    for (int i = 0; i < d.size(); i++) {
      clog << d[i] << ", ";
    }
    clog << "\n";
  }
  df.log();

  for (auto row_iter = df.iter_rows(); row_iter < df.end(); row_iter++) {
    for (auto& c : row_iter.current_row()) {
      c->value = 123;
    }
    // i.row()[0]->value = 123;
    auto d = row_iter.current_row().copy_data();
    for (int i = 0; i < d.size(); i++) {
      clog << d[i] << ", ";
    }
    clog << "\n";
  }

  df.log();

  for (sizetype i = 0; i < df.size(); ++i) {
    df[i] = static_cast<int>(i);
  }

  clog << "-- copy assignment operator row:\n";
  df.row(0) = df.row(1);
  df.log();

  clog << "-- copy assignment operator column:\n";
  df.column(0) = df.column(1);
  df.log();

  clog << "------------------COPY_DF------------------\n";
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = rand() % static_cast<int>(df.size());
  }

  clog << "--old df:\n";
  df.log();
  DataFrame<int> new_df = df.copy();
  clog << "\n--new df:\n";
  new_df.log();

  clog << "\n";
  Series<int> comp = df.column(1) + df.column(2);
  for (sizetype i = 0; i < comp.size(); i++) {
    clog << comp[i] << " ";
  }
  clog << "\n";

  clog << "------------------SORTING------------------\n";

  DataFrame<int> unsorted_df = df.copy();

  clog << "-- unsorted df:\n";
  unsorted_df.log();

  clog << "\n-- aec sort:\n";
  List<Row<int>> aec_sorted_rows = ascending_sort(unsorted_df, "col-2");
  clog << "asc rows log algo: \n";
  unsorted_df.logger.log_sorted_rows(aec_sorted_rows);
  clog << "asc rows log algo head 5: \n";
  unsorted_df.logger.log_sorted_rows(aec_sorted_rows, 5);
  clog << "asc rows log algo tail 3: \n";
  unsorted_df.logger.log_sorted_rows(aec_sorted_rows, -3);

  clog << "\n-- dec sort:\n";
  List<Row<int>> dec_sorted_rows = descending_sort(unsorted_df, "col-2");
  clog << "dec rows log algo: \n";
  unsorted_df.logger.log_sorted_rows(dec_sorted_rows);

  clog << "\n-- inplace aec sort:\n";
  DataFrame<int> unsorted_aec_df = unsorted_df.copy();
  DataFrame<int> sorted_aec_df   = unsorted_aec_df.inplace_ascending_sort("col-2");
  sorted_aec_df.log();

  clog << "\n-- inplace dec sort:\n";
  DataFrame<int> unsorted_dec_df = unsorted_df.copy();
  DataFrame<int> sorted_dec_df   = unsorted_dec_df.inplace_descending_rows("col-2");
  sorted_dec_df.log();

  clog << "------------------LOGGING------------------\n";

  clog << "-- head:\n";
  unsorted_dec_df.log(3);

  clog << "-- tail:\n";
  unsorted_dec_df.log(-4);

  clog << "-- log row with operator<<:\n";
  clog << unsorted_dec_df.row(1) << "\n";

  clog << "-- log col with operator<<:\n";
  clog << unsorted_dec_df.column(1) << "\n";

  sorted_aec_df = sorted_dec_df;
  sorted_aec_df.logger
    .with_cell_color_condition([df](Cell<int>* c) {
      if ((c->idx.col_name == "col-2") && (c->value > 10)) {
        return DF_COLOR_G;
      } else {
        return DF_COLOR_W;
      }
    })
    .log(10);

  clog << "------------------UTILS------------------\n";
  clog << "-- before fill_df():\n";
  unsorted_dec_df.log();
  clog << "-- after fill_df():\n";
  fill_df(unsorted_dec_df, 0);
  unsorted_dec_df.log();

  clog << "------------------SERIES------------------\n";
  Series<int> series_1(20);
  fill_series(series_1, 1);
  Series<int> series_2(20);
  fill_series(series_2, 2);

  Series<bool> bar = (series_2 == 2);
  series_2 += 10;

  for (sizetype i = 0; i < bar.size(); i++) {
    clog << series_2[i] << " ";
  }
  clog << "\n";
  return 0;
}
