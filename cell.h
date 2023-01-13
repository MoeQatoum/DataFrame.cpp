#ifndef DATA_FRAME_CELL_H
#define DATA_FRAME_CELL_H

#include <iostream>
#include <stdlib.h>

struct Index {
  using size_t  = std::size_t;
  using string  = std::string;
  using ostream = std::ostream;

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

  using ValueType = T;

  // Cell() { std::cout << "Cell def constructor\n"; }

  // Cell(const Cell& other) : value(other.value), idx(other.idx) { std::cout << "Cell def constructor\n"; }

  // Cell(Cell&& other) : value(other.value), idx(other.idx) { std::cout << "Cell def constructor\n"; }

  ~Cell() {
    // std::cout << "Cell destroyed\n";
  }

  ValueType value;
  Index     idx;

  void operator=(const ValueType val) {
    // std::cout << "Cell operator=\n";
    value = val;
  }

  friend ostream& operator<<(ostream& os, const Cell<ValueType>& cell) {
    os << "Cell(value: " << cell.value << ", " << cell.idx << ")";
    return os;
  }
};

#endif // DATA_FRAME_CELL_H
