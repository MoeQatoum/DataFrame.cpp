#include "bench_timer.h"
#include <array>
#include <iostream>
#include <memory>
#include <stdlib.h>

#include <dataframe.h>

using namespace df;

#define BENCH_COL_COUNT 5000
#define BENCH_ROW_COUNT 5000

#define COUNT__ITER_DF_BENCH   1000
#define COUNT__ITER_ROW_BENCH  1000
#define COUNT__ITER_COL_BENCH  1000
#define COUNT__ITER_SORT_BENCH 1000

#define DF_BENCH
#define ROW_BENCH
#define COL_BENCH
// #define SORT_BENCH
#define ROW_SORT_BENCH

template<typename TimeUnit, unsigned long N>
void print_bench_result(std::array<long, N> data, const char* bench_name) {
  int sum = 0;
  for (sizetype i = 0; i < N; i++) {
    sum += data[i];
  }
  auto avg = static_cast<sizetype>(sum) / N;
#ifdef QT_IMPLEMENTATION
  if (std::is_same<TimeUnit, std::chrono::milliseconds>::value) {
    clog << "    " << bench_name << " , avg time per it: " << String("%1%2").arg(TimeUnit(avg).count()).arg("ms");
  } else if (std::is_same<TimeUnit, std::chrono::microseconds>::value) {
    clog << "    " << bench_name << " , avg time per it: " << String("%1%2").arg(TimeUnit(avg).count()).arg("µs");
  } else if (std::is_same<TimeUnit, std::chrono::nanoseconds>::value) {
    clog << "    " << bench_name << " , avg time per it: " << String("%1%2").arg(TimeUnit(avg).count()).arg("ns");
  } else {
    clog << "Unknown TimeUnit Type:" << typeid(TimeUnit).name();
    qTerminate();
  }
#else
  clog << "    " << bench_name << " , avg time per it: " << TimeUnit(avg) << "\n";
#endif
}

