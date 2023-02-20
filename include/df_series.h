#ifndef DATA_FRAME_SERIES_H
#define DATA_FRAME_SERIES_H

#include "df_common.h"

namespace df {
  template<typename T>
  struct Series {

    explicit Series(sizetype data_size) : size(data_size), d(new T[data_size]) {
    }

    Series(sizetype data_size, T fill) : size(data_size), d(new T[data_size]) {
      for (sizetype i = 0; i < data_size; i++) {
        d[i] = fill;
      }
    }

    ~Series() {
      delete[] d;
    }

    Series(const Series& other) {
      size = other.size;
      d    = new T[size];
      for (sizetype i = 0; i < size; i++) {
        d[i] = other[i];
      }
    }

    Series& operator=(const Series& other) {
      FORCED_ASSERT(size == other.size, "copy assignment operator on nonmatching size objects");
      if (this != &other) {
        for (sizetype i = 0; i < size; i++) {
          d[i] = other[i];
        }
      }
      return *this;
    }

    T& operator[](sizetype idx) {
      return *(d + idx);
    }

    const T& operator[](sizetype idx) const {
      return *(d + idx);
    }

    // comparaison operators
    Series<bool> operator==(const Series& other) const
      requires(std::equality_comparable<T>)
    {
      FORCED_ASSERT(size == other.size, "comparaison operations on nonmatching size objects");
      Series<bool> temp{size};
      for (sizetype i = 0; i < size; i++) {
        temp[i] = (d[i] == other[i]);
      }
      return temp;
    }

    friend Series<bool> operator==(const Series& lhs, const T& rhs)
      requires(std::equality_comparable<T>)
    {
      Series<bool> temp{lhs.size};
      for (sizetype i = 0; i < lhs.size; i++) {
        temp[i] = (lhs[i] == rhs);
      }
      return temp;
    }

    friend Series<bool> operator==(const T& lhs, const Series& rhs)
      requires(std::equality_comparable<T>)
    {
      return rhs == lhs;
    }

    Series<bool> operator!=(const Series& other)
      requires(std::equality_comparable<T>)
    {
      FORCED_ASSERT(size == other.size, "comparaison operations on nonmatching size objects");
      Series<bool> temp{size};
      for (sizetype i = 0; i < size; i++) {
        temp[i] = (d[i] != other[i]);
      }
      return temp;
    }

    friend Series<bool> operator!=(const Series& lhs, const T& rhs)
      requires(std::equality_comparable<T>)
    {
      Series<bool> temp{lhs.size};
      for (sizetype i = 0; i < lhs.size; i++) {
        temp[i] = (lhs[i] != rhs);
      }
      return temp;
    }

    friend Series<bool> operator!=(const T& lhs, const Series& rhs)
      requires(std::equality_comparable<T>)
    {
      return rhs != lhs;
    }

    Series<bool> operator>=(const Series& other)
      requires(std::totally_ordered<T>)
    {
      FORCED_ASSERT(size == other.size, "comparaison operations on nonmatching size objects");
      Series<bool> temp{size};
      for (sizetype i = 0; i < size; i++) {
        temp[i] = (d[i] >= other[i]);
      }
      return temp;
    }

    friend Series<bool> operator>=(const Series& lhs, const T& rhs)
      requires(std::totally_ordered<T>)
    {
      Series<bool> temp{lhs.size};
      for (sizetype i = 0; i < lhs.size; i++) {
        temp[i] = (lhs[i] >= rhs);
      }
      return temp;
    }

    friend Series<bool> operator>=(const T& lhs, const Series& rhs)
      requires(std::totally_ordered<T>)
    {
      Series<bool> temp{rhs.size};
      for (sizetype i = 0; i < rhs.size; i++) {
        temp[i] = (lhs >= rhs[i]);
      }
      return temp;
    }

    Series<bool> operator<=(const Series& other)
      requires(std::totally_ordered<T>)
    {
      FORCED_ASSERT(size == other.size, "comparaison operations on nonmatching size objects");
      Series<bool> temp{size};
      for (sizetype i = 0; i < size; i++) {
        temp[i] = (d[i] <= other[i]);
      }
      return temp;
    }

    friend Series<bool> operator<=(const Series& lhs, const T& rhs)
      requires(std::totally_ordered<T>)
    {
      Series<bool> temp{lhs.size};
      for (sizetype i = 0; i < lhs.size; i++) {
        temp[i] = (lhs[i] <= rhs);
      }
      return temp;
    }

