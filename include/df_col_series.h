#ifndef DATA_FRAME_COLUMN_SERIES_H
#define DATA_FRAME_COLUMN_SERIES_H

#include "df_common.h"

namespace df {

  template<typename Iterable>
  class Iterator;

  template<typename T>
  class DataFrame;

  template<typename T>
  struct ColumnData {

    ColumnData(sizetype size) : size(size), data(new T[size]) {
    }

    T*       data;
    sizetype size;

    ~ColumnData() {
      delete[] data;
    }
  };

  template<typename T>
  struct ColumnSeries {

    using ValueType         = typename DataFrame<T>::pValueType;
    using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
    using ColIterator       = Iterator<ColumnSeries>;

    ColumnSeries(DataFrameIterator col_begin, sizetype col_size, sizetype row_size) {
      m_size = col_size;
      m_d    = new ValueType[m_size];

      for (sizetype idx = 0; idx < col_size; idx++) {
        m_d[idx] = &(col_begin + (idx * row_size));
      }
    }

    ColumnSeries(const ColumnSeries& other) = delete;

    ~ColumnSeries() {
      delete[] m_d;
    }

    ValueType& operator[](const sizetype& idx) {
      return *(m_d + idx);
    }

    ColumnData<T> copy_data() {
      ColumnData<T> data{m_size};
      for (sizetype i = 0; i < m_size; i++) {
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

    constexpr sizetype size() {
      return m_size;
    }

    constexpr sizetype size() const {
      return m_size;
    }

    constexpr sizetype idx() {
      return (*begin())->idx.col_idx;
    }

    constexpr const sizetype& idx() const {
      return (*begin())->idx.col_idx;
    }

    String name() {
      return (*begin())->idx.col_name;
    }

    const String& name() const {
      return (*begin())->idx.col_name;
    }

    ColIterator begin() {
      return ColIterator(m_d);
    }

    ColIterator begin() const {
      return ColIterator(m_d);
    }

    ColIterator end() {
      return ColIterator(m_d + m_size);
    }

    ColIterator end() const {
      return ColIterator(m_d + m_size);
    }

private:
    sizetype   m_size;
    ValueType* m_d;
  };
} // namespace df

#endif // COLUMN_SERIES_H
