
#ifndef DATA_FRAME_ROW_H
#define DATA_FRAME_ROW_H

#include "df_common.h"

namespace df {

  template<typename Iterable>
  class Iterator;

  template<typename T>
  class Series;

  template<typename T>
  class DataFrame;

  template<typename T>
  struct Row {

    using ValueType         = typename DataFrame<T>::pValueType;
    using DataFrameIterator = typename DataFrame<T>::Iterator;
    using RowIterator       = Iterator<Row>;

    Row(DataFrameIterator df_begin, sizetype row_idx, sizetype row_size) {
      m_size = row_size;
      m_d    = new ValueType[m_size];

      for (sizetype idx = 0; idx < row_size; idx++) {
        m_d[idx] = &(df_begin + ((row_idx * row_size) + idx));
      }
    }

    Row(const Row& other) : m_size(other.m_size), m_d(new ValueType[m_size]) {
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i] = other.m_d[i];
      }
    }

    ~Row() {
      delete[] m_d;
    }

    ValueType& operator[](const sizetype& idx) {
      return m_d[idx];
    }

    const ValueType& operator[](const sizetype& idx) const {
      return m_d[idx];
    }

    ValueType operator[](const String& col_name) {
      for (sizetype i = 0; i < m_size; i++) {
        if (m_d[i]->idx.col_name == col_name) {
          return m_d[i];
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

    Row& operator=(const Row& rhs) {
      if (this != &rhs) {
        FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
        for (sizetype i = 0; i < m_size; i++) {
          m_d[i]->value = rhs[i]->value;
        }
      }
      return *this;
    }

    Row& operator=(const Series<T>& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i]->value = rhs[i];
      }
    }

    Series<T> copy_data() {
      Series<T> data(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        data[i] = m_d[i]->value;
      }
      return data;
    }

    Series<T> copy_data() const {
      Series<T> data(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        data[i] = m_d[i]->value;
      }
      return data;
    }

#ifdef QT_IMPLEMENTATION
    friend QDebug operator<<(QDebug dbg, const Row& row) {
      dbg.noquote().nospace();
      dbg << "Row(addr: " << &row << ", size: " << row.m_size << ", type: " << typeid(T).name() << ")";
      return dbg;
    }
#else
    friend ostream& operator<<(ostream& os, const Row& row) {
      os << "Row(addr: " << &row << ", size: " << row.m_size << ", type: " << typeid(T).name() << ")";
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
