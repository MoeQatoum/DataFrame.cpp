#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <dataframe.h>
#undef clog

template<typename T, df::sizetype COL_COUNT, df::sizetype ROW_COUNT>
df::DataFrame<T> create_dataframe() {
  df::StringList col_names{};
  for (df::sizetype i = 1; i <= COL_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    col_names.push_back(df::String("col-%1").arg(i));
#else
    col_names.push_back(df::String{"col-" + std::to_string(i)});
#endif
  }

  df::StringList row_names{};
  for (df::sizetype i = 1; i <= ROW_COUNT; i++) {
#ifdef QT_IMPLEMENTATION
    row_names.push_back(df::String("row-%1").arg(i));
#else
    row_names.push_back(df::String{"row-" + std::to_string(i)});
#endif
  }

  df::DataFrame<T> df{col_names, row_names};

  return df;
}

template<typename T>
df::Series<T> make_series(std::initializer_list<T> data) {
  return df::Series<T>{data};
}

#endif // TEST_UTILS_H