int main() {

  using dataT = double;

  Timer<std::chrono::milliseconds> msec_timer;
  Timer<std::chrono::microseconds> usec_timer;
  Timer<std::chrono::nanoseconds>  nsec_timer;

  StringList bench_col_names{};
  for (sizetype i = 0; i < BENCH_COL_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    bench_col_names.push_back(String{"row-%1"}.arg(i));
#else
    bench_col_names.push_back(String{"col-" + std::to_string(i + 1)});
#endif
  }
  StringList bench_row_names{};
  for (sizetype i = 0; i < BENCH_ROW_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    bench_row_names.push_back(String{"row-%1"}.arg(i));
#else
    bench_row_names.push_back(String{"row-" + std::to_string(i + 1)});
#endif
  }

  DataFrame<dataT> df{bench_col_names, bench_row_names};

  for (sizetype i = 0; i < df.size(); ++i) {
    df[i] = static_cast<dataT>(i);
  }

  clog << df.shape() << " size: " << sizeof(dataT) * BENCH_COL_COUNT * BENCH_ROW_COUNT / 1000000
       << " MB, data Type: " << typeid(dataT).name() << "\n";

#ifdef DF_BENCH
  #ifdef QT_IMPLEMENTATION
  clog << "  direct access, test iterations: " << COUNT__ITER_DF_BENCH;
  #else
  clog << "  direct access, test iterations: " << COUNT__ITER_DF_BENCH << "\n";
  #endif

  std::array<long, COUNT__ITER_DF_BENCH> DataFrameIterator_bench_data;

  for (sizetype i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    nsec_timer.tick();
    // for ( sizetype i = 0; i < df.size(); i++) {
    //   auto v = df[i].value;
    // }
    for (const auto& cell : df) {
      auto c = cell.value;
    }
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "iter(), read value - all cells");

  for (sizetype i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    msec_timer.tick();
    // for ( sizetype i = 0; i < df.size(); i++) {
    //   df[i].value = 234234.234478;
    // }
    for (auto& cell : df) {
      cell.value = 234234.234478;
    }
    msec_timer.tock();
    DataFrameIterator_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(DataFrameIterator_bench_data, "iter(), write value - all cells");

  for (sizetype i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    sizetype rand_idx = static_cast<sizetype>(rand()) % (df.size() - 1);
    nsec_timer.tick();
    df[rand_idx].value;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data,
                                               "random access read single integral indexing- single cell");

  for (sizetype i = 0; i < COUNT__ITER_DF_BENCH; i++) {
  #ifdef QT_IMPLEMENTATION
    String col_name = String("col-%1").arg(static_cast<sizetype>(rand()) % (df.col_size() - 1));
    String row_name = String("row-%1").arg(static_cast<sizetype>(rand()) % (df.row_size() - 1));
  #else
    String col_name{"col-" + std::to_string(static_cast<sizetype>(rand()) % (df.col_size() - 1))};
    String row_name{"row-" + std::to_string(static_cast<sizetype>(rand()) % (df.row_size() - 1))};
  #endif
    nsec_timer.tick();
    auto v = df[col_name, row_name].value;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data,
                                               "random access, read value col row string indexing - single cell");

  for (sizetype i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    sizetype rand_idx = static_cast<sizetype>(rand()) % (df.size() - 1);
    nsec_timer.tick();
    df[rand_idx].value = 234234.234478;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "random access wite - single cell");

  for (sizetype i = 0; i < COUNT__ITER_DF_BENCH; i++) {
  #ifdef QT_IMPLEMENTATION
    String col_name = String("col-%1").arg(static_cast<sizetype>(rand()) % (df.col_size() - 1));
    String row_name = String("row-%1").arg(static_cast<sizetype>(rand()) % (df.row_size() - 1));
  #else
    String col_name{"col-" + std::to_string(static_cast<sizetype>(rand()) % (df.col_size() - 1))};
    String row_name{"row-" + std::to_string(static_cast<sizetype>(rand()) % (df.row_size() - 1))};
  #endif
    nsec_timer.tick();
    df[col_name, row_name].value = 159159.159;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data,
                                               "random access, write value col row string indexing - single cell");
#endif

#ifdef ROW_BENCH
  #ifdef QT_IMPLEMENTATION
  clog << "\n  row access, test iterations: " << COUNT__ITER_ROW_BENCH;
  #else
  clog << "\n  row access, test iterations: " << COUNT__ITER_ROW_BENCH << "\n";
  #endif

  std::array<long, COUNT__ITER_ROW_BENCH> RowIterator_bench_data;

  for (sizetype i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    nsec_timer.tick();
    for (auto i = df.iter_rows(); i < df.end(); i++) {
      for (const auto& c : i.current_row()) {
        c->value;
      }
    }
    nsec_timer.tock();
    RowIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(RowIterator_bench_data, "iter_row(), read all cells");

  for (sizetype i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    sizetype row_c = df.shape().row_count;
    msec_timer.tick();
    for (auto i = df.iter_rows(); i < row_c; i++) {
      for (auto& c : i.current_row()) {
        c->value = 136136.136;
      }
    }
    msec_timer.tock();
    RowIterator_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(RowIterator_bench_data, "iter_row(), write to all cells");

  for (sizetype i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    sizetype rand_idx = static_cast<sizetype>(rand()) % (df.shape().row_count - 1);
    nsec_timer.tick();
    auto row = df.get_row(rand_idx);
    for (const auto& c : row) {
      auto v = c->value;
    }
    nsec_timer.tock();
    RowIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(RowIterator_bench_data, "row rand access read single row cells");

  for (sizetype i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    sizetype rand_idx = static_cast<sizetype>(rand()) % (df.shape().row_count - 1);
    usec_timer.tick();
    auto row = df.get_row(rand_idx);
    for (auto& c : row) {
      c->value = 789789.789;
    }
    usec_timer.tock();
    RowIterator_bench_data[i] = usec_timer.duration().count();
  }
  print_bench_result<std::chrono::microseconds>(RowIterator_bench_data, "row rand access write to single row cells");
#endif

#ifdef COL_BENCH
  #ifdef QT_IMPLEMENTATION
  clog << "\n  col access, test iterations: " << COUNT__ITER_COL_BENCH;
  #else
  clog << "\n  col access, test iterations: " << COUNT__ITER_COL_BENCH << "\n";
  #endif
  std::array<long, COUNT__ITER_COL_BENCH> ColumnIterator_bench_data;

  for (sizetype i = 0; i < COUNT__ITER_COL_BENCH; i++) {
    nsec_timer.tick();
    for (auto i = df.iter_cols(); i < df.end(); i++) {
      for (auto& c : i.current_col()) {
        auto v = c->value;
      }
    }
    nsec_timer.tock();
    ColumnIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(ColumnIterator_bench_data, "iter_col(), read all cells");

  for (sizetype i = 0; i < COUNT__ITER_COL_BENCH; i++) {
    sizetype col_c = df.shape().col_count;
    msec_timer.tick();
    for (auto i = df.iter_cols(); i < col_c; i++) {
      for (auto& c : i.current_col()) {
        c->value = 456456.456;
      }
    }
    msec_timer.tock();
    ColumnIterator_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(ColumnIterator_bench_data, "iter_col(), write to all cells");

  for (sizetype i = 0; i < COUNT__ITER_COL_BENCH; i++) {
    sizetype rand_idx = static_cast<sizetype>(rand()) % (df.shape().col_count - 1);
    nsec_timer.tick();
    auto col = df.get_col(rand_idx);
    for (auto& c : col) {
      auto v = c->value;
    }
    nsec_timer.tock();
    ColumnIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(ColumnIterator_bench_data,
                                                "col rand access, read single col cell values");

  for (sizetype i = 0; i < COUNT__ITER_COL_BENCH; i++) {
    sizetype rand_idx = static_cast<sizetype>(rand()) % (df.shape().col_count - 1);
    usec_timer.tick();
    auto col = df.get_col(rand_idx);
    for (auto& c : col) {
      c->value = 123123.123;
    }
    usec_timer.tock();
    ColumnIterator_bench_data[i] = usec_timer.duration().count();
  }
  print_bench_result<std::chrono::microseconds>(ColumnIterator_bench_data,
                                                "col rand access, write to single col cells");
#endif

#ifdef SORT_BENCH
  #ifdef QT_IMPLEMENTATION
  clog << "\n  sort, test iterations: " << COUNT__ITER_SORT_BENCH;
  #else
  clog << "\n  sort, test iterations: " << COUNT__ITER_SORT_BENCH << "\n";
  #endif
  std::array<long, COUNT__ITER_SORT_BENCH> sort_bench_data;

  for (sizetype i = 0; i < COUNT__ITER_SORT_BENCH; i++) {
    for (sizetype i = 0; i < df.size(); ++i) {
      df[i] = static_cast<dataT>(static_cast<sizetype>(rand()) % df.size());
    }
    String col_name = bench_col_names[static_cast<sizetype>(rand()) % (bench_col_names.size() - 1)];
    msec_timer.tick();
    df.asc_sort_rows(col_name);
    msec_timer.tock();
    sort_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(sort_bench_data, "sort_rows(col_name), sort df by col");
#endif

#ifdef ROW_SORT_BENCH
  #ifdef QT_IMPLEMENTATION
  clog << "\n  sort, test iterations: " << COUNT__ITER_SORT_BENCH;
  #else
  clog << "\n  sort, test iterations: " << COUNT__ITER_SORT_BENCH << "\n";
  #endif
  std::array<long, COUNT__ITER_SORT_BENCH> row_sort_bench_data;

  for (sizetype i = 0; i < COUNT__ITER_SORT_BENCH; i++) {
    for (sizetype i = 0; i < df.size(); ++i) {
      df[i] = static_cast<dataT>(static_cast<sizetype>(rand()) % df.size());
    }
    String col_name = bench_col_names[(sizetype)rand() % (bench_col_names.size() - 1)];
    msec_timer.tick();
    asc_sort_rows(df, col_name);
    msec_timer.tock();
    row_sort_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(row_sort_bench_data,
                                                "utils::asc_sort_rows(df&, col_name), sort df rows by col");
#endif
  return 0;
}
