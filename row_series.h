
#ifndef ROW_SERIES_H
#define ROW_SERIES_H

#include <cell.h>
#include <ostream>
#include <stdlib.h>

template<typename Iterable>
class Iterator;

template<typename T>
class DataFrame;

template<typename T>
struct RowData {
  T*          data;
  std::size_t size;

  ~RowData() {
    delete[] data;
  }
};

template<typename T>
struct RowSeries {
  using size_t  = std::size_t;
  using ostream = std::ostream;

  using ValueType         = Cell<T>*;
  using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
  using RowIterator       = Iterator<RowSeries>;

  RowSeries(DataFrameIterator row_begin, std::size_t row_idx, std::size_t row_size) {
    m_size = row_size;
    m_d    = new ValueType[m_size];

    for (int idx = 0; idx < row_size; idx++) {
      m_d[idx] = &(row_begin + ((row_idx * row_size) + idx));
    }
  }

  RowSeries(const RowSeries& other) = delete;

  ~RowSeries() {
    delete[] m_d;
  }

  ValueType operator[](const int& idx) {
    return *(m_d + idx);
  }

  RowData<T> get_data() {
    RowData<T> data;
    data.data = new T[m_size];
    data.size = m_size;
    for (int i = 0; i < m_size; i++) {
      data.data[i] = (*(m_d + i))->value;
    }
    return data;
  }

  friend ostream& operator<<(ostream& os, const RowSeries& row) {
    os << "RowSeries(size: " << row.m_size << ", "
       << "Items: \n";
    for (const ValueType& cell : row) {
      os << *cell << "\n";
    }
    os << ")";
    return os;
  }

  constexpr size_t size() {
    return m_size;
  }

  RowIterator begin() {
    return RowIterator(m_d);
  }
  RowIterator begin() const {
    return RowIterator(m_d);
  }

  RowIterator end() {
    return RowIterator(m_d + m_size);
  }
  RowIterator end() const {
    return RowIterator(m_d + m_size);
  }

  ValueType& at(int idx) {
    return *(begin() + idx);
  }

  private:
  size_t     m_size;
  ValueType* m_d;
};

#endif // COLUMN_SERIES_H
