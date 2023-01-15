#ifndef COLUMN_SERIES_H
#define COLUMN_SERIES_H

#include <cell.h>
#include <ostream>
#include <stdlib.h>

template<typename Iterable>
class Iterator;

template<typename T>
class DataFrame;

template<typename T>
struct ColumnSeries {
  using size_t  = std::size_t;
  using ostream = std::ostream;

  using ValueType         = Cell<T>*;
  using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
  using Iterator          = Iterator<ColumnSeries>;

  ColumnSeries(DataFrameIterator col_begin, size_t col_size, size_t row_size) {
    m_size = col_size;
    m_d    = new ValueType[m_size];

    for (int idx = 0; idx < col_size; idx++) {
      m_d[idx] = &(col_begin + (idx * row_size));
    }
  }

  ColumnSeries(const ColumnSeries& other) = delete;

  ~ColumnSeries() {
    delete[] m_d;
  }

  ValueType operator[](const int& idx) {
    return *(m_d + idx);
  }

  friend ostream& operator<<(ostream& os, const ColumnSeries& col) {
    os << "ColumnSeries(size: " << col.m_size << ", "
       << "Items: \n";
    for (const ValueType& cell : col) {
      os << *cell << "\n";
    }
    os << ")";
    return os;
  }

  constexpr size_t size() {
    return m_size;
  }

  Iterator begin() {
    return Iterator(m_d);
  }
  Iterator begin() const {
    return Iterator(m_d);
  }

  Iterator end() {
    return Iterator(m_d + m_size);
  }
  Iterator end() const {
    return Iterator(m_d + m_size);
  }

  ValueType& at(int idx) {
    return *(begin() + idx);
  }

  private:
  size_t     m_size;
  ValueType* m_d;
};

#endif // COLUMN_SERIES_H
