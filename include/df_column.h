#ifndef DATA_FRAME_COLUMN_H
#define DATA_FRAME_COLUMN_H

#include "df_common.h"
#include "df_series.h"
namespace df {

  template<typename Iterable>
  class Iterator;

  template<typename T>
  class DataFrame;

  template<typename T>
  struct Column {

    using ValueType         = typename DataFrame<T>::pValueType;
    using DataFrameIterator = typename DataFrame<T>::Iterator;
    using ColIterator       = Iterator<Column>;

    Column(DataFrameIterator col_begin, sizetype col_size, sizetype stride) {
      m_size   = col_size;
      m_stride = stride;
      m_d      = new ValueType[m_size];

      for (sizetype idx = 0; idx < col_size; idx++) {
        m_d[idx] = &(col_begin + (idx * stride));
      }
    }

    Column(const Column& other) = delete;

    ~Column() {
      delete[] m_d;
    }

    ValueType& operator[](const sizetype& idx) {
      return m_d[idx];
    }

    const ValueType& operator[](const sizetype& idx) const {
      return m_d[idx];
    }

    ValueType operator[](const String& row_name) {
      for (sizetype i = 0; i < m_size; i++) {
        if (m_d[i]->idx.row_name == row_name) {
          return m_d[i];
        }
      }

#ifdef QT_IMPLEMENTATION
      qWarning() << "row name not found.";
      qTerminate();
#else
      std::cerr << "row name not found.";
      abort();
#endif
    }

    Column& operator=(const Column& rhs) {
      if (this != &rhs) {
        FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
        for (sizetype i = 0; i < m_size; i++) {
          m_d[i]->value = rhs[i]->value;
        }
      }
      return *this;
    }