    friend Series<bool> operator<=(const T& lhs, const Series& rhs)
      requires(std::totally_ordered<T>)
    {
      Series<bool> temp{rhs.size};
      for (sizetype i = 0; i < rhs.size; i++) {
        temp[i] = (lhs <= rhs[i]);
      }
      return temp;
    }

    Series<bool> operator<(const Series& other)
      requires(std::totally_ordered<T>)
    {
      FORCED_ASSERT(size == other.size, "comparaison operations on nonmatching size objects");
      Series<bool> temp{size};
      for (sizetype i = 0; i < size; i++) {
        temp[i] = (d[i] < other[i]);
      }
      return temp;
    }

    friend Series<bool> operator<(const Series& lhs, const T& rhs)
      requires(std::totally_ordered<T>)
    {
      Series<bool> temp{lhs.size};
      for (sizetype i = 0; i < lhs.size; i++) {
        temp[i] = (lhs[i] < rhs);
      }
      return temp;
    }

    friend Series<bool> operator<(const T& lhs, const Series& rhs)
      requires(std::totally_ordered<T>)
    {
      Series<bool> temp{rhs.size};
      for (sizetype i = 0; i < rhs.size; i++) {
        temp[i] = (lhs < rhs[i]);
      }
      return temp;
    }

    Series<bool> operator>(const Series& other)
      requires(std::totally_ordered<T>)
    {
      FORCED_ASSERT(size == other.size, "comparaison operations on nonmatching size objects");
      Series<bool> temp{size};
      for (sizetype i = 0; i < size; i++) {
        temp[i] = (d[i] > other[i]);
      }
      return temp;
    }

    friend Series<bool> operator>(const Series& lhs, const T& rhs)
      requires(std::totally_ordered<T>)
    {
      Series<bool> temp{lhs.size};
      for (sizetype i = 0; i < lhs.size; i++) {
        temp[i] = (lhs[i] > rhs);
      }
      return temp;
    }

    friend Series<bool> operator>(const T& lhs, const Series& rhs)
      requires(std::totally_ordered<T>)
    {
      Series<bool> temp{rhs.size};
      for (sizetype i = 0; i < rhs.size; i++) {
        temp[i] = (lhs > rhs[i]);
      }
      return temp;
    }

    // arithmetic operators
    Series operator+(const Series& rhs) {
      Series res{size};
      for (sizetype i = 0; i < size; i++) {
        res[i] = d[i] + rhs[i];
      }
      return res;
    }

    friend Series operator+(const Series& lhs, const T& rhs) {
      Series res{lhs.size};
      for (sizetype i = 0; i < lhs.size; i++) {
        res[i] = lhs[i] + rhs;
      }
      return res;
    }

    friend Series operator+(const T& lhs, const Series& rhs) {
      return rhs + lhs;
    }

    Series operator+=(const T& rhs) {
      for (sizetype i = 0; i < size; i++) {
        d[i] += rhs;
      }
      return *this;
    }

    Series operator++(int) {
      Series temp{*this};
      ++(*this);
      return temp;
    }

    Series operator++() {
      for (sizetype i = 0; i < size; i++) {
        ++d[i];
      }
      return *this;
    }

    Series operator-(const Series& rhs) {
      Series res{size};
      for (sizetype i = 0; i < size; i++) {
        res[i] = d[i] - rhs[i];
      }
      return res;
    }

    friend Series operator-(const Series& lhs, const T& rhs) {
      Series res{lhs.size};
      for (sizetype i = 0; i < lhs.size; i++) {
        res[i] = lhs[i] - rhs;
      }
      return res;
    }

    friend Series operator-(const T& lhs, const Series& rhs) {
      Series res{rhs.size};
      for (sizetype i = 0; i < rhs.size; i++) {
        res[i] = lhs - rhs[i];
      }
      return res;
    }

    Series operator-=(const T& rhs) {
      for (sizetype i = 0; i < size; i++) {
        d[i] -= rhs;
      }
      return *this;
    }

    Series operator--(int) {
      Series tmp{*this};
      --(*this);
      return tmp;
    }

    Series operator--() {
      for (sizetype i = 0; i < size; i++) {
        --d[i];
      }
      return *this;
    }

    bool is_equal_with(const Series& other) {
      FORCED_ASSERT(size == other.size, "comparaison operations on nonmatching size objects");
      for (sizetype i = 0; i < size; i++) {
        if (d[i] != other[i]) {
          return false;
        }
      }
      return true;
    }

    T*       d;
    sizetype size;
  };

} // namespace df

#endif // DATA_FRAME_SERIES_H
