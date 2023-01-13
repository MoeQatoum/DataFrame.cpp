#include "bench.h"
#include <dataframe.h>
#include <iostream>
#include <memory>
#include <stdlib.h>

#define R "\033[91m"
#define G "\033[92m"
#define C "\033[96m"
#define Y "\033[93m"
#define W "\033[00m"

// #define TEST
#define BENCH

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
#ifdef TEST
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
  {
    DataFrame<int> df{col_names, row_names};

    for (std::size_t i = 0; i < df.size(); ++i) {
      df[i] = static_cast<int>(i);
    }
    std::cout << df.shape() << "\n";
    df.print();

    for (int i = 0; i < df.shape().col_count; ++i) {
      // std::cout << df.get_col(i) << "\n";
      // df.get_col(1)[2].value = 111;
      // auto col = df.get_col(i);
      // for (int i = 0; i < col.size(); i++)
      // {
      // std::cout << "----------- " << col[i] << "\n";
      // }
    }
    std::cout << "iter_rows---------------------\n";

    std::size_t iiii = 0;
    // for (auto i = df.iter_rows(); i < df.shape().row_count; i++) {

    // // df[i.row()[0].idx.col_idx, i.row()[0].idx.row_idx] = 123123;
    // // i.row()[0].value                                   = 123123;
    // ++iiii;
    // }

    std::cout << "iter_cols---------------------\n";
    iiii = 0;
    // for (auto i = df.iter_cols(); i < df.end(); ++i) {
    for (auto i = df.iter_cols(); i < df.shape().col_count; i++) {
      // df[i.column()[0].idx.col_idx, i.column()[0].idx.row_idx] = 123123;
      // i.column()[0]->value;
      // i.column()[0]->value = 111;
      for (auto c : i.column()) {
        c->value = 111;
      }
      // std::cout << &i.column()[0] << "\n";
      // std::cout << &df[0] << "\n";
      // std::cout << "                         " << iiii * / << "----------\n";
      // std::cout << i.column() << "\n";
      // std::cout << i.column()[0] << "\n";
      ++iiii;
    }

    std::cout << "CELL---------------------\n";
    // df[1, 2].value = 99;
    // df.get_col(1)[2].value = 69;
    // df[2].value = 69;
    df.print();
  }
#endif

#ifdef BENCH
  #define BENCH_COL_COUNT 5000
  #define BENCH_ROW_COUNT 5000

  #define COUNT__ITER_DF_BENCH  100
  #define COUNT__ITER_ROW_BENCH 100
  #define COUNT__ITER_COL_BENCH 100

  #define ITER_DF_BENCH
  #define ITER_ROW_BENCH
  #define ITER_COL_BENCH

  using dataT = double;

  Timer<std::chrono::milliseconds> msec_timer;
  Timer<std::chrono::microseconds> usec_timer;
  Timer<std::chrono::nanoseconds>  nsec_timer;

  std::vector<std::string> col_names{};
  for (std::size_t i = 0; i < BENCH_COL_COUNT; i++) {
    col_names.push_back(std::string{"col-" + std::to_string(i + 1)});
  }
  std::vector<std::string> row_names{};
  for (std::size_t i = 0; i < BENCH_ROW_COUNT; i++) {
    row_names.push_back(std::string{"row-" + std::to_string(i + 1)});
  }

  DataFrame<dataT> df{col_names, row_names};

  for (std::size_t i = 0; i < df.size(); ++i) {
    df[i] = static_cast<dataT>(i);
  }

  std::cout << df.shape() << " size: " << sizeof(dataT) * BENCH_COL_COUNT * BENCH_ROW_COUNT / 1000000
            << " MB, data Type: " << typeid(dataT).name() << "\n";

  #ifdef ITER_DF_BENCH
  std::cout << "  direct access, iterations: " << COUNT__ITER_DF_BENCH << "\n";

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
    for (auto cell : df) {
      cell.value = 234234.234478;
    }
    msec_timer.tock();
    DataFrameIterator_bench_data[i] = msec_timer.duration().count();
  }
  print_bench_result<std::chrono::milliseconds>(DataFrameIterator_bench_data, "iter(), write value - all cells");

  for (int i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    int rand_idx = rand() % (df.size() - 1);
    nsec_timer.tick();
    auto v = df[rand_idx].value;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "random access read - single cell");

  for (int i = 0; i < COUNT__ITER_DF_BENCH; i++) {
    int rand_idx = rand() % (df.size() - 1);
    nsec_timer.tick();
    df[rand_idx].value = 234234.234478;
    nsec_timer.tock();
    DataFrameIterator_bench_data[i] = nsec_timer.duration().count();
  }
  print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "random access wite - single cell");
  #endif

  #ifdef ITER_ROW_BENCH
  std::cout << "\n  row access, iterations: " << COUNT__ITER_ROW_BENCH << "\n";
  std::array<int, COUNT__ITER_ROW_BENCH> RowIterator_bench_data;

  for (int i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    usec_timer.tick();
    for (auto i = df.iter_rows(); i < df.end(); ++i) {
      for (const auto& c : i.row()) {
        c->value;
      }
    }
    usec_timer.tock();
    RowIterator_bench_data[i] = usec_timer.duration().count();
  }
  print_bench_result<std::chrono::microseconds>(RowIterator_bench_data, "iter_row(), read all cells");

  for (int i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
    msec_timer.tick();
    for (auto i = df.iter_rows(); i < df.end(); ++i) {
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
      auto v = c->value = 789789.789;
    }
    usec_timer.tock();
    RowIterator_bench_data[i] = usec_timer.duration().count();
  }
  print_bench_result<std::chrono::microseconds>(RowIterator_bench_data, "row rand access write to single row cells");
  #endif

  #ifdef ITER_COL_BENCH
  std::cout << "\n  col access, iterations: " << COUNT__ITER_COL_BENCH << "\n";
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
    usec_timer.tick();
    for (auto i = df.iter_cols(); i < df.end(); i++) {
      for (auto& c : i.column()) {
        c->value = 456456.456;
      }
    }
    usec_timer.tock();
    ColumnIterator_bench_data[i] = usec_timer.duration().count();
  }
  print_bench_result<std::chrono::microseconds>(ColumnIterator_bench_data, "iter_col(), write to all cells");

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
  print_bench_result<std::chrono::nanoseconds>(ColumnIterator_bench_data,
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

#endif
  return 0;
}