    Column& operator=(const Series<T>& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i]->value = rhs[i];
      }
      return *this;
    }

    // comparaison operators
    Series<bool> operator==(const Column& rhs) const {
      FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i]->value == rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator==(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value == rhs[i]);
      }
      return temp;
    }

    friend Series<bool> operator==(const Series<T>& lhs, const Column& rhs) {
      return rhs == lhs;
    }

    friend Series<bool> operator==(const Column& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value == rhs);
      }
      return temp;
    }

    friend Series<bool> operator==(const T& lhs, const Column& rhs) {
      return rhs == lhs;
    }

    Series<bool> operator!=(const Column& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i]->value != rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator!=(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value != rhs[i]);
      }
      return temp;
    }

    friend Series<bool> operator!=(const Series<T>& lhs, const Column& rhs) {
      return rhs != lhs;
    }

    friend Series<bool> operator!=(const Column& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value != rhs);
      }
      return temp;
    }

    friend Series<bool> operator!=(const T& lhs, const Column& rhs) {
      return rhs != lhs;
    }

    Series<bool> operator>=(const Column& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i]->value >= rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator>=(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value >= rhs[i]);
      }
      return temp;
    }

    friend Series<bool> operator>=(const Series<T>& lhs, const Column& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs[i] >= rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator>=(const Column& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value >= rhs);
      }
      return temp;
    }

    friend Series<bool> operator>=(const T& lhs, const Column& rhs) {
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs >= rhs[i]->value);
      }
      return temp;
    }

    Series<bool> operator<=(const Column& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i]->value <= rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator<=(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value <= rhs[i]);
      }
      return temp;
    }

    friend Series<bool> operator<=(const Series<T>& lhs, const Column& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs[i] <= rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator<=(const Column& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value <= rhs);
      }
      return temp;
    }

    friend Series<bool> operator<=(const T& lhs, const Column& rhs) {
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs <= rhs[i]->value);
      }
      return temp;
    }

    Series<bool> operator<(const Column& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i]->value < rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator<(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value < rhs[i]);
      }
      return temp;
    }

    friend Series<bool> operator<(const Series<T>& lhs, const Column& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs[i] < rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator<(const Column& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value < rhs);
      }
      return temp;
    }

    friend Series<bool> operator<(const T& lhs, const Column& rhs) {
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs < rhs[i]->value);
      }
      return temp;
    }

    Series<bool> operator>(const Column& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i]->value > rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator>(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value > rhs[i]);
      }
      return temp;
    }

    friend Series<bool> operator>(const Series<T>& lhs, const Column& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs[i] > rhs[i]->value);
      }
      return temp;
    }

    friend Series<bool> operator>(const Column& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i]->value > rhs);
      }
      return temp;
    }

    friend Series<bool> operator>(const T& lhs, const Column& rhs) {
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs > rhs[i]->value);
      }
      return temp;
    }

    // arithmetic operators
    Series<T> operator+(const Column& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series<T> res(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        res[i] = m_d[i]->value + rhs[i]->value;
      }
      return res;
    }

    friend Series<T> operator+(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "arithmetic operation on nonmatching size objects");
      Series<T> res(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        res[i] = lhs[i]->value + rhs[i];
      }
      return res;
    }

    friend Series<T> operator+(const Series<T>& lhs, const Column& rhs) {
      return rhs + lhs;
    }

    // Column& operator+=(const T& rhs) {
    //   for (sizetype i = 0; i < m_size; i++) {
    //     m_d[i]->value += rhs;
    //   }
    //   return *this;
    // }

    // Column& operator++() {
    //   for (sizetype i = 0; i < m_size; i++) {
    //     ++m_d[i]->value;
    //   }
    //   return *this;
    // }

    Series<T> operator*(const Column& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series<T> res(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        res[i] = m_d[i]->value * rhs[i]->value;
      }
      return res;
    }

    friend Series<T> operator*(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "arithmetic operation on nonmatching size objects");
      Series<T> res(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        res[i] = lhs[i]->value * rhs[i];
      }
      return res;
    }

    friend Series<T> operator*(const Series<T>& lhs, const Column& rhs) {
      return rhs * lhs;
    }

    Series<T> operator-(const Column& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series<T> res(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        res[i] = m_d[i]->value - rhs[i]->value;
      }
      return res;
    }

    friend Series<T> operator-(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "arithmetic operation on nonmatching size objects");
      Series<T> res(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        res[i] = lhs[i]->value - rhs[i];
      }
      return res;
    }

    friend Series<T> operator-(const Series<T>& lhs, const Column& rhs) {
      FORCED_ASSERT(lhs.size() == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series<T> res(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        res[i] = lhs[i] - rhs[i]->value;
      }
      return res;
    }

    // Column& operator-=(const T& rhs) {
    //   for (sizetype i = 0; i < m_size; i++) {
    //     m_d[i]->value -= rhs;
    //   }
    //   return *this;
    // }

    // Column& operator--() {
    //   for (sizetype i = 0; i < m_size; i++) {
    //     --m_d[i]->value;
    //   }
    //   return *this;
    // }

    Series<T> operator/(const Column& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series<T> res(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        res[i] = m_d[i]->value / rhs[i]->value;
      }
      return res;
    }

    friend Series<T> operator/(const Column& lhs, const Series<T>& rhs) {
      FORCED_ASSERT(lhs.m_size == rhs.size(), "arithmetic operation on nonmatching size objects");
      Series<T> res(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        res[i] = lhs[i]->value / rhs[i];
      }
      return res;
    }

    friend Series<T> operator/(const Series<T>& lhs, const Column& rhs) {
      FORCED_ASSERT(lhs.size() == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series<T> res(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        res[i] = lhs[i] / rhs[i]->value;
      }
      return res;
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
    friend QDebug operator<<(QDebug dbg, const Column& col) {
      dbg.noquote().nospace();
      dbg << "Column(addr: " << &col << ", size: " << col.m_size << ", type: " << typeid(T).name() << ")";
      return dbg;
    }
#else
    friend ostream& operator<<(ostream& os, const Column& col) {
      os << "Column(addr: " << &col << ", size: " << col.m_size << ", type: " << typeid(T).name() << ")";
      return os;
    }
#endif

    constexpr sizetype size() {
      return m_size;
    }

    constexpr sizetype size() const {
      return m_size;
    }

    constexpr sizetype stride() {
      return m_stride;
    }

    constexpr sizetype stride() const {
      return m_stride;
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
    sizetype   m_stride;
    ValueType* m_d;
  };
} // namespace df

#endif // COLUMN_SERIES_H
