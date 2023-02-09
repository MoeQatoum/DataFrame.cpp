#ifndef DATA_FRAME_UTILS_H
#define DATA_FRAME_UTILS_H

#include "common.h"
#include "dataframe.h"

namespace df {

  // template<NumericalTypes T>
  // class DataFrame;

  // template<typename T>
  // class RowSeries;

  // template<typename T>
  // class ColumnSeries;

  // template<typename T>
  // class ColumnData;

  // template<typename T>
  // class Cell;

  namespace utils {

    template<typename T>
    std::vector<RowSeries<T>> asc_sort_rows(DataFrame<T>& df, std::string col_name) {
      using ValueType = typename ColumnSeries<T>::ValueType;

      size_t          col_idx = df.get_col_idx(col_name);
      ColumnSeries<T> col     = df.get_column(col_name);

      ValueType* sorted_cells = new ValueType[col.size()];

      std::vector<RowSeries<T>> rows;
      for (auto row = df.iter_rows(); row < df.end(); row++) {
        rows.push_back(row.row());
      }

      // check if row copy works ... it does.
      // for (int i = 0; i < rows.size(); i++) {
      //   // std::cout << rows[i];
      //   for (const auto& row_item : rows[i]) {
      //     std::cout << *row_item << "\n";
      //   }
      // }

      sorted_cells[0] = col[0];

      for (size_t idx = 0; idx < col.size(); idx++) {
        bool lower_found = false;
        int  insert_idx  = 0;
        for (size_t sorted_idx = 0; sorted_idx <= idx; sorted_idx++) {
          if (col[idx]->value < sorted_cells[sorted_idx]->value) {
            lower_found = true;
            insert_idx  = sorted_idx;
            std::cout << "lower found, "
                      << "col_value: " << col[idx]->value << ", sorted_value: " << sorted_cells[sorted_idx]->value
                      << " ,insert_idx: " << insert_idx << "\n";
            break;
          }
        }
        if (lower_found) {
          for (int i = idx - 1; i >= insert_idx; i--) {
            sorted_cells[i + 1] = sorted_cells[i];
            std::cout << i << " -> " << i + 1 << " " << (i >= insert_idx) << "\n";
          }
          std::cout << "-----\n";
          sorted_cells[insert_idx] = col[idx];
        } else {
          sorted_cells[idx] = col[idx];
        }
      }
      std::cout << "sort finished.\n";
      std::vector<RowSeries<T>> sorted_rows;
      // sort
      for (size_t idx = 0; idx < col.size(); idx++) {
        sorted_rows.push_back(df.get_row(sorted_cells[idx]->idx.row_idx));
        std::cout << sorted_cells[idx]->value << " " << sorted_cells[idx]->idx.row_name << "\n";
      }

      for (int i = 0; i < sorted_rows.size(); i++) {
        std::cout << sorted_rows[i];
      }

      delete[] sorted_cells;
      return sorted_rows;
    }

  } // namespace utils
} // namespace df

#endif // DATA_FRAME_UTILS_H
