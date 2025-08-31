#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <dataframe>

template<typename T, std::size_t COL_COUNT, std::size_t ROW_COUNT>
df::DataFrame<T> create_dataframe() {
    std::vector<std::string> col_names{};
    for (std::size_t i = 1; i <= COL_COUNT; i++) {
        col_names.push_back(std::string{"col-" + std::to_string(i)});
    }

    std::vector<std ::string> row_names{};
    for (std::size_t i = 1; i <= ROW_COUNT; i++) {
        row_names.push_back(std::string{"row-" + std::to_string(i)});
    }

    return df::DataFrame<T>{col_names, row_names};
}

template<typename T>
df::Series<T> make_series(std::initializer_list<T> data) {
    return df::Series<T>{data};
}

#endif // TEST_UTILS_H
