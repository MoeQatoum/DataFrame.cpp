#ifndef DATA_FRAME_CELL_H
#define DATA_FRAME_CELL_H

#include <iostream>
#include <stdlib.h>

namespace df {

  struct Index {
    using size_t  = std::size_t;
    using string  = std::string;
    using ostream = std::ostream;

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

    string col_name;
    string row_name;

    friend ostream& operator<<(ostream& os, const Index& df_idx) {
      os << "Index(<" << df_idx.global_idx << ">, <" << df_idx.col_name << ", " << df_idx.col_idx << ">, <"
         << df_idx.row_name << ", " << df_idx.row_idx << ">)";
      return os;
    }
  };

  template<typename T>
  struct Cell {
    using ostream = std::ostream;

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

    friend ostream& operator<<(ostream& os, const Cell<T>& cell) {
      os << "Cell(value: " << cell.value << ", " << cell.idx << ")";
      return os;
    }

    T     value;
    Index idx;
  };
} // namespace df

#endif // DATA_FRAME_CELL_H
