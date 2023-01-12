#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <array>
#include <assert.h>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include <cell.h>
#include <column_series.h>
#include <row_series.h>

template<typename T>
class DataFrame;

template<typename Iterable>
class Iterator
{
  using ValueType = typename Iterable::ValueType;

  template<typename>
  friend class DataFrameRowIterator;

  template<typename>
  friend class DataFrameColIterator;

  public:
  Iterator() = default;

  Iterator(ValueType* data_p) : m_d(data_p) {}

  Iterator(Iterator& other) : m_d(other.m_d) {}

  Iterator(Iterator&& other) : m_d(other.m_d) {}

  ValueType* operator&() const { return m_d; }

  ValueType& operator*() const { return *m_d; }

  ValueType* operator->() { return m_d; }

  Iterator& operator=(Iterator&& other)
  {
    // m_d = other.m_d;
    // return *this;
    return Iterator{other.m_d};
  }

  Iterator& operator=(Iterator& other)
  {
    // m_d = other.m_d;
    // return *this;
    return Iterator{other.m_d};
  }

  // Iterator operator+(const std::size_t& off)
  // {
  //   Iterator tmp(this->m_d);
  //   tmp.m_d = tmp.m_d + off;
  //   return tmp;
  // }

  // ValueType* operator+(const std::size_t& off)
  // {
  //   std::cout << "add add\n";

  // return m_d + off;
  // }

  friend const Iterator operator+(const std::size_t off, Iterator iterator)
  {
    Iterator tmp{iterator.m_d};
    tmp += off;
    return tmp;
  }

  friend const Iterator operator+(const Iterator iterator, std::size_t off)
  {
    Iterator tmp{iterator.m_d};
    tmp += off;
    return tmp;
  }

  Iterator& operator++()
  {
    m_d++;
    return *this;
  }

  Iterator operator++(int)
  {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  Iterator& operator+=(int off)
  {
    m_d = m_d + off;
    return *this;
  }

  Iterator operator-(const int& off)
  {
    Iterator<Iterable> tmp(this->m_d);
    tmp.m_d = tmp.m_d - off;
    return tmp;
  }

  friend int operator-(const Iterator& lhs, const Iterator& rhs) { return lhs.m_d - rhs.m_d; }

  Iterator& operator--()
  {
    m_d--;
    return *this;
  }

  Iterator operator--(int)
  {
    Iterator tmp = *this;
    --(*this);
    return tmp;
  }

  bool operator==(const Iterator& other) const { return other.m_d == this->m_d; };
  bool operator!=(const Iterator& other) const { return !(other == *this); };

  friend bool operator<(const Iterator& lhs, const Iterator& rhs) { return lhs.m_d < rhs.m_d; }
  friend bool operator>(const Iterator& lhs, const Iterator& rhs) { return lhs.m_d > rhs.m_d; }

  friend std::ostream& operator<<(std::ostream& os, const Iterator& itr)
  {
    os << itr.m_d;
    return os;
  }

  private:
  ValueType* m_d;
};

template<typename Iterable>
class DataFrameRowIterator
{
  using DataFrameIterator = typename Iterable::DataFrameIterator;
  using RowSeries         = typename Iterable::RowSeries;

  public:
  DataFrameRowIterator(DataFrameIterator df_begin, std::size_t row_size) : m_df_iterator(df_begin), m_row_size(row_size)
  {
  }
  RowSeries row() { return RowSeries(m_df_iterator, m_df_iterator + m_row_size); }

  DataFrameRowIterator operator+(const std::size_t& off)
  {
    DataFrameRowIterator tmp(this->m_df_iterator);
    tmp.m_df_iterator = tmp.m_df_iterator + off;
    return tmp;
  }

  DataFrameRowIterator& operator++()
  {
    m_df_iterator += m_row_size;
    return *this;
  }

  DataFrameRowIterator operator++(int)
  {
    DataFrameRowIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator<(const DataFrameRowIterator& lhs, const DataFrameIterator& rhs)
  {
    // std::cout << lhs.m_df_iterator << "      " << rhs << "     " << (lhs.m_df_iterator < rhs) << "\n";
    return lhs.m_df_iterator < rhs;
  }

  friend bool operator<(const DataFrameIterator& lhs, const DataFrameRowIterator& rhs)
  {
    // std::cout << lhs << "      " << rhs.m_df_iterator << "     " << (lhs < rhs.m_df_iterator) << "\n";
    return lhs < rhs.m_df_iterator;
  }

  friend bool operator>(const DataFrameRowIterator& lhs, const DataFrameIterator& rhs)
  {
    // std::cout << lhs.m_df_iterator << "      " << rhs << "     " << (lhs.m_df_iterator > rhs) << "\n";
    return lhs.m_df_iterator > rhs;
  }

  friend bool operator>(const DataFrameIterator& lhs, const DataFrameRowIterator& rhs)
  {
    // std::cout << lhs << "      " << rhs.m_df_iterator << "     " << (lhs > rhs.m_df_iterator) << "\n";
    return lhs > rhs.m_df_iterator;
  }

  private:
  DataFrameIterator m_df_iterator;
  std::size_t       m_row_size;
};

template<typename Iterable>
class DataFrameColIterator
{
  using ValueType         = typename Iterable::ValueType;
  using DataFrameIterator = typename Iterable::DataFrameIterator;
  using ColumnSeries      = typename Iterable::ColumnSeries;
  using ColIterator       = typename ColumnSeries::Iterator;

