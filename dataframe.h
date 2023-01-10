#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <array>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include <vector>

struct Index
{
  std::size_t row_idx;
  std::size_t col_idx;
  std::size_t global_idx;

  std::string col_name;
  std::string row_name;

  friend std::ostream& operator<<(std::ostream& os, const Index& df_idx)
  {
    os << "DatFrameIndex(<" << df_idx.global_idx << ">, <" << df_idx.col_name << ", " << df_idx.col_idx << ">, <"
       << df_idx.row_name << ", " << df_idx.row_idx << ">)\n";
    return os;
  }
};

template<typename T>
struct Cell
{
  T     value;
  Index idx;

  void operator=(const T val) { value = val; }

  friend std::ostream& operator<<(std::ostream& os, const Cell<T>& cell)
  {
    os << cell.value;
    return os;
  }
};

template<typename T, std::size_t N>
struct RowSeries
{
  T& operator[](const int idx)
  {
    assert(0 <= idx && idx <= (size() - 1));
    T& item = *(m_d + idx);
    return item;
  }

  T at(int idx)
  {
    assert(0 <= idx && idx <= (size() - 1));
    return m_d[idx];
  }

  constexpr std::size_t size() { return N; }

  private:
  std::array<std::string, N> col_names;
  Cell<T>                    m_d[N];
};

template<typename T, std::size_t N>
struct ColumnSeries
{
  T& operator[](const int idx)
  {
    assert(0 <= idx && idx <= (size() - 1));
    T& item = *(m_d + idx);
    return item;
  }

  T at(int idx)
  {
    assert(0 <= idx && idx <= (size() - 1));
    return m_d[idx];
  }

  constexpr std::size_t size() { return N; }

  private:
  std::array<std::string, N> row_names;
  Cell<T>                    m_d[N];
};

template<typename DataFrame>
class df_Iterator;

template<typename T, std::size_t N_col, std::size_t N_row>
class DataFrame
{
  public:
  using ValueType = T;
  using Iterator  = df_Iterator<DataFrame>;

  public:
  DataFrame(const std::array<std::string, N_col>& col_names, const std::array<std::string, N_row>& row_names)
      : m_col_names(col_names),
        m_row_names(row_names),
        m_data(new Cell<T>[N_col * N_row])
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

  Cell<T>& operator[](const std::size_t& idx) { return *(m_data + idx); }

  const Cell<T>& operator[](const std::size_t& idx) const { return *(m_data + idx); }

  Cell<T>& operator[](const std::size_t& col_idx, const std::size_t& row_idx)
  {
    assert(0 <= col_idx && col_idx <= (m_df->col_size() - 1));
    assert(0 <= row_idx && row_idx <= (m_df->row_size() - 1));

    Cell<T>& cell = *(m_data + ((row_size() * row_idx) + col_idx));
    return cell;
  }

  Cell<ValueType>& operator[](const std::string& col_name, const std::string& row_name)
  {
    assert(m_df->m_col_idx_map.contains(col_name));
    assert(m_df->m_row_idx_map.contains(row_name));

    std::size_t col_idx = m_col_idx_map[col_name];
    std::size_t row_idx = m_row_idx_map[row_name];

    assert(0 <= col_idx && col_idx <= (m_df->col_size() - 1));
    assert(0 <= row_idx && row_idx <= (m_df->row_size() - 1));

    Cell<T>& cell = *(m_data + ((row_size() * row_idx) + col_idx));
    return cell;
  }

  std::array<std::string, N_col>& col_names() { return m_col_names; }
  std::array<std::string, N_row>& rows_names() { return m_row_names; }

  void update_row(RowSeries<T, N_row> data);

  void update_col(ColumnSeries<T, N_col> data);

  int get_col_idx(std::string col) { return m_col_idx_map[col]; }

  int get_row_idx(std::string row) { return m_row_idx_map[row]; }

  Cell<T>& at(std::size_t idx) { return m_data[idx]; }

  const Cell<T>& at(std::size_t idx) const { return m_data[idx]; }

  Iterator begin() { return Iterator(m_data); }

  Iterator end() { return Iterator(m_data + size()); }

  constexpr std::size_t size() const { return N_col * N_row; }
  constexpr std::size_t col_size() const { return N_row; }
  constexpr std::size_t row_size() const { return N_col; }

  ColumnSeries<Cell<T>, N_row> get_col();
  RowSeries<Cell<T>, N_col>    get_raw();

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
  Cell<T>*                           m_data;
};

template<typename DataFrame>
class df_Iterator
{
  using T = typename DataFrame::ValueType;

  public:
  df_Iterator(Cell<T>* data_p) : m_d(data_p) {}

  Cell<T>& operator*() const { return *m_d; }
  Cell<T>* operator->() { return m_d; }

  df_Iterator& operator++()
  {
    m_d++;
    return *this;
  }

  df_Iterator operator++(int)
  {
    df_Iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  df_Iterator& operator--()
  {
    m_d--;
    return *this;
  }

  df_Iterator operator--(int)
  {
    df_Iterator tmp = *this;
    --(*this);
    return tmp;
  }

  friend bool operator==(const df_Iterator& a, const df_Iterator& b) { return a.m_d == b.m_d; };
  friend bool operator!=(const df_Iterator& a, const df_Iterator& b) { return a.m_d != b.m_d; };

  private:
  Cell<T>* m_d;
};

#endif // DATA_FRAME_H
