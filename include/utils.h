#ifndef DATA_FRAME_UTILS_H
#define DATA_FRAME_UTILS_H

#include "common.h"

namespace df {
  template<NumericalTypes T>
  class DataFrame;

  template<typename T>
  class RowSeries;

  template<typename T>
  class ColumnSeries;

  namespace utils {

    template<typename T>
    std::vector<RowSeries<T>> asc_sort_rows(DataFrame<T>& df, const std::string& col_name) {
      using ValueType = typename ColumnSeries<T>::ValueType;

      size_t          col_idx = df.get_col_idx(col_name);
      ColumnSeries<T> col     = df.get_column(col_name);

      ValueType* sorted_cells = new ValueType[col.size()];

      std::vector<RowSeries<T>> rows;
      for (auto row = df.iter_rows(); row < df.end(); row++) {
        rows.push_back(row.row());
      }

      sorted_cells[0] = col[0];

      for (int idx = 0; idx < col.size(); idx++) {
        bool lower_found = false;
        int  insert_idx  = 0;
        for (int sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
          if (col[idx]->value < sorted_cells[sorted_idx]->value) {
            lower_found = true;
            insert_idx  = sorted_idx;
            break;
          }
        }
        if (lower_found) {
          for (int i = idx - 1; i >= insert_idx; i--) {
            sorted_cells[i + 1] = sorted_cells[i];
          }
          sorted_cells[insert_idx] = col[idx];
        } else {
          sorted_cells[idx] = col[idx];
        }
      }

      // sort
      std::vector<RowSeries<T>> sorted_rows;
      for (size_t idx = 0; idx < col.size(); idx++) {
        sorted_rows.push_back(df.get_row(sorted_cells[idx]->idx.row_idx));
      }

      delete[] sorted_cells;
      return sorted_rows;
    }

  } // namespace utils
} // namespace df

#endif // DATA_FRAME_UTILS_H
