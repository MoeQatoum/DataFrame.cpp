#ifndef DATA_FRAME_SERIES_H
#define DATA_FRAME_SERIES_H

#include "df_common.h"

namespace df {
  template<typename Iterable>
  class Iterator;

  template<typename T>
  struct Series {
public:
    using ValueType      = T;
    using SeriesIterator = Iterator<Series>;

    explicit Series(const sizetype& size) : m_d(new T[size]), m_size(size) {
    }

    Series(const std::initializer_list<T>& il) : m_size(il.size()), m_d(new T[il.size()]) {
      std::copy(il.begin(), il.end(), m_d);
    }

    ~Series() {
      delete[] m_d;
    }

    Series(const Series& other) {
      m_size = other.m_size;
      m_d    = new T[m_size];
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i] = other[i];
      }
    }

    Series& operator=(const Series& other) {
      FORCED_ASSERT(m_size == other.m_size, "copy assignment operator on nonmatching size objects");
      if (this != &other) {
        for (sizetype i = 0; i < m_size; i++) {
          m_d[i] = other[i];
        }
      }
      return *this;
    }

    T& operator[](sizetype idx) {
      return *(m_d + idx);
    }

    const T& operator[](sizetype idx) const {
      return *(m_d + idx);
    }

    // comparaison operators
    Series<bool> operator==(const Series& other) const {
      FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i] == other[i]);
      }
      return temp;
    }

    friend Series<bool> operator==(const Series& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i] == rhs);
      }
      return temp;
    }

    friend Series<bool> operator==(const T& lhs, const Series& rhs) {
      return rhs == lhs;
    }

    Series<bool> operator!=(const Series& other) {
      FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i] != other[i]);
      }
      return temp;
    }

    friend Series<bool> operator!=(const Series& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i] != rhs);
      }
      return temp;
    }

    friend Series<bool> operator!=(const T& lhs, const Series& rhs) {
      return rhs != lhs;
    }

    Series<bool> operator>=(const Series& other) {
      FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i] >= other[i]);
      }
      return temp;
    }

    friend Series<bool> operator>=(const Series& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i] >= rhs);
      }
      return temp;
    }

    friend Series<bool> operator>=(const T& lhs, const Series& rhs) {
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs >= rhs[i]);
      }
      return temp;
    }

    Series<bool> operator<=(const Series& other) {
      FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i] <= other[i]);
      }
      return temp;
    }

    friend Series<bool> operator<=(const Series& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i] <= rhs);
      }
      return temp;
    }

    friend Series<bool> operator<=(const T& lhs, const Series& rhs) {
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs <= rhs[i]);
      }
      return temp;
    }

    Series<bool> operator<(const Series& other) {
      FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i] < other[i]);
      }
      return temp;
    }

    friend Series<bool> operator<(const Series& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i] < rhs);
      }
      return temp;
    }

    friend Series<bool> operator<(const T& lhs, const Series& rhs) {
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs < rhs[i]);
      }
      return temp;
    }

    Series<bool> operator>(const Series& other) {
      FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
      Series<bool> temp(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        temp[i] = (m_d[i] > other[i]);
      }
      return temp;
    }

    friend Series<bool> operator>(const Series& lhs, const T& rhs) {
      Series<bool> temp(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        temp[i] = (lhs[i] > rhs);
      }
      return temp;
    }

    friend Series<bool> operator>(const T& lhs, const Series& rhs) {
      Series<bool> temp(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        temp[i] = (lhs > rhs[i]);
      }
      return temp;
    }

    // arithmetic operators
    Series operator+(const Series& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series res(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        res[i] = m_d[i] + rhs[i];
      }
      return res;
    }

    friend Series operator+(const Series& lhs, const T& rhs) {
      Series res(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        res[i] = lhs[i] + rhs;
      }
      return res;
    }

    friend Series operator+(const T& lhs, const Series& rhs) {
      return rhs + lhs;
    }

    Series operator+=(const T& rhs) {
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i] += rhs;
      }
      return *this;
    }

    Series operator++(int) {
      Series temp{*this};
      ++(*this);
      return temp;
    }

    Series operator++() {
      for (sizetype i = 0; i < m_size; i++) {
        ++m_d[i];
      }
      return *this;
    }

    Series operator*(const Series& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series res(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        res[i] = m_d[i] * rhs[i];
      }
      return res;
    }

    friend Series operator*(const Series& lhs, const T& rhs) {
      Series res(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        res[i] = lhs[i] * rhs;
      }
      return res;
    }

    friend Series operator*(const T& lhs, const Series& rhs) {
      return rhs * lhs;
    }

    Series operator-(const Series& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series res(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        res[i] = m_d[i] - rhs[i];
      }
      return res;
    }

    friend Series operator-(const Series& lhs, const T& rhs) {
      Series res(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        res[i] = lhs[i] - rhs;
      }
      return res;
    }

    friend Series operator-(const T& lhs, const Series& rhs) {
      Series res(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        res[i] = lhs - rhs[i];
      }
      return res;
    }

    Series operator-=(const T& rhs) {
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i] -= rhs;
      }
      return *this;
    }

    Series operator--(int) {
      Series tmp{*this};
      --(*this);
      return tmp;
    }

    Series operator--() {
      for (sizetype i = 0; i < m_size; i++) {
        --m_d[i];
      }
      return *this;
    }

    Series operator/(const Series& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
      Series res(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        res[i] = m_d[i] / rhs[i];
      }
      return res;
    }

    friend Series operator/(const Series& lhs, const T& rhs) {
      Series res(lhs.m_size);
      for (sizetype i = 0; i < lhs.m_size; i++) {
        res[i] = lhs[i] / rhs;
      }
      return res;
    }

    friend Series operator/(const T& lhs, const Series& rhs) {
      Series res(rhs.m_size);
      for (sizetype i = 0; i < rhs.m_size; i++) {
        res[i] = lhs / rhs[i];
      }
      return res;
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    T max() {
      T temp = m_d[0]->value;
      for (sizetype i = 1; i < m_size; ++i) {
        if (m_d[i]->value > temp) {
          temp = m_d[i]->value;
        }
      }
      return temp;
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    T min() {
      T temp = m_d[0]->value;
      for (sizetype i = 1; i < m_size; ++i) {
        if (m_d[i]->value < temp) {
          temp = m_d[i]->value;
        }
      }
      return temp;
    }

    bool is_equal_with(const Series& other) {
      FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
      for (sizetype i = 0; i < m_size; i++) {
        if (m_d[i] != other[i]) {
          return false;
        }
      }
      return true;
    }

    SeriesIterator begin() {
      return SeriesIterator(m_d);
    }

    SeriesIterator begin() const {
      return SeriesIterator(m_d);
    }

    SeriesIterator end() {
      return SeriesIterator(m_d + m_size);
    }

    SeriesIterator end() const {
      return SeriesIterator(m_d + m_size);
    }

    sizetype size() {
      return m_size;
    }

    sizetype size() const {
      return m_size;
    }

    T* data() {
      return m_d;
    }

    const T* data() const {
      return m_d;
    }

private:
    T*       m_d;
    sizetype m_size;
  };

} // namespace df

#endif // DATA_FRAME_SERIES_H
