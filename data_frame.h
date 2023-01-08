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

  constexpr size_t size() { return sizeof(m_d); }
  constexpr size_t length() { return d_len; }

  private:
  DataFrameIndex m_idx;
  T              m_d[d_len];
};

template<typename T, std::size_t df_size, std::size_t col_len, std::size_t row_len>
class StringIndexer;

template<typename T, std::size_t df_size, std::size_t col_len, std::size_t row_len>
class IntegerIndexer;

template<typename T, std::size_t df_size, std::size_t col_len, std::size_t row_len>
class DataFrame
{
  friend class StringIndexer<T, df_size, col_len, row_len>;
  friend class IntegerIndexer<T, df_size, col_len, row_len>;

  public:
  DataFrame(std::array<std::string, col_len> col_names, std::array<std::string, row_len> row_names)
      : loc(this),
        iloc(this),
        m_col_names(col_names),
        m_row_names(row_names)
  {

    for (std::size_t i = 0; i < col_len; i++)
    {
      m_col_idx_map.insert({m_col_names[i], i});
    }

    for (std::size_t i = 0; i < row_len; i++)
    {
      m_row_idx_map.insert({m_row_names[i], i});
    }
  }

  ~DataFrame() {}

  T& operator[](const int& idx) { return m_data[idx]; }

  // const T& operator[](const int& idx) const { return m_data[idx]; }

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

  StringIndexer<T, df_size, col_len, row_len>  loc;
  IntegerIndexer<T, df_size, col_len, row_len> iloc;

  Series<T&, col_len> get_col();
  Series<T&, row_len> get_raw();
  // std::itr       iter_rows();
  // std::itr       iter_cols();

  void print()
  {
    for (std::size_t i = 0; i < col_len; i++)
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

    for (std::size_t i = 0; i < (sizeof(m_data) / sizeof(T)); i++)
    {
      if (i % row_len == 0)
      {
        printf("%3lu %5s", ((i + 1) / row_len), m_row_names[i / row_len].c_str());
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
  std::array<std::string, col_len>   m_col_names;
  std::array<std::string, row_len>   m_row_names;
  std::map<std::string, std::size_t> m_col_idx_map;
  std::map<std::string, std::size_t> m_row_idx_map;
  std::size_t                        m_size_of_col;
  std::size_t                        m_size_of_row;
  std::size_t                        m_data_size;
  T                                  m_data[df_size]{};
};

template<typename T, std::size_t df_size, std::size_t col_len, std::size_t row_len>
class StringIndexer
{
  public:
  StringIndexer(DataFrame<T, df_size, col_len, row_len>* df) : m_df(df) {}

  T& operator[](const std::string& col_name, const std::string& row_name)
  {
    assert(m_df->m_col_idx_map.contains(col_name));
    assert(m_df->m_row_idx_map.contains(row_name));

    std::size_t col_idx = m_df->m_col_idx_map[col_name];
    std::cout << "col_idx: " << col_idx << "\n";
    std::size_t row_idx = m_df->m_row_idx_map[row_name];
    std::cout << "row_idx: " << row_idx << "\n";

    assert(0 <= col_idx && col_idx <= (col_len - 1));
    assert(0 <= row_idx && row_idx <= (row_len - 1));

    std::cout << m_df->begin() << "\n";
    std::cout << m_df->end() << "\n";

    T& cell = *(m_df->begin() + ((row_len * row_idx) + col_idx));
    return cell;
  }

  private:
  DataFrame<T, df_size, col_len, row_len>* m_df;
};

template<typename T, std::size_t df_size, std::size_t col_len, std::size_t row_len>
class IntegerIndexer
{
  public:
  IntegerIndexer(DataFrame<T, df_size, col_len, row_len>* df) : m_df(df) {}

  T& operator[](const std::size_t& col_idx, const std::size_t& row_idx)
  {
    assert(0 <= col_idx && col_idx <= (col_len - 1));
    assert(0 <= row_idx && row_idx <= (row_len - 1));

    T& cell = *(m_df->begin() + ((row_len * row_idx) + col_idx));
    return cell;
  }

  private:
  DataFrame<T, df_size, col_len, row_len>* m_df;
};

#endif // DATA_FRAME_H
