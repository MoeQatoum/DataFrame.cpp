#ifndef DATA_FRAME_UTILS_H
#define DATA_FRAME_UTILS_H

#include "df_common.h"

namespace df {
  template<NumericalTypes T>
  class DataFrame;

  template<NumericalTypes T>
  class RowSeries;

  template<NumericalTypes T>
  class ColumnSeries;

  template<NumericalTypes T>
  List<RowSeries<T>> asc_sort_rows(DataFrame<T>& df, const String& col_name) {
    using ValueType = typename ColumnSeries<T>::ValueType;

    sizetype        col_idx = df.get_col_idx(col_name);
    ColumnSeries<T> col     = df.get_column(col_name);

    ValueType* sorted_cells = new ValueType[col.size()];

    List<RowSeries<T>> rows;
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
    List<RowSeries<T>> sorted_rows;
    for (sizetype idx = 0; idx < col.size(); idx++) {
      sorted_rows.push_back(df.get_row(sorted_cells[idx]->idx.row_idx));
    }

    delete[] sorted_cells;
    return sorted_rows;
  }

  template<NumericalTypes T>
  List<RowSeries<T>> dec_sort_rows(DataFrame<T>& df, const String& col_name) {
    using ValueType = typename ColumnSeries<T>::ValueType;

    sizetype        col_idx = df.get_col_idx(col_name);
    ColumnSeries<T> col     = df.get_column(col_name);

    ValueType* sorted_cells = new ValueType[col.size()];

    List<RowSeries<T>> rows;
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
    List<RowSeries<T>> sorted_rows;
    for (sizetype idx = 0; idx < col.size(); idx++) {
      sorted_rows.push_back(df.get_row(sorted_cells[idx]->idx.row_idx));
    }

    delete[] sorted_cells;
    return sorted_rows;
  }

} // namespace df

#endif // DATA_FRAME_UTILS_H
