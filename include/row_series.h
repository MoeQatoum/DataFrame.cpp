
#ifndef DATA_FRAME_ROW_SERIES_H
#define DATA_FRAME_ROW_SERIES_H

#include "common.h"

namespace df {

  template<typename Iterable>
  class Iterator;

  template<NumericalTypes T>
  class DataFrame;

  template<NumericalTypes T>
  struct RowData {

    RowData(size_t size) : size(size), data(new T[size]) {
    }

    T*     data;
    size_t size;

    ~RowData() {
      delete[] data;
    }
  };

  template<NumericalTypes T>
  struct RowSeries {

    using ValueType         = typename DataFrame<T>::pValueType;
    using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
    using RowIterator       = Iterator<RowSeries>;

    RowSeries(DataFrameIterator row_begin, size_t row_idx, size_t row_size) {
      m_size = row_size;
      m_d    = new ValueType[m_size];

      for (size_t idx = 0; idx < row_size; idx++) {
        m_d[idx] = &(row_begin + ((row_idx * row_size) + idx));
      }
    }

    RowSeries(const RowSeries& other) : m_size(other.m_size), m_d(new ValueType[m_size]) {
      for (size_t i = 0; i < m_size; i++) {
        m_d[i] = other.m_d[i];
      }
    }

    ~RowSeries() {
      delete[] m_d;
    }

    ValueType operator[](const size_t& idx) {
      return *(m_d + idx);
    }

    RowData<T> copy_data() {
      RowData<T> data{m_size};
      for (size_t i = 0; i < m_size; i++) {
        data.data[i] = (*(m_d + i))->value;
      }
      return data;
    }
#ifdef QT_IMPLEMENTATION
#else
    friend ostream& operator<<(ostream& os, const RowSeries& row) {
      os << "RowSeries(size: " << row.m_size << ", "
         << "Items: \n";
      for (const ValueType& cell : row) {
        os << *cell << "\n";
      }
      os << ")";
      return os;
    }
#endif

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

    ValueType& at(size_t idx) {
      return *(begin() + idx);
    }

private:
    size_t     m_size;
    ValueType* m_d;
  };
} // namespace df

#endif // COLUMN_SERIES_H
