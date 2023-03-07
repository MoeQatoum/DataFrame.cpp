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

  clog << "-- max min df:\n";
  clog << unsorted_df["col-2"].max() << " " << unsorted_df["col-2"].min() << "\n";

  clog << "\n-- asc sort:\n";
  auto asc_sorted_rows = unsorted_df.sort("col-2", true);
  asc_sorted_rows.log();

  clog << "asc rows log algo head 5: \n";
  asc_sorted_rows.log(5);
  clog << "asc rows log algo tail 3: \n";
  asc_sorted_rows.log(-3);

  clog << "\n-- dec sort:\n";
  auto dec_sorted_rows = unsorted_df.rows().sort(unsorted_df["col-2"]);
  dec_sorted_rows.log();

  // clog << "\n-- inplace aec sort:\n";
  // DataFrame<int> unsorted_asc_df = unsorted_df.copy();
  // DataFrame<int> sorted_asc_df(unsorted_asc_df.rows().sort(unsorted_asc_df["col-2"], true));
  // sorted_asc_df.log();

  // clog << "\n-- inplace dec sort:\n";
  // DataFrame<int> unsorted_dec_df = unsorted_df.copy();
  // DataFrame<int> sorted_dec_df(unsorted_asc_df.rows().sort(unsorted_asc_df["col-2"]));
  // sorted_dec_df.log();

  clog << "------------------LOGGING------------------\n";
  DataFrame<int> unsorted_dec_df = unsorted_df.copy();

  clog << "-- head:\n";
  unsorted_dec_df.log(3);

  clog << "-- tail:\n";
  unsorted_dec_df.log(-4);

  clog << "-- log row with operator<<:\n";
  clog << unsorted_dec_df.row(1) << "\n";

  clog << "-- log col with operator<<:\n";
  clog << unsorted_dec_df.column(1) << "\n";

  clog << "-- log color condition:\n";
  DataFrame<int> ddd = unsorted_dec_df;
  ddd.logger.context.with_cell_color_condition([df](const Cell<int>* c) {
    if ((c->idx.col_name == "col-2") && (c->value > 10)) {
      return DF_COLOR_G;
    } else {
      return DF_COLOR_W;
    }
  });
  ddd.log(10);

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
