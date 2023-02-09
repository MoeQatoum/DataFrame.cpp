#ifndef DATA_FRAME_COLUMN_SERIES_H
#define DATA_FRAME_COLUMN_SERIES_H

#include "common.h"

#include "cell.h"

namespace sDataFrame {

  template<typename Iterable>
  class Iterator;

  template<NumericalTypes T>
  class DataFrame;

  template<typename T>
  struct ColumnData {
    T*          data;
    std::size_t size;

    ~ColumnData() {
      delete[] data;
    }
  };

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

    ColumnData<T> copy_data() {
      ColumnData<T> data;
      data.data = new T[m_size];
      data.size = m_size;
      for (int i = 0; i < m_size; i++) {
        data.data[i] = (*(m_d + i))->value;
      }
      return data;
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
} // namespace sDataFrame

#endif // COLUMN_SERIES_H
