#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <array>
#include <assert.h>
#include <iostream>
#include <map>
#include <stdlib.h>
#include <string>
#include <vector>

struct DataFrameIndex
{
  int row_idx;
  int col_idx;

  std::string col_name;
  std::string row_name;
};

template<typename T, int d_len>
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

  size_t size() { return sizeof(m_d); }

  size_t length() { return m_length; }

  private:
  DataFrameIndex m_idx;
  size_t         m_length = d_len;
  T              m_d[d_len];
};

template<typename T, int df_size, int col_len, int row_len>
class DataFrame
{
  public:
  DataFrame(std::array<std::string, col_len> col_names, std::array<std::string, row_len> row_names)
      : m_col_names(col_names),
        m_row_names(row_names)
  {

    for (int i = 0; i < col_len; i++)
    {
      m_row_idx_map.insert({m_col_names[i], i});
    }

    for (int i = 0; i < row_len; i++)
    {
      m_row_idx_map.insert({m_row_names[i], i});
    }
  }

  T& operator[](const int col_idx, const int row_idx)
  {
    assert(0 <= col_idx && col_idx <= (col_len - 1));
    assert(0 <= row_idx && row_idx <= (row_len - 1));

    std::cout << ((row_len * row_idx) + col_idx) << "\n";

    T& cell = *(begin() + (row_len * row_idx) + col_idx);
    return cell;
  }

  const T& operator[](const int& col_idx, const int& row_idx) const
  {
    assert(0 <= col_idx && col_idx <= (col_len - 1));
    assert(0 <= row_idx && row_idx <= (row_len - 1));

    const T& cell = *(begin() + ((sizeof(T) * row_len * row_idx)) + (sizeof(T) + col_idx));
    return cell;
  }

  // Series<T&, col_len> operator[](const std::string& col)
  // {
  //   Series<T, col_len> col_series;
  //   for (int i = 0; i < col_len; i++)
  //   {
  //     col_series[i] = *(begin() + (row_len * i));
  //   }
  //   return col_series;
  // }

  T& operator[](const int& idx) { return m_data[idx]; }

  // Series<T&, row_len> operator[](const int row_idx) {}

  ~DataFrame() {}

  const std::array<std::string, col_len>& cols() const { return m_col_names; }
  const std::array<std::string, row_len>& rows() const { return m_row_names; }

  void update_row(T data[row_len], int row_idx);
  void update_col(T data[col_len], int col_idx);

  int get_col_idx(std::string col) { return m_col_idx_map[col]; }
  int get_row_idx(std::string row) { return m_row_idx_map[row]; }

  T* begin()
  {
    T* begin_p = m_data;
    return begin_p;
  }

  T* end()
  {
    T* end_p = m_data + df_size - 1;
    return end_p;
  }

  Series<T&, col_len> get_col();
  Series<T&, row_len> get_raw();
  // std::itr       iter_rows();
  // std::itr       iter_cols();

  void print()
  {
    for (int i = 0; i < col_len; i++)
    {
      if (i == 0)
      {
        printf("%24s", m_col_names[i].c_str());
      }
      else if (i == col_len - 1)
      {
        printf("%15s\n\n", m_col_names[i].c_str());
      }
      else
      {
        printf("%15s", m_col_names[i].c_str());
      }
    }

    for (int i = 0; i < (sizeof(m_data) / sizeof(T)); i++)
    {
      if (i % row_len == 0)
      {
        printf("%3d %5s", ((i + 1) / row_len), m_row_names[i / row_len].c_str());
      }
      printf("%15d", m_data[i]);
      if (((i + 1) % row_len) == 0 && i != 0)
      {
        printf("\n");
      }
    }
    printf("\n");
  }

  private:
  std::array<std::string, col_len> m_col_names;
  std::array<std::string, row_len> m_row_names;
  std::map<std::string, int>       m_col_idx_map;
  std::map<std::string, int>       m_row_idx_map;
  int                              m_size_of_col;
  int                              m_size_of_row;
  int                              m_data_size;
  T                                m_data[col_len * row_len]{};
};

#endif // DATA_FRAME_H