  public:
  DataFrameColIterator(DataFrameIterator df_begin, DataFrameIterator df_end, std::size_t row_size)
      : m_df_begin(df_begin),
        m_df_end(df_end),
        m_current_col_end(df_end - row_size),
        m_current_col_idx(0),
        m_row_size(row_size)
  {
  }
  ColumnSeries column() { return ColumnSeries(m_df_begin, m_df_end, m_current_col_idx, m_row_size); }

  DataFrameColIterator operator+(const std::size_t& off)
  {
    DataFrameColIterator tmp(this->m_df_begin, this->m_df_end, m_row_size);
    tmp.m_current_col_idx = m_current_col_idx;
    tmp.m_current_col_end = m_current_col_end;

    m_current_col_idx = m_current_col_idx + off;

    return tmp;
  }

  DataFrameColIterator& operator++()
  {
    ++m_current_col_idx;
    ++m_current_col_end;
    return *this;
  }

  DataFrameColIterator operator++(int)
  {
    DataFrameColIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  friend bool operator<(const DataFrameColIterator& lhs, const DataFrameIterator& rhs)
  {
    // std::cout << lhs.m_current_col_end.m_d << "      " << rhs.m_d << "     " << (lhs.m_current_col_end.m_d < rhs.m_d)
    // << "\n";
    return lhs.m_current_col_end.m_d < rhs.m_d;
  }

  friend bool operator<(const DataFrameIterator& lhs, const DataFrameColIterator& rhs)
  {
    // std::cout << lhs << "      " << rhs.m_current_col_end << "     " << (lhs.m_d < rhs.m_current_col_end.m_d) <<
    // "\n";
    return lhs.m_d < rhs.m_current_col_end.m_d;
  }

  friend bool operator>(const DataFrameColIterator& lhs, const DataFrameIterator& rhs)
  {
    // std::cout << lhs.m_current_col_end << "      " << rhs << "     " << (lhs.m_current_col_end > rhs) << "\n";
    return lhs.m_current_col_end > rhs;
  }

  friend bool operator>(const DataFrameIterator& lhs, const DataFrameColIterator& rhs)
  {
    // std::cout << lhs << "      " << rhs.m_current_col_end << "     " << (lhs > rhs.m_current_col_end) << "\n";
    return lhs > rhs.m_current_col_end;
  }

  private:
  DataFrameIterator m_df_begin;
  DataFrameIterator m_df_end;
  DataFrameIterator m_current_col_end;
  std::size_t       m_current_col_idx;
  std::size_t       m_row_size;
};

struct Shape
{
  size_t col_count;
  size_t row_count;

  friend std::ostream& operator<<(std::ostream& os, const Shape& shape)
  {
    os << "Shape(c: " << shape.col_count << ", r: " << shape.row_count << ")";
    return os;
  }
};

template<typename T>
struct ColumnSeries;

template<typename T>
struct RowSeries;

template<typename T>
class DataFrame
{
  public:
  using ValueType            = Cell<T>;
  using RowSeries            = RowSeries<T>;
  using ColumnSeries         = ColumnSeries<T>;
  using DataFrameIterator    = Iterator<DataFrame<T>>;
  using DataFrameRowIterator = DataFrameRowIterator<DataFrame<T>>;
  using DataFrameColIterator = DataFrameColIterator<DataFrame<T>>;
  using size_t               = std::size_t;

  public:
  DataFrame(const std::vector<std::string>& col_names, const std::vector<std::string>& row_names)
  {
    m_col_count    = col_names.size();
    m_row_count    = row_names.size();
    m_col_size     = m_row_count;
    m_row_size     = m_col_count;
    m_current_size = m_col_count * m_row_count;
    m_d            = new ValueType[m_current_size];

    for (std::size_t i = 0; i < m_col_count; i++)
    {
      m_col_idx_map.insert({col_names[i], i});
    }

    for (std::size_t i = 0; i < m_row_count; i++)
    {
      m_row_idx_map.insert({row_names[i], i});
    }

    // 00 01 02 03 04
    // 05 06 07 08 09
    // 10 11 12 13 14
    // 15 16 17 18 19

    for (std::size_t i = 0; i < m_current_size; i++)
    {
      m_d[i].idx.global_idx = i;

      m_d[i].idx.col_idx  = i % m_col_count;
      m_d[i].idx.col_name = col_names[i % m_col_count];

      // std::cout << i % m_col_count << " " << col_names[i % m_col_count] << "\n";

      m_d[i].idx.row_idx  = i / m_col_count;
      m_d[i].idx.row_name = row_names[i / m_col_count];
    }
  }

