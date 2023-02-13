#ifndef DATA_FRAME_COLUMN_SERIES_H
#define DATA_FRAME_COLUMN_SERIES_H

#include "df_common.h"

namespace df {

  template<typename Iterable>
  class Iterator;

  template<NumericalTypes T>
  class DataFrame;

  template<NumericalTypes T>
  struct ColumnData {

    ColumnData(size_t size) : size(size), data(new T[size]) {
    }

    T*     data;
    size_t size;

    ~ColumnData() {
      delete[] data;
    }
  };

  template<NumericalTypes T>
  struct ColumnSeries {

    using ValueType         = typename DataFrame<T>::pValueType;
    using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
    using Iterator          = Iterator<ColumnSeries>;

    ColumnSeries(DataFrameIterator col_begin, size_t col_size, size_t row_size) {
      m_size = col_size;
      m_d    = new ValueType[m_size];

      for (size_t idx = 0; idx < col_size; idx++) {
        m_d[idx] = &(col_begin + (idx * row_size));
      }
    }

    ColumnSeries(const ColumnSeries& other) = delete;

    ~ColumnSeries() {
      delete[] m_d;
    }

    ValueType& operator[](const size_t& idx) {
      return *(m_d + idx);
    }

    ColumnData<T> copy_data() {
      ColumnData<T> data{m_size};
      for (size_t i = 0; i < m_size; i++) {
        data.data[i] = (*(m_d + i))->value;
      }
      return data;
    }
#ifdef QT_IMPLEMENTATION
    friend QDebug operator<<(QDebug dbg, const ColumnSeries& col) {
      dbg.noquote();
      dbg << "ColumnSeries(size: " << col.m_size << ", "
          << "Items: \n";
      for (const ValueType& cell : col) {
        dbg << *cell << "\n";
      }
      dbg << ")";
      return dbg;
    }
#else
    friend ostream& operator<<(ostream& os, const ColumnSeries& col) {
      os << "ColumnSeries(size: " << col.m_size << ", "
         << "Items: \n";
      for (const ValueType& cell : col) {
        os << *cell << "\n";
      }
      os << ")";
      return os;
    }
#endif

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

    ValueType& at(size_t idx) {
      return *(begin() + idx);
    }

private:
    size_t     m_size;
    ValueType* m_d;
  };
} // namespace df

#endif // COLUMN_SERIES_H
