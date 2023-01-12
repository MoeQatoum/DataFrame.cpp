
#ifndef ROW_SERIES_H
#define ROW_SERIES_H

#include <stdlib.h>

#include <cell.h>

template<typename Iterable>
class Iterator;

template<typename T>
class DataFrame;

template<typename T>
struct RowSeries
{
  using size_t  = std::size_t;
  using ostream = std::ostream;

  using ValueType         = Cell<T>;
  using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
  using RowIterator       = Iterator<RowSeries>;

  RowSeries(DataFrameIterator row_begin, DataFrameIterator row_end)
  {
    m_size = row_end - row_begin;
    m_d    = new ValueType[m_size];

    for (size_t i = 0; i < m_size; i++)
    {
      m_d[i] = *(row_begin + i);
    }
  }

  ~RowSeries() { delete[] m_d; }

  ValueType& operator[](const int idx)
  {
    ValueType& item = *(m_d + idx);
    return item;
  }

  friend ostream& operator<<(ostream& os, const RowSeries& row)
  {
    os << "RowSeries(size: " << row.m_size << ", "
       << "Items: \n";
    for (const ValueType& cell : row)
    {
      os << cell << "\n";
    }
    os << ")";
    return os;
  }

  RowIterator begin() { return RowIterator(m_d); }
  RowIterator begin() const { return RowIterator(m_d); }

  RowIterator end() { return RowIterator(m_d + m_size); }
  RowIterator end() const { return RowIterator(m_d + m_size); }

  ValueType& at(int idx) { return *(begin() + idx); }

  private:
  size_t     m_size;
  ValueType* m_d;
};

#endif // COLUMN_SERIES_H
