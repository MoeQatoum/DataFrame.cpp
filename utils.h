#ifndef DATA_FRAME_UTILS_H
#define DATA_FRAME_UTILS_H

#include "dataframe.h"

namespace sDataFrame {

  enum class SortType {
    Accenting,
    Descending
  };

  template<typename T>
  DataFrame<T> sort(DataFrame<T>& df, SortType sortType, bool inplace = false) {
    DataFrame<T>* temp_df;
    if (inplace) {
      temp_df = &df;
    } else {
    }
  }
} // namespace sDataFrame

#endif // DATA_FRAME_UTILS_H
