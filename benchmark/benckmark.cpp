#include "bench_timer.hpp"
#include <array>
#include <iostream>
#include <memory>
#include <stdlib.h>

#include <dataframe>

using namespace df;

#define BENCH_COL_COUNT 5000
#define BENCH_ROW_COUNT 5000

#define COUNT__ITER_DF_BENCH   1000
#define COUNT__ITER_ROW_BENCH  1000
#define COUNT__ITER_COL_BENCH  1000
#define COUNT__ITER_SORT_BENCH 1000
#define COUNT__ITER_COPY_BENCH 30

#define DF_BENCH
#define ROW_BENCH
#define COL_BENCH
// #define SORT_BENCH // REMOVED
#define ROW_SORT_BENCH
// #define COPY_BENCH

template<typename TimeUnit, unsigned long N>
void print_bench_result(std::array<std::chrono::nanoseconds, N> data, const char* bench_name) {
    TimeUnit sum = {};
    for (std::size_t i = 0; i < N; i++) {
        sum += std::chrono::duration_cast<TimeUnit>(data[i]);
    }
    auto avg = sum / N;
    std::cout << "    " << bench_name << ", avg time per it: " << avg << "\n";
}

int main() {

    using dataT = double;

    Timer<std::chrono::nanoseconds> nsec_timer;

    std::vector<std ::string> bench_col_names{};
    for (std::size_t i = 0; i < BENCH_COL_COUNT; i++) {
        bench_col_names.push_back(std::string{"col-" + std::to_string(i)});
    }
    std::vector<std ::string> bench_row_names{};
    for (std::size_t i = 0; i < BENCH_ROW_COUNT; i++) {
        bench_row_names.push_back(std::string{"row-" + std::to_string(i)});
    }

    DataFrame<dataT> df{bench_col_names, bench_row_names};

    for (std::size_t i = 0; i < df.size(); ++i) {
        df[i] = static_cast<dataT>(i);
    }

    std::cout << df.shape() << " size: " << (sizeof(dataT) * BENCH_COL_COUNT * BENCH_ROW_COUNT) / (1024 * 1000)
              << " MB, data Type: " << typeid(dataT).name() << "\n";

#ifdef DF_BENCH
    std::cout << "  direct access, iterations: " << COUNT__ITER_DF_BENCH << "\n";

    std::array<std::chrono::nanoseconds, COUNT__ITER_DF_BENCH> DataFrameIterator_bench_data;

    for (std::size_t i = 0; i < COUNT__ITER_DF_BENCH; i++) {
        nsec_timer.tick();
        // for ( std::size_t i = 0; i < df.size(); i++) {
        //   auto v = df[i].value;
        // }
        for (const auto& cell : df) {
            auto c __attribute__((unused)) = cell.value;
        }
        nsec_timer.tock();
        DataFrameIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "iter(), read value - all cells");

    for (std::size_t i = 0; i < COUNT__ITER_DF_BENCH; i++) {
        nsec_timer.tick();
        // for ( std::size_t i = 0; i < df.size(); i++) {
        //   df[i].value = 234234.234478;
        // }
        for (auto& cell : df) {
            cell.value = 234234.234478;
        }
        nsec_timer.tock();
        DataFrameIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::milliseconds>(DataFrameIterator_bench_data, "iter(), write value - all cells");

    for (std::size_t i = 0; i < COUNT__ITER_DF_BENCH; i++) {
        std::size_t rand_idx = static_cast<std::size_t>(rand()) % (df.size() - 1);
        nsec_timer.tick();
        auto v __attribute__((unused)) = df[rand_idx].value;
        nsec_timer.tock();
        DataFrameIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "random access read single integral indexing- single cell");

    for (std::size_t i = 0; i < COUNT__ITER_DF_BENCH; i++) {
        std::string col_name{"col-" + std::to_string(static_cast<std::size_t>(rand()) % (df.col_size() - 1))};
        std::string row_name{"row-" + std::to_string(static_cast<std::size_t>(rand()) % (df.row_size() - 1))};
        nsec_timer.tick();
        auto v __attribute__((unused)) = df[col_name, row_name].value;
        nsec_timer.tock();
        DataFrameIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "random access, read value col row string indexing - single cell");

    for (std::size_t i = 0; i < COUNT__ITER_DF_BENCH; i++) {
        std::size_t rand_idx = static_cast<std::size_t>(rand()) % (df.size() - 1);
        nsec_timer.tick();
        df[rand_idx].value = 234234.234478;
        nsec_timer.tock();
        DataFrameIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "random access wite - single cell");

    for (std::size_t i = 0; i < COUNT__ITER_DF_BENCH; i++) {
        std::string col_name{"col-" + std::to_string(static_cast<std::size_t>(rand()) % (df.col_size() - 1))};
        std::string row_name{"row-" + std::to_string(static_cast<std::size_t>(rand()) % (df.row_size() - 1))};
        nsec_timer.tick();
        df[col_name, row_name].value = 159159.159;
        nsec_timer.tock();
        DataFrameIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::nanoseconds>(DataFrameIterator_bench_data, "random access, write value col row string indexing - single cell");
#endif

#ifdef ROW_BENCH
    std::cout << "\n  row access, iterations: " << COUNT__ITER_ROW_BENCH << "\n";
    std::array<std::chrono::nanoseconds, COUNT__ITER_ROW_BENCH> RowIterator_bench_data;

    for (std::size_t i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
        nsec_timer.tick();
        for (auto i = df.iter_rows(); i < df.end(); i++) {
            for (const auto& c : i.current_row()) {
                auto v __attribute__((unused)) = c->value;
            }
        }
        nsec_timer.tock();
        RowIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::nanoseconds>(RowIterator_bench_data, "iter_row(), read all cells");

    for (std::size_t i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
        std::size_t row_count = df.shape().row_count;
        nsec_timer.tick();
        for (auto i = df.iter_rows(); i.current_row_idx() < row_count; i++) {
            for (auto& c : i.current_row()) {
                c->value = 136136.136;
            }
        }
        nsec_timer.tock();
        RowIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::milliseconds>(RowIterator_bench_data, "iter_row(), write to all cells");

    for (std::size_t i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
        std::size_t rand_idx = static_cast<std::size_t>(rand()) % (df.shape().row_count - 1);
        nsec_timer.tick();
        auto row = df.row(rand_idx);
        for (const auto& c : row) {
            auto v __attribute__((unused)) = c->value;
        }
        nsec_timer.tock();
        RowIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::nanoseconds>(RowIterator_bench_data, "row rand access read single row cells");

    for (std::size_t i = 0; i < COUNT__ITER_ROW_BENCH; i++) {
        std::size_t rand_idx = static_cast<std::size_t>(rand()) % (df.shape().row_count - 1);
        nsec_timer.tick();
        auto row = df.row(rand_idx);
        for (auto& c : row) {
            c->value = 789789.789;
        }
        nsec_timer.tock();
        RowIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::microseconds>(RowIterator_bench_data, "row rand access write to single row cells");
#endif

#ifdef COL_BENCH
    std::cout << "\n  col access, iterations: " << COUNT__ITER_COL_BENCH << "\n";
    std::array<std::chrono::nanoseconds, COUNT__ITER_COL_BENCH> ColumnIterator_bench_data;

    for (std::size_t i = 0; i < COUNT__ITER_COL_BENCH; i++) {
        nsec_timer.tick();
        for (auto i = df.iter_cols(); i < df.end(); i++) {
            for (auto& c : i.current_col()) {
                auto v __attribute__((unused)) = c->value;
            }
        }
        nsec_timer.tock();
        ColumnIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::nanoseconds>(ColumnIterator_bench_data, "iter_col(), read all cells");

    for (std::size_t i = 0; i < COUNT__ITER_COL_BENCH; i++) {
        std::size_t col_count = df.shape().col_count;
        nsec_timer.tick();
        for (auto col = df.iter_cols(); col.current_col_idx() < col_count; col++) {
            for (auto& c : col.current_col()) {
                c->value = 456456.456;
            }
        }
        nsec_timer.tock();
        ColumnIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::milliseconds>(ColumnIterator_bench_data, "iter_col(), write to all cells");

    for (std::size_t i = 0; i < COUNT__ITER_COL_BENCH; i++) {
        std::size_t rand_idx = static_cast<std::size_t>(rand()) % (df.shape().col_count - 1);
        nsec_timer.tick();
        auto col = df.column(rand_idx);
        for (auto& c : col) {
            auto v __attribute__((unused)) = c->value;
        }
        nsec_timer.tock();
        ColumnIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::nanoseconds>(ColumnIterator_bench_data, "col rand access, read single col cell values");

    for (std::size_t i = 0; i < COUNT__ITER_COL_BENCH; i++) {
        std::size_t rand_idx = static_cast<std::size_t>(rand()) % (df.shape().col_count - 1);
        nsec_timer.tick();
        auto col = df.column(rand_idx);
        for (auto& c : col) {
            c->value = 123123.123;
        }
        nsec_timer.tock();
        ColumnIterator_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::microseconds>(ColumnIterator_bench_data, "col rand access, write to single col cells");
#endif

    // #ifdef SORT_BENCH
    //     std::cout << "\n  sort, test iterations: " << COUNT__ITER_SORT_BENCH << "\n";
    //     std::array<std::chrono::nanoseconds, COUNT__ITER_SORT_BENCH> sort_bench_data;

    // for (std::size_t i = 0; i < COUNT__ITER_SORT_BENCH; i++) {
    //     for (std::size_t i = 0; i < df.size(); ++i) {
    //         df[i] = static_cast<dataT>(static_cast<std::size_t>(rand()) % df.size());
    //     }
    //     std::string col_name = bench_col_names[static_cast<std::size_t>(rand()) % (bench_col_names.size() - 1)];
    //     nsec_timer.tick();
    //     df.inplace_ascending_sort(col_name);
    //     nsec_timer.tock();
    //     sort_bench_data[i] = nsec_timer.duration();
    // }
    // print_bench_result<std::chrono::milliseconds>(sort_bench_data, "sort_rows(col_name), sort df by col");
    // #endif

#ifdef ROW_SORT_BENCH
    std::cout << "\n  sort, iterations: " << COUNT__ITER_SORT_BENCH << "\n";
    std::array<std::chrono::nanoseconds, COUNT__ITER_SORT_BENCH> row_sort_bench_data;

    for (std::size_t i = 0; i < df.size(); ++i) {
        df[i] = static_cast<dataT>(static_cast<std::size_t>(rand()) % df.size());
    }
    for (std::size_t i = 0; i < COUNT__ITER_SORT_BENCH; i++) {
        // std::string col_name = bench_col_names[(std::size_t)rand() % (bench_col_names.size() - 1)];
        auto col_name = bench_col_names[(std::size_t)rand() % (bench_col_names.size() - 1)];
        auto rows     = df.rows();
        nsec_timer.tick();
        rows.sort(col_name, true);
        nsec_timer.tock();
        row_sort_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::microseconds>(row_sort_bench_data, "sort(col_name, true), sort df by col, sort df rows by col value");
#endif

#ifdef COPY_BENCH
    std::cout << "\n  df copy, test iterations: " << COUNT__ITER_COPY_BENCH << "\n";
    std::array<std::chrono::nanoseconds, COUNT__ITER_COPY_BENCH> row_copy_bench_data;

    for (std::size_t i = 0; i < COUNT__ITER_COPY_BENCH; i++) {
        nsec_timer.tick();
        DataFrame<dataT> new_df{df};
        nsec_timer.tock();
        row_copy_bench_data[i] = nsec_timer.duration();
    }
    print_bench_result<std::chrono::milliseconds>(row_copy_bench_data, "DataFrame<T>::DataFrame(const DataFrame<T>& other): copy constructor");
#endif
    return 0;
}
