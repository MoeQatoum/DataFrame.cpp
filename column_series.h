#ifndef COLUMN_SERIES_H
#define COLUMN_SERIES_H

#include <stdlib.h>

#include <cell.h>

template<typename Iterable>
class Iterator;

template<typename T>
class DataFrame;

template<typename T>
struct ColumnSeries
{
  using size_t  = std::size_t;
  using ostream = std::ostream;

  using ValueType         = Cell<T>;
  using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
  using Iterator          = Iterator<ColumnSeries>;

  ColumnSeries(DataFrameIterator df_begin, DataFrameIterator df_end, size_t col_idx, size_t row_size)
  {
    m_size = (df_end - df_begin) / row_size;
    m_d    = new ValueType[m_size];

    int idx = 0;
    for (DataFrameIterator df_iterator = col_idx + df_begin; df_iterator < df_end; df_iterator += (m_size + 1))
    {
      // cout << "addr: " << df_iterator << " value: " << *df_iterator << " out index: " << col_idx
      //           << " in index: " << idx << "\n";
      m_d[idx] = *df_iterator;
      // cout << "index " << df_iterator << " " << df_iterator - df_begin << " add in cell\n";
      ++idx;
    }
  }

  ~ColumnSeries() { delete[] m_d; }

  ValueType& operator[](const int idx) { return *(m_d + idx); }

  friend ostream& operator<<(ostream& os, const ColumnSeries& col)
  {
    os << "ColumnSeries(size: " << col.m_size << ", "
       << "Items: \n";
    for (const ValueType& cell : col)
    {
      os << cell << "\n";
    }
    os << ")";
    return os;
  }

  constexpr size_t size() { return m_size; };

  Iterator begin() { return Iterator(m_d); }
  Iterator begin() const { return Iterator(m_d); }

  Iterator end() { return Iterator(m_d + m_size); }
  Iterator end() const { return Iterator(m_d + m_size); }

  ValueType& at(int idx) { return *(begin() + idx); }

  private:
  size_t m_size;
  // ValueType** m_d;
  ValueType* m_d;
};

#endif // COLUMN_SERIES_H
