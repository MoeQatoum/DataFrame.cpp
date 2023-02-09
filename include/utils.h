#ifndef DATA_FRAME_UTILS_H
#define DATA_FRAME_UTILS_H

#include "common.h"

namespace sDataFrame {

  template<NumericalTypes T>
  class DataFrame;

  template<typename T>
  class RowSeries;

  template<typename T>
  class ColumnData;

  namespace utils {

    enum SortType {
      Accenting,
      Descending
    };

    template<typename T>
    DataFrame<T> sort_rows(DataFrame<T>& df, std::string col_name, SortType sortType, bool inplace = false) {
      DataFrame<T>* temp_df;
      if (inplace) {
        temp_df = &df;
      } else {
        temp_df = new DataFrame<T>(df);
      }

      size_t        col_idx  = temp_df->get_col_idx(col_name);
      ColumnData<T> col_data = temp_df->get_column(col_name).copy_data();

      T*      sorted_data     = new T[col_data.size];
      size_t* sorted_data_idx = new size_t[col_data.size];

      std::vector<RowSeries<T>> rows;
      for (auto row = temp_df->iter_rows(); row < temp_df->end(); row++) {
        rows.push_back(row.row());
      }

      // check if row copy works ... it does.
      // for (int i = 0; i < rows.size(); i++) {
      //   // std::cout << rows[i];
      //   for (const auto& row_item : rows[i]) {
      //     std::cout << *row_item << "\n";
      //   }
      // }

      for (int i = 0; i < rows.size(); i++) {
        T temp_val = 0;
        for (const auto& cell : rows[i]) {
          switch (sortType) {
          case Accenting:
            break;
          case Descending:
            break;
          }
        }
      }

      delete[] sorted_data;
      delete[] sorted_data_idx;

      // if (inplace) {
      //   return df;
      // } else {
      //   return *temp_dfNumericalTypes;
      // }
      return *temp_df;
    }

  } // namespace utils
} // namespace sDataFrame

#endif // DATA_FRAME_UTILS_H
