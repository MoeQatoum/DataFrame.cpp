#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <array>
#include <assert.h>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <vector>

template<typename Iterable>
class Iterator
{
  using ValueType = typename Iterable::ValueType;

  public:
  Iterator(ValueType* data_p) : m_d(data_p) {}

  ValueType& operator*() const { return *m_d; }
  ValueType* operator->() { return m_d; }

  Iterator operator+(const std::size_t& off)
  {
    Iterator tmp(this->m_d);
    tmp.m_d = tmp.m_d + off;
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

  int operator-(const int& off)
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

struct Index
{
  std::size_t row_idx;
  std::size_t col_idx;
  std::size_t global_idx;

  std::string col_name;
  std::string row_name;

  friend std::ostream& operator<<(std::ostream& os, const Index& df_idx)
  {
    os << "Index(<" << df_idx.global_idx << ">, <" << df_idx.col_name << ", " << df_idx.col_idx << ">, <"
       << df_idx.row_name << ", " << df_idx.row_idx << ">)";
    return os;
  }
};

template<typename T>
struct Cell
{

  using ValueType = T;

  ValueType value;
  Index     idx;

  void operator=(const ValueType val) { value = val; }

  friend std::ostream& operator<<(std::ostream& os, const Cell<ValueType>& cell)
  {
    os << "Cell(value: " << cell.value << ", " << cell.idx << ")";
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
  using ValueType         = Cell<T>;
  using DataFrameIterator = Iterator<DataFrame<T>>;
  using size_t            = std::size_t;

  public:
  DataFrame(const std::vector<std::string>& col_names, const std::vector<std::string>& row_names)
  {
    m_col_size     = col_names.size();
    m_row_size     = row_names.size();
    m_col_count    = m_row_size;
    m_row_count    = m_col_size;
    m_current_size = m_col_size * m_row_size;
    m_d            = new ValueType[m_current_size];

    for (std::size_t i = 0; i < m_col_size; i++)
    {
      m_col_idx_map.insert({col_names[i], i});
    }

    for (std::size_t i = 0; i < m_row_size; i++)
    {
      m_row_idx_map.insert({row_names[i], i});
    }

    // 00 01 02 03 04
    // 05 06 07 08 09
    // 10 11 12 13 14
    // 15 16 17 18 19

    for (std::size_t i = 0; i < (m_col_size * m_row_size); i++)
    {
      m_d[i].idx.global_idx = i;

      m_d[i].idx.col_idx  = i % m_col_size;
      m_d[i].idx.col_name = col_names[i % m_col_size];

      m_d[i].idx.row_idx  = i / m_col_size;
      m_d[i].idx.row_name = row_names[i / m_col_size];
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

  // void update_row(RowSeries<DataFrame> data);

  // void update_col(ColumnSeries<DataFrame> data);

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
  constexpr size_t col_size() const { return m_row_size; }
  constexpr size_t row_size() const { return m_col_size; }

  constexpr std::pair<size_t, size_t> shape() const { return {m_col_count, m_row_count}; }

  RowSeries<T> get_raw(std::size_t row_idx)
  {
    DataFrameIterator row_bingen = begin() + (row_idx * row_size());
    DataFrameIterator row_end    = row_bingen + m_col_size;
    return RowSeries<T>(row_bingen, row_end);
  }

  ColumnSeries<T> get_col(size_t col_idx) { return ColumnSeries<T>{begin(), end(), col_idx, m_col_count}; }

  // ColumnIterator    iter_rows();
  // RowIterator       iter_cols();

  void print()
  {
    for (std::size_t i = 0; i < m_col_size; i++)
    {
      if (i == 0)
      {
        printf("%24s", get_col_name(i).value().c_str());
      }
      else if (i == m_col_size - 1)
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
      if (i % m_col_size == 0)
      {
        printf("%3lu %5s", ((i + 1) / m_col_size), get_row_name(i / m_col_size).value().c_str());
      }
      printf("%15d", m_d[i].value);
      if (((i + 1) % m_col_size) == 0 && i != 0)
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

template<typename T>
struct RowSeries
{
  using ValueType         = Cell<T>;
  using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
  using RowIterator       = Iterator<RowSeries>;

  RowSeries(DataFrameIterator row_begin, DataFrameIterator row_end)
  {
    m_size = row_end - row_begin;
    m_d    = new ValueType[m_size];

    for (std::size_t i = 0; i < m_size; i++)
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

  friend std::ostream& operator<<(std::ostream& os, const RowSeries& row)
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
  std::size_t m_size;
  ValueType*  m_d;
};

template<typename T>
struct ColumnSeries
{
  using ValueType         = Cell<T>;
  using DataFrameIterator = typename DataFrame<T>::DataFrameIterator;
  using ColumnIterator    = Iterator<ColumnSeries>;

  ColumnSeries(DataFrameIterator df_begin, DataFrameIterator df_end, std::size_t col_idx, std::size_t col_count)
  {
    m_d    = new ValueType[col_count];
    m_size = col_count;

    DataFrameIterator i = df_begin;
    for (std::size_t ii = 0; i < df_end; i += col_count)
    {
      m_d[ii] = *(i + col_idx);
      ++ii;
    }
  }

  ~ColumnSeries() { delete[] m_d; }

  ValueType& operator[](const int idx)
  {
    ValueType& item = *(m_d + idx);
    return item;
  }

  friend std::ostream& operator<<(std::ostream& os, const ColumnSeries& col)
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

  ColumnIterator begin() { return ColumnIterator(m_d); }
  ColumnIterator begin() const { return ColumnIterator(m_d); }

  ColumnIterator end() { return ColumnIterator(m_d + m_size); }
  ColumnIterator end() const { return ColumnIterator(m_d + m_size); }

  ValueType& at(int idx) { return *(begin() + idx); }

  private:
  std::size_t m_size;
  ValueType*  m_d;
};

#endif // DATA_FRAME_H
