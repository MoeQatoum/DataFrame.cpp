#ifndef DATA_FRAME_CELL_H
#define DATA_FRAME_CELL_H

#include "common.h"

namespace df {

  struct Index {

    Index() {
    }

    Index(const Index& other)
        : row_idx(other.row_idx),
          col_idx(other.col_idx),
          global_idx(other.global_idx),
          col_name(other.col_name),
          row_name(other.row_name) {
    }

    // Index(Index&& other) = delete;

    // Index operator=(const Index& other) {
    //   Index temp_idx;
    //   temp_idx.row_idx    = other.row_idx;
    //   temp_idx.col_idx    = other.col_idx;
    //   temp_idx.global_idx = other.global_idx;
    //   temp_idx.col_name   = other.col_name;
    //   temp_idx.row_name   = other.row_name;
    //   return temp_idx;
    // }

    size_t row_idx;
    size_t col_idx;
    size_t global_idx;

    String col_name;
    String row_name;

#ifdef QT_IMPLEMENTATION
#else
    friend ostream& operator<<(ostream& os, const Index& df_idx) {
      os << "Index(<" << df_idx.global_idx << ">, <" << df_idx.col_name << ", " << df_idx.col_idx << ">, <"
         << df_idx.row_name << ", " << df_idx.row_idx << ">)";
      return os;
    }
#endif
  };

  template<NumericalTypes T>
  struct Cell {

    Cell() {
    }

    Cell(const Cell& other) : value(other.value), idx(other.idx) {
    }

    // Cell(Cell&& other) = delete;

    Cell<T> operator=(const Cell<T>& other) {
      value = other.value;
      idx   = other.idx;
      // return temp_cell;
      // return Cell(other);
      return *this;
    }

    void operator=(const T val) {
      value = val;
    }

#ifdef QT_IMPLEMENTATION
#else
    friend ostream& operator<<(ostream& os, const Cell<T>& cell) {
      os << "Cell(value: " << cell.value << ", " << cell.idx << ")";
      return os;
    }
#endif

    T     value;
    Index idx;
  };
} // namespace df

#endif // DATA_FRAME_CELL_H