  ~DataFrame() { delete[] m_d; }

  ValueType& operator[](const std::size_t& idx) { return *(m_d + idx); }

  const ValueType& operator[](const std::size_t& idx) const { return *(m_d + idx); }

  ValueType& operator[](const std::size_t& col_idx, const std::size_t& row_idx)
  {
    assert(0 <= col_idx && col_idx <= (m_col_count - 1));
    assert(0 <= row_idx && row_idx <= (m_row_count - 1));

    ValueType& cell = *(m_d + ((m_row_size * row_idx) + col_idx));
    return cell;
  }

  ValueType& operator[](const std::string& col_name, const std::string& row_name)
  {
    assert(m_col_idx_map.contains(col_name));
    assert(m_row_idx_map.contains(row_name));

    std::size_t col_idx = m_col_idx_map[col_name];
    std::size_t row_idx = m_row_idx_map[row_name];

    assert(0 <= col_idx && col_idx <= (m_col_count - 1));
    assert(0 <= row_idx && row_idx <= (m_row_count - 1));

    ValueType& cell = *(m_d + ((m_row_size * row_idx) + col_idx));
    return cell;
  }

  std::size_t get_col_idx(std::string col) { return m_col_idx_map[col]; }

  std::optional<std::string> get_col_name(std::size_t col_idx)
  {
    for (const auto& [col_name, idx] : m_col_idx_map)
    {
      if (idx == col_idx)
      {
        return col_name;
      }
    }
    return std::nullopt;
  }

  std::size_t get_row_idx(std::string row) { return m_row_idx_map[row]; }

  std::optional<std::string> get_row_name(std::size_t row_idx)
  {
    for (const auto& [row_name, idx] : m_row_idx_map)
    {
      if (idx == row_idx)
      {
        return row_name;
      }
    }
    return std::nullopt;
  }

  ValueType& at(std::size_t idx) { return m_d[idx]; }

  const ValueType& at(std::size_t idx) const { return m_d[idx]; }

  DataFrameIterator begin() { return DataFrameIterator(m_d); }

  DataFrameIterator end() { return DataFrameIterator(m_d + size()); }

  constexpr size_t size() const { return m_current_size; }
  constexpr size_t col_size() const { return m_col_size; }
  constexpr size_t row_size() const { return m_row_size; }

  constexpr Shape shape() const { return {m_col_count, m_row_count}; }

  RowSeries get_row(std::size_t row_idx)
  {
    DataFrameIterator row_bingen = begin() + (row_idx * m_row_size);
    DataFrameIterator row_end    = row_bingen + m_row_size;
    return RowSeries(row_bingen, row_end);
  }

  ColumnSeries get_col(size_t col_idx) { return ColumnSeries{begin(), end(), col_idx, m_row_size}; }

  DataFrameRowIterator iter_rows() { return DataFrameRowIterator(begin(), m_row_size); }
  DataFrameColIterator iter_cols() { return DataFrameColIterator(begin(), end(), m_row_size); }

  void print()
  {
    for (std::size_t i = 0; i < m_col_count; i++)
    {
      if (i == 0)
      {
        printf("%24s", get_col_name(i).value().c_str());
      }
      else if (i == m_col_count - 1)
      {
        printf("%15s\n\n", get_col_name(i).value().c_str());
      }
      else
      {
        printf("%15s", get_col_name(i).value().c_str());
      }
    }

    for (std::size_t i = 0; i < size(); i++)
    {
      if (i % m_col_count == 0)
      {
        printf("%3lu %5s", ((i + 1) / m_col_count), get_row_name(i / m_col_count).value().c_str());
      }
      printf("%15d", m_d[i].value);
      if (((i + 1) % m_col_count) == 0 && i != 0)
      {
        printf("\n");
      }
    }
    printf("\n");
  }

  private:
  std::map<std::string, std::size_t> m_col_idx_map;
  std::map<std::string, std::size_t> m_row_idx_map;
  size_t                             m_current_size;
  size_t                             m_col_size;
  size_t                             m_col_count;
  size_t                             m_row_size;
  size_t                             m_row_count;
  ValueType*                         m_d;
};

#endif // DATA_FRAME_H
