#include "bench_timer.h"
#include <array>
#include <iostream>
#include <memory>
#include <stdlib.h>

#include "utils.h"
#include <dataframe.h>

#define R "\033[91m"
#define G "\033[92m"
#define C "\033[96m"
#define Y "\033[93m"
#define W "\033[00m"

#define BENCH_COL_COUNT 400
#define BENCH_ROW_COUNT 400

#define COUNT__ITER_DF_BENCH   1000
#define COUNT__ITER_ROW_BENCH  1000
#define COUNT__ITER_COL_BENCH  1000
#define COUNT__ITER_SORT_BENCH 1000

#define DF_BENCH
#define ROW_BENCH
#define COL_BENCH
// #define SORT_BENCH
#define ROW_SORT_BENCH

template<typename TimeUnit, std::size_t N>
void print_bench_result(std::array<int, N> data, const char* bench_name) {
  int sum = 0;
  for (int i = 0; i < N; i++) {
    sum += data[i];
  }
  auto avg = sum / N;
  std::cout << "    " << bench_name << " , avg time per it: " << TimeUnit(avg) << "\n";
}

int main() {

  using dataT = double;

  Timer<std::chrono::milliseconds> msec_timer;
  Timer<std::chrono::microseconds> usec_timer;
  Timer<std::chrono::nanoseconds>  nsec_timer;

  std::vector<std::string> bench_col_names{};
  for (std::size_t i = 0; i < BENCH_COL_COUNT; i++) {
    bench_col_names.push_back(std::string{"col-" + std::to_string(i + 1)});
  }
  std::vector<std::string> bench_row_names{};
  for (std::size_t i = 0; i < BENCH_ROW_COUNT; i++) {
    bench_row_names.push_back(std::string{"row-" + std::to_string(i + 1)});
  }

  df::DataFrame<dataT> df{bench_col_names, bench_row_names};

  for (std::size_t i = 0; i < df.size(); ++i) {
    df[i] = static_cast<dataT>(i);
  }

  std::cout << df.shape() << " size: " << sizeof(dataT) * BENCH_COL_COUNT * BENCH_ROW_COUNT / 1000000
            << " MB, data Type: " << typeid(dataT).name() << "\n";

#ifdef DF_BENCH
  std::cout << "  direct access, test iterations: " << COUNT__ITER_DF_BENCH << "\n";

  std::array<int, COUNT__ITER_DF_BENCH> DataFrameIterator_bench_data;

  for (int i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    nsec_timer.tick();
    // for (int i = 0; i < df.size(); i++) {
    //   auto v = df[i].value;
    // }
    for (const auto& cell : df) {
      auto c = cell.value;
    }
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "iter(), read value - all cells");

  for (int i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    msec_timer.tick();
    // for (int i = 0; i < df.size(); i++) {
    //   df[i].value = 234234.234478;
    // }
    for (auto& cell : df) {
      cell.value = 234234.234478;
    }
    msec_timer.tock();
    DataFrameIterator_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(DataFrameIterator_bench_data, "iter(), write value - all cells");

  for (int i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    int rand_idx = rand() % (df.size() - 1);
    nsec_timer.tick();
    df[rand_idx].value;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data,
                                               "random access read single integral indexing- single cell");

  for (int i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    std::string col_name{"col-" + std::to_string(rand() % (df.col_size() - 1))};
    std::string row_name{"row-" + std::to_string(rand() % (df.row_size() - 1))};
    nsec_timer.tick();
    auto v = df[col_name, row_name].value;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data,
                                               "random access, read value col row string indexing - single cell");

  for (int i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    int rand_idx = rand() % (df.size() - 1);
    nsec_timer.tick();
    df[rand_idx].value = 234234.234478;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "random access wite - single cell");

  for (int i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    std::string col_name{"col-" + std::to_string(rand() % (df.col_size() - 1))};
    std::string row_name{"row-" + std::to_string(rand() % (df.row_size() - 1))};
    nsec_timer.tick();
    df[col_name, row_name].value = 159159.159;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data,
                                               "random access, write value col row string indexing - single cell");
#endif

#ifdef ROW_BENCH
  std::cout << "\n  row access, test iterations: " << COUNT__ITER_ROW_BENCH << "\n";
  std::array<int, COUNT__ITER_ROW_BENCH> RowIterator_bench_data;

  for (int i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    nsec_timer.tick();
    for (auto i = df.iter_rows(); i < df.end(); i++) {
      for (const auto& c : i.row()) {
        c->value;
      }
    }
    nsec_timer.tock();
    RowIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(RowIterator_bench_data, "iter_row(), read all cells");

  for (int i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    int row_c = df.shape().row_count;
    msec_timer.tick();
    for (auto i = df.iter_rows(); i < row_c; i++) {
      for (auto& c : i.row()) {
        c->value = 136136.136;
      }
    }
    msec_timer.tock();
    RowIterator_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(RowIterator_bench_data, "iter_row(), write to all cells");

  for (int i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    int rand_idx = rand() % (df.shape().row_count - 1);
    nsec_timer.tick();
    auto row = df.get_row(rand_idx);
    for (const auto& c : row) {
      auto v = c->value;
    }
    nsec_timer.tock();
    RowIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(RowIterator_bench_data, "row rand access read single row cells");

  for (int i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    int rand_idx = rand() % (df.shape().row_count - 1);
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
  std::cout << "\n  col access, test iterations: " << COUNT__ITER_COL_BENCH << "\n";
  std::array<int, COUNT__ITER_COL_BENCH> ColumnIterator_bench_data;

  for (int i = 0; i < COUNT__ITER_COL_BENCH; i++) {
    nsec_timer.tick();
    for (auto i = df.iter_cols(); i < df.end(); i++) {
      for (auto& c : i.column()) {
        auto v = c->value;
      }
    }
    nsec_timer.tock();
    ColumnIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(ColumnIterator_bench_data, "iter_col(), read all cells");

  for (int i = 0; i < COUNT__ITER_COL_BENCH; i++) {
    int col_c = df.shape().col_count;
    msec_timer.tick();
    for (auto i = df.iter_cols(); i < col_c; i++) {
      for (auto& c : i.column()) {
        c->value = 456456.456;
      }
    }
    msec_timer.tock();
    ColumnIterator_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(ColumnIterator_bench_data, "iter_col(), write to all cells");

  for (int i = 0; i < COUNT__ITER_COL_BENCH; i++) {
    int rand_idx = rand() % (df.shape().col_count - 1);
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

  for (int i = 0; i < COUNT__ITER_COL_BENCH; i++) {
    int rand_idx = rand() % (df.shape().col_count - 1);
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
  std::cout << "\n  sort, test iterations: " << COUNT__ITER_SORT_BENCH << "\n";
  std::array<int, COUNT__ITER_SORT_BENCH> sort_bench_data;

  for (int i = 0; i < COUNT__ITER_SORT_BENCH; i++) {
    for (std::size_t i = 0; i < df.size(); ++i) {
      df[i] = static_cast<dataT>(rand() % df.size());
    }
    std::string col_name = bench_col_names[rand() % (bench_col_names.size() - 1)];
    msec_timer.tick();
    df.sort_rows(col_name);
    msec_timer.tock();
    sort_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(sort_bench_data, "sort_rows(col_name), sort df by col");
#endif

#ifdef ROW_SORT_BENCH
  std::cout << "\n  sort, test iterations: " << COUNT__ITER_SORT_BENCH << "\n";
  std::array<int, COUNT__ITER_SORT_BENCH> row_sort_bench_data;

  for (int i = 0; i < COUNT__ITER_SORT_BENCH; i++) {
    for (std::size_t i = 0; i < df.size(); ++i) {
      df[i] = static_cast<dataT>(rand() % df.size());
    }
    std::string col_name = bench_col_names[rand() % (bench_col_names.size() - 1)];
    msec_timer.tick();
    df::utils::asc_sort_rows(df, col_name);
    msec_timer.tock();
    row_sort_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(row_sort_bench_data,
                                                "utils::asc_sort_rows(df&, col_name), sort df rows by col");
#endif
  return 0;
}
