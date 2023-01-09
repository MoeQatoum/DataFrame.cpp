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
       << df_idx.row_name << ", " << df_idx.row_idx << ">)";
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

template<typename T, int N>
struct Series
{
  T& operator[](const int idx)
  {
    assert(0 <= idx && idx <= (d_len - 1));
    T& item = *(m_d + idx);
    return item;
  }

  T at(int idx)
  {
    assert(0 <= idx && idx <= (d_len - 1));
    return m_d[idx];
  }

  constexpr size_t size() { return N; }

  private:
  Index m_idx;
  T     m_d[N];
};

template<typename T, std::size_t... Args>
class StringIndexer;

template<typename T, std::size_t... Args>
class IntegralIndexer;

template<typename T, std::size_t N_col, std::size_t N_row>
class DataFrame
{
  friend class StringIndexer<T, N_col, N_row>;
  friend class IntegralIndexer<T, N_col, N_row>;

  public:
  DataFrame(std::array<std::string, N_col> col_names, std::array<std::string, N_row> row_names)
      : loc(this),
        iloc(this),
        m_col_names(col_names),
        m_row_names(row_names)
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

  ~DataFrame() {}

  Cell<T>& operator[](const int& idx) { return m_data[idx]; }

  std::array<std::string, N_col>& col_names() { return m_col_names; }
  std::array<std::string, N_row>& rows_names() { return m_row_names; }

  void update_row(Series<T, N_row> data, int row_idx);
  void update_col(Series<T, N_col> data, int col_idx);

  int get_col_idx(std::string col) { return m_col_idx_map[col]; }
  int get_row_idx(std::string row) { return m_row_idx_map[row]; }

  Cell<T>* begin()
  {
    Cell<T>* begin_p = m_data;
    return begin_p;
  }

  Cell<T>* end()
  {
    Cell<T>* end_p = (m_data + size()) - 1;
    return end_p;
  }

  constexpr std::size_t size() const { return N_col * N_row; };
  constexpr std::size_t col_size() const { return N_row; };
  constexpr std::size_t row_size() const { return N_col; };

  StringIndexer<T, N_col, N_row>   loc;
  IntegralIndexer<T, N_col, N_row> iloc;

  Series<T&, N_row> get_col();
  Series<T&, N_col> get_raw();
  // std::itr       iter_rows();
  // std::itr       iter_cols();

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

    for (std::size_t i = 0; i < (sizeof(m_data) / sizeof(Cell<T>)); i++)
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
  Cell<T>                            m_data[N_col * N_row];
};

template<typename T, std::size_t... Args>
class StringIndexer
{
  friend class DataFrame<T, Args...>;

  StringIndexer(DataFrame<T, Args...>* df) : m_df(df) {}

  DataFrame<T, Args...>* m_df;

  public:
  Cell<T>& operator[](const std::string& col_name, const std::string& row_name)
  {
    assert(m_df->m_col_idx_map.contains(col_name));
    assert(m_df->m_row_idx_map.contains(row_name));

    std::size_t col_idx = m_df->m_col_idx_map[col_name];
    std::size_t row_idx = m_df->m_row_idx_map[row_name];

    assert(0 <= col_idx && col_idx <= (col_len - 1));
    assert(0 <= row_idx && row_idx <= (row_len - 1));

    // Cell<T>& cell = *(m_df->begin() + ((m_df->row_size * row_idx) + col_idx));
    Cell<T>& cell = m_df->m_data[(m_df->row_size() * row_idx) + col_idx];

    return cell;
  }
};

template<typename T, std::size_t... Args>
class IntegralIndexer
{
  friend class DataFrame<T, Args...>;

  IntegralIndexer(DataFrame<T, Args...>* df) : m_df(df) {}

  DataFrame<T, Args...>* m_df;

  public:
  Cell<T>& operator[](const std::size_t& col_idx, const std::size_t& row_idx)
  {
    assert(0 <= col_idx && col_idx <= (col_len - 1));
    assert(0 <= row_idx && row_idx <= (row_len - 1));

    Cell<T>& cell = *(m_df->begin() + ((m_df->row_size() * row_idx) + col_idx));
    // Cell<T>& cell = m_df->m_data[(m_df->row_size() * row_idx) + col_idx];
    return cell;
  }
};

#endif // DATA_FRAME_H
