#include <dataframe>
#include <format>

using namespace df;

int main() {

#define TEST_COL_COUNT 3
#define TEST_ROW_COUNT 10
    std::vector<std::string> col_names{};
    for (std::size_t i = 1; i <= TEST_COL_COUNT; i++) {
        col_names.push_back(std::format("col-{}", i));
    }

    std::vector<std::string> row_names{};
    for (std::size_t i = 1; i <= TEST_ROW_COUNT; i++) {
        row_names.push_back(std::format("row-{}", i));
    }

    DataFrame<int> df{col_names, row_names};
    for (std::size_t i = 0; i < df.size(); ++i) {
        df[i] = static_cast<int>(i);
    }
    df.log();

    const DataFrame<int> const_df = df.copy();
    // const_df[1,1] = 123;
    for (auto col_iter = const_df.iter_cols(); col_iter < const_df.end(); ++col_iter) {
        if (col_iter.current_col().index() == 1) {
            // col_iter.current_col()[0]->value = 123;
            for (auto& c : col_iter.current_col()) {
                // c->value = 456;
            }

            auto d = col_iter.current_col().to_series();
            for (std::size_t i = 0; i < d.size(); i++) {
                std::cout << d[i] << ", ";
            }
            std::cout << "\n";
        }
    }

    for (auto col_iter = df.iter_cols(); col_iter < df.end(); ++col_iter) {
        if (col_iter.current_col().index() == 1) {
            for (const auto& c : col_iter.current_col()) {
                c->value = 456;
            }

            auto d = col_iter.current_col().to_series();
            for (std::size_t i = 0; i < d.size(); i++) {
                std::cout << d[i] << ", ";
            }
            std::cout << "\n";
        }
    }
    df.log();

    for (auto row_iter = df.iter_rows(); row_iter < df.end(); row_iter++) {
        if (row_iter.current_row().index() == 1) {
            for (const auto& c : row_iter.current_row()) {
                c->value = 123;
            }
            // i.row()[0]->value = 123;
            auto d = row_iter.current_row().to_series();
            for (std::size_t i = 0; i < d.size(); i++) {
                std::cout << d[i] << ", ";
            }
            std::cout << "\n";
        }
    }

    df.log();

    for (std::size_t i = 0; i < df.size(); ++i) {
        df[i] = static_cast<int>(i);
    }

    df.log();

    std::cout << "------------------COPY_DF------------------\n";
    for (std::size_t i = 0; i < df.size(); i++) {
        df[i] = rand() % static_cast<int>(df.size());
    }

    std::cout << "--old df:\n";
    df.log();
    DataFrame<int> new_df = df.copy();
    std::cout << "\n--new df:\n";
    new_df.log();

    std::cout << "\n";
    Series<int> comp = df.column(1) + df.column(2);
    for (std::size_t i = 0; i < comp.size(); i++) {
        std::cout << comp[i] << " ";
    }
    std::cout << "\n";

    std::cout << "------------------SORTING------------------\n";

    DataFrame<int> unsorted_df = df.copy();

    std::cout << "-- unsorted df:\n";
    unsorted_df.log();

    std::cout << "-- max min df:\n";
    std::cout << unsorted_df["col-2"].max() << " " << unsorted_df["col-2"].min() << "\n";

    std::cout << "\n-- asc sort:\n";
    std::cout << "---------------\n";
    auto asc_sorted_rows = unsorted_df.sort("col-2", true);
    std::cout << "---------------\n";
    asc_sorted_rows.log();

    std::cout << "asc rows log algo head 5: \n";
    asc_sorted_rows.log(5);
    std::cout << "asc rows log algo tail 3: \n";
    asc_sorted_rows.log(-3);

    std::cout << "\n-- dec sort:\n";
    auto dec_sorted_rows = unsorted_df.rows().sort("col-2");
    dec_sorted_rows.log();

    // std::cout << "\n-- inplace aec sort:\n";
    // DataFrame<int> unsorted_asc_df = unsorted_df.copy();
    // DataFrame<int> sorted_asc_df(unsorted_asc_df.rows().sort(unsorted_asc_df["col-2"], true));
    // sorted_asc_df.log();

    // std::cout << "\n-- inplace dec sort:\n";
    // DataFrame<int> unsorted_dec_df = unsorted_df.copy();
    // DataFrame<int> sorted_dec_df(unsorted_asc_df.rows().sort(unsorted_asc_df["col-2"]));
    // sorted_dec_df.log();

    std::cout << "------------------LOGGING------------------\n";
    DataFrame<int> unsorted_dec_df = unsorted_df.copy();

    std::cout << "-- head:\n";
    unsorted_dec_df.log(3);

    std::cout << "-- tail:\n";
    unsorted_dec_df.log(-4);

    std::cout << "-- log row with operator<<:\n";
    std::cout << unsorted_dec_df.row(1) << "\n";

    std::cout << "-- log col with operator<<:\n";
    std::cout << unsorted_dec_df.column(1) << "\n";

    std::cout << "-- log color condition:\n";
    DataFrame<int> ddd = unsorted_dec_df;
    ddd.logger.context.with_cell_color_condition([df](const Cell<int>* c) {
        if ((c->idx.col_name == "col-2") && (c->value > 10)) {
            return DF_COLOR_G;
        } else {
            return DF_COLOR_W;
        }
    });
    ddd.log(10);

    std::cout << "------------------UTILS------------------\n";
    std::cout << "-- before fill_df():\n";
    unsorted_dec_df.log();
    std::cout << "-- after fill_df():\n";
    fill_df(unsorted_dec_df, 0);
    unsorted_dec_df.log();

    std::cout << "------------------SERIES------------------\n";
    Series<int> series_1(20);
    fill_series(series_1, 1);
    Series<int> series_2(20);
    fill_series(series_2, 2);

    Series<bool> bar = (series_2 == 2);
    series_2 += 10;

    for (std::size_t i = 0; i < bar.size(); i++) {
        std::cout << series_2[i] << " ";
    }
    std::cout << "\n";
    return 0;
}
