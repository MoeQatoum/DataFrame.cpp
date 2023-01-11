#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <array>
#include <assert.h>
#include <iostream>
#include <map>
#include <string>

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

  int operator-(const int& off)
  {
    Iterator<Iterable> tmp(this->m_d);
    tmp.m_d = tmp.m_d - off;
    return tmp;
  }

  friend int operator-(const Iterator& l, const Iterator& r) { return l.m_d - r.m_d; }

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

  bool operator==(const Iterator& other) { return other.m_d == this->m_d; };
  bool operator!=(const Iterator& other) { return !(other == *this); };

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

template<typename DataFrame>
struct RowSeries
{
  using ValueType         = typename DataFrame::ValueType;
  using DataFrameIterator = typename DataFrame::DataFrameIterator;
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

  ValueType& operator[](const int idx)
  {
    ValueType& item = *(m_d + idx);
    return item;
  }

  friend std::ostream& operator<<(std::ostream& os, const RowSeries& row)
  {
    os << "RowSeries(size: " << row.m_size << ", "
       << "Items: \n";
    for (const auto& cell : row)
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

/*
template<typename DataFrame>
struct ColumnSeries
{
 using ValueType         = typename DataFrame::ValueType;
 using DataFrameIterator = typename DataFrame::DataFrameIterator;
 using ColumnIterator    = Iterator<ColumnSeries>;

   ColumnSeries(DataFrameIterator df_begin, DataFrameIterator df_end)
   {
     m_size = df_end - df_begin;
     m_d    = new ValueType[m_size];

     for (std::size_t i = 0; i < m_size; i++)
     {
       m_d[i] = *(df_begin + i);
     }
   }

   ValueType& operator[](const int idx)
   {
     ValueType& item = *(m_d + idx);
     return item;
   }

   friend std::ostream& operator<<(std::ostream& os, const ColumnSeries& row)
   {
     os << "ColumnSeries(size: " << row.m_size << ", "
        << "Items: \n";
     for (const auto& cell : row)
     {
       os << cell << "\n";
     }
     os << ")";
     return os;
   }

ColumnIterator begin()
{
  return RowIterator(m_d);
}
ColumnIterator begin() const
{
  return RowIterator(m_d);
}

ColumnIterator end()
{
  return RowIterator(m_d + m_size);
}
ColumnIterator end() const
{
  return RowIterator(m_d + m_size);
}

ValueType& at(int idx)
{
  return *(begin() + idx);
}

private:
std::size_t m_size;
ValueType*  m_d;
}
;
*/
template<typename T, std::size_t N_col, std::size_t N_row>
class DataFrame
{
  public:
  using ValueType         = Cell<T>;
  using DataFrameIterator = Iterator<DataFrame>;

  public:
  DataFrame(const std::array<std::string, N_col>& col_names, const std::array<std::string, N_row>& row_names)
      : m_col_names(col_names),
        m_row_names(row_names),
        m_data(new ValueType[N_col * N_row])
  {
    for (std::size_t i = 0; i < N_col; i++)
    {
      m_col_idx_map.insert({m_col_names[i], i});
    }

    for (std::size_t i = 0; i < N_row; i++)
    {
      m_row_idx_map.insert({m_row_names[i], i});
    }

    // 0  1  2  3  4
    // 5  6  7  8  9
    // 10 11 12 13 14
    // 15 16 17 18 19

    for (std::size_t i = 0; i < (N_col * N_row); i++)
    {
      m_data[i].idx.global_idx = i;

      m_data[i].idx.col_idx  = i % N_col;
      m_data[i].idx.col_name = col_names[i % N_col];

      m_data[i].idx.row_idx  = i / N_col;
      m_data[i].idx.row_name = row_names[i / N_col];
    }
  }

  ~DataFrame() { delete[] m_data; }

  ValueType& operator[](const std::size_t& idx) { return *(m_data + idx); }

  const ValueType& operator[](const std::size_t& idx) const { return *(m_data + idx); }

  ValueType& operator[](const std::size_t& col_idx, const std::size_t& row_idx)
  {
    assert(0 <= col_idx && col_idx <= (col_size() - 1));
    assert(0 <= row_idx && row_idx <= (row_size() - 1));

    ValueType& cell = *(m_data + ((row_size() * row_idx) + col_idx));
    return cell;
  }

  ValueType& operator[](const std::string& col_name, const std::string& row_name)
  {
    assert(m_col_idx_map.contains(col_name));
    assert(m_row_idx_map.contains(row_name));

    std::size_t col_idx = m_col_idx_map[col_name];
    std::size_t row_idx = m_row_idx_map[row_name];

    assert(0 <= col_idx && col_idx <= (col_size() - 1));
    assert(0 <= row_idx && row_idx <= (row_size() - 1));

    ValueType& cell = *(m_data + ((row_size() * row_idx) + col_idx));
    return cell;
  }

  std::array<std::string, N_col>& col_names() { return m_col_names; }
  std::array<std::string, N_row>& rows_names() { return m_row_names; }

  // void update_row(RowSeries<DataFrame> data);

  // void update_col(ColumnSeries<DataFrame> data);

  int get_col_idx(std::string col) { return m_col_idx_map[col]; }

  int get_row_idx(std::string row) { return m_row_idx_map[row]; }

  ValueType& at(std::size_t idx) { return m_data[idx]; }

  const ValueType& at(std::size_t idx) const { return m_data[idx]; }

  DataFrameIterator begin() { return DataFrameIterator(m_data); }

  DataFrameIterator end() { return DataFrameIterator(m_data + size()); }

  constexpr std::size_t size() const { return N_col * N_row; }
  constexpr std::size_t col_size() const { return N_row; }
  constexpr std::size_t row_size() const { return N_col; }

  RowSeries<DataFrame> get_raw(std::size_t row_idx)
  {
    Iterator row_bingen = begin() + (row_idx * row_size());
    Iterator row_end    = row_bingen + N_col;
    return RowSeries<DataFrame>(row_bingen, row_end);
  }

  // ColumnIterator    iter_rows();
  // RowIterator       iter_cols();

  void print()
  {
    for (std::size_t i = 0; i < N_col; i++)
    {
      if (i == 0)
      {
        printf("%24s", m_col_names[i].c_str());
      }
      else if (i == N_col - 1)
      {
        printf("%15s\n\n", m_col_names[i].c_str());
      }
      else
      {
        printf("%15s", m_col_names[i].c_str());
      }
    }

    for (std::size_t i = 0; i < size(); i++)
    {
      if (i % N_col == 0)
      {
        printf("%3lu %5s", ((i + 1) / N_col), m_row_names[i / N_col].c_str());
      }
      printf("%15d", m_data[i].value);
      if (((i + 1) % N_col) == 0 && i != 0)
      {
        printf("\n");
      }
    }
    printf("\n");
  }

  private:
  std::array<std::string, N_col>     m_col_names;
  std::array<std::string, N_row>     m_row_names;
  std::map<std::string, std::size_t> m_col_idx_map;
  std::map<std::string, std::size_t> m_row_idx_map;
  ValueType*                         m_data;
};

#endif // DATA_FRAME_H
