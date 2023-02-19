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
  clog << df;

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
  clog << df;

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

  clog << df;

  clog << "------------------COPY_DF------------------\n";
  for (sizetype i = 0; i < df.size(); i++) {
    df[i] = rand() % static_cast<int>(df.size());
  }

  clog << "--old df:\n";
  clog << df;
  DataFrame<int> new_df = df.copy();
  clog << "\n--new df:\n";
  clog << new_df;

  clog << "------------------SORTING------------------\n";

  DataFrame<int> unsorted_df = df.copy();

  clog << "-- unsorted df:\n";
  clog << unsorted_df;

  clog << "\n-- aec sort:\n";
  List<Row<int>> aec_sorted_rows = asc_sort_rows(unsorted_df, "col-2");
  clog << "asc rows log algo: \n";
  log_sorted_rows(aec_sorted_rows, unsorted_df);
  clog << "asc rows log algo head 5: \n";
  log_sorted_rows(aec_sorted_rows, unsorted_df, 5);
  clog << "asc rows log algo tail 3: \n";
  log_sorted_rows(aec_sorted_rows, unsorted_df, -3);

  clog << "\n-- dec sort:\n";
  List<Row<int>> dec_sorted_rows = dec_sort_rows(unsorted_df, "col-2");
  clog << "dec rows log algo: \n";
  log_sorted_rows(dec_sorted_rows, unsorted_df);

  clog << "\n-- inplace aec sort:\n";
  DataFrame<int> unsorted_aec_df = unsorted_df.copy();
  DataFrame<int> sorted_aec_df   = unsorted_aec_df.asc_sort_rows("col-2");
  clog << sorted_aec_df;

  clog << "\n-- inplace dec sort:\n";
  DataFrame<int> unsorted_dec_df = unsorted_df.copy();
  DataFrame<int> sorted_dec_df   = unsorted_dec_df.dec_sort_rows("col-2");
  clog << sorted_dec_df;
  // clog << unsorted_df;

  clog << "------------------LOGGING------------------\n";

  clog << "-- head:\n";
  unsorted_dec_df.print(3);

  fill_df(unsorted_dec_df, 0);

  clog << "-- tail:\n";
  unsorted_dec_df.print(-4);

  clog << "-- log row with operator<<:\n";
  clog << unsorted_dec_df.row(1) << "\n";

  clog << "-- log col with operator<<:\n";
  clog << unsorted_dec_df.column(1) << "\n";

  sorted_aec_df = sorted_dec_df;
  sorted_aec_df.logger
    .with_cell_logging_color_condition([df](Cell<int>* c) {
      if ((c->idx.col_name == "col-2") && (c->value > 10)) {
        return DF_COLOR_G;
      } else {
        return DF_COLOR_W;
      }
    })
    .log(10);

  // dec_inplace_sort_df.print(3, 3);
  // clog << dec_inplace_sort_df;

  return 0;
}
