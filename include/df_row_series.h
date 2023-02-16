
#ifndef DATA_FRAME_ROW_SERIES_H
#define DATA_FRAME_ROW_SERIES_H

#include "df_common.h"

namespace df {

  template<typename Iterable>
  class Iterator;

  template<typename T>
  class DataFrame;

  template<typename T>
  struct RowData {

    RowData(sizetype size) : size(size), data(new T[size]) {
    }

    T*       data;
    sizetype size;

    ~RowData() {
      delete[] data;
    }
  };

  template<typename T>
  struct RowSeries {

    using ValueType         = typename DataFrame<T>::pValueType;
    using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
    using RowIterator       = Iterator<RowSeries>;

    RowSeries(DataFrameIterator row_begin, sizetype row_idx, sizetype row_size) {
      m_size = row_size;
      m_d    = new ValueType[m_size];

      for (sizetype idx = 0; idx < row_size; idx++) {
        m_d[idx] = &(row_begin + ((row_idx * row_size) + idx));
      }
    }

    RowSeries(const RowSeries& other) : m_size(other.m_size), m_d(new ValueType[m_size]) {
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i] = other.m_d[i];
      }
    }

    ~RowSeries() {
      delete[] m_d;
    }

    ValueType operator[](const sizetype& idx) {
      return *(m_d + idx);
    }

    ValueType operator[](const String& col_name) {
      for (sizetype idx = 0; idx < m_size; idx++) {
        if (*(m_d + idx)->idx.col_name == col_name) {
          return *(m_d + idx);
        }
      }

#ifdef QT_IMPLEMENTATION
      qWarning() << "col name not found.";
      qTerminate();
#else
      std::cerr << "col name not found.";
      abort();
#endif
    }

    RowData<T> copy_data() {
      RowData<T> data{m_size};
      for (sizetype i = 0; i < m_size; i++) {
        data.data[i] = (*(m_d + i))->value;
      }
      return data;
    }
#ifdef QT_IMPLEMENTATION
    friend QDebug operator<<(QDebug dbg, const RowSeries& row) {
      dbg.noquote();
      dbg << "RowSeries(size: " << row.m_size << ", "
          << "Items: \n";
      for (const ValueType& cell : row) {
        dbg << *cell << "\n";
      }
      dbg << ")";
      return dbg;
    }
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

    constexpr sizetype size() {
      return m_size;
    }

    constexpr sizetype size() const {
      return m_size;
    }

    constexpr const sizetype& idx() {
      return (*begin())->idx.row_idx;
    }

    constexpr const sizetype& idx() const {
      return (*begin())->idx.row_idx;
    }

    String& name() {
      return (*begin())->idx.row_name;
    }

    const String& name() const {
      return (*begin())->idx.row_name;
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

private:
    sizetype   m_size;
    ValueType* m_d;
  };
} // namespace df

#endif // COLUMN_SERIES_H
