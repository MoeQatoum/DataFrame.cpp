#ifndef DATA_FRAME_UTILS_H
#define DATA_FRAME_UTILS_H

#include "df_common.h"

namespace df {
  template<typename T>
  class Cell;

  template<typename T>
  class DataFrame;

  template<typename T>
  class Row;

  template<typename T>
  class Column;

  template<typename T>
  void fill_df(DataFrame<T>& df, T fill_value) {
    for (auto& c : df) {
      c.value = fill_value;
    }
  }

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  List<Row<T>> ascending_sort(DataFrame<T>& df, const String& col_name) {
    using ValueType = typename Column<T>::ValueType;

    Column<T> col = df.column(col_name);

    ValueType* sorted_cells = new ValueType[col.size()];

    List<Row<T>> rows;
    for (auto row_iterator = df.iter_rows(); row_iterator < df.end(); row_iterator++) {
      rows.push_back(row_iterator.current_row());
    }

    sorted_cells[0] = col[0];

    for (sizetype idx = 0; idx < col.size(); idx++) {
      bool     lower_found = false;
      sizetype insert_idx  = 0;
      for (sizetype sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
        if (col[idx]->value < sorted_cells[sorted_idx]->value) {
          lower_found = true;
          insert_idx  = sorted_idx;
          break;
        }
      }
      if (lower_found) {
        for (sizetype i = idx - 1; i >= insert_idx; i--) {
          sorted_cells[i + 1] = sorted_cells[i];

          // avoidsizetype (aka df_ui32) underflow
          if (i == 0) {
            break;
          }
        }
        sorted_cells[insert_idx] = col[idx];
      } else {
        sorted_cells[idx] = col[idx];
      }
    }

    // sort
    List<Row<T>> sorted_rows;
    for (sizetype idx = 0; idx < col.size(); idx++) {
      sorted_rows.push_back(df.row(sorted_cells[idx]->idx.row_idx));
    }

    delete[] sorted_cells;
    return sorted_rows;
  }

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  List<Row<T>> descending_sort(DataFrame<T>& df, const String& col_name) {
    using ValueType = typename Column<T>::ValueType;

    Column<T> col = df.column(col_name);

    ValueType* sorted_cells = new ValueType[col.size()];

    List<Row<T>> rows;
    for (auto row_iterator = df.iter_rows(); row_iterator < df.end(); row_iterator++) {
      rows.push_back(row_iterator.current_row());
    }

    sorted_cells[0] = col[0];

    for (sizetype idx = 0; idx < col.size(); idx++) {
      bool     higher_found = false;
      sizetype insert_idx   = 0;
      for (sizetype sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
        if (col[idx]->value > sorted_cells[sorted_idx]->value) {
          higher_found = true;
          insert_idx   = sorted_idx;
          break;
        }
      }
      if (higher_found) {
        for (sizetype i = idx - 1; i >= insert_idx; i--) {
          sorted_cells[i + 1] = sorted_cells[i];

          // avoidsizetype (aka df_ui32) underflow
          if (i == 0) {
            break;
          }
        }
        sorted_cells[insert_idx] = col[idx];
      } else {
        sorted_cells[idx] = col[idx];
      }
    }

    // sort
    List<Row<T>> sorted_rows;
    for (sizetype idx = 0; idx < col.size(); idx++) {
      sorted_rows.push_back(df.row(sorted_cells[idx]->idx.row_idx));
    }

    delete[] sorted_cells;
    return sorted_rows;
  }

} // namespace df

#endif // DATA_FRAME_UTILS_H
