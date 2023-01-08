#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include <array>
#include <stdlib.h>
#include <string>
#include <vector>

struct DataFrameIndex
{
  int raw_idx;
  int col_idx;

  std::string col_name;
  std::string row_name;
};

template<typename T, int d_size>
struct Series
{
  size_t size;
  T      d[d_size];
};

template<typename T, int df_size, int col_size, int row_size>
class DataFrame
{
  public:
  DataFrame(std::array<std::string, col_size> col_names, std::array<std::string, row_size> row_names) {}
  T&                   operator[](const int& col_idx, const int& raw_idx);
  T                    operator[](const int& col_idx, const int& raw_idx) const;
  Series<T&, col_size> operator[](const std::string& col);
  Series<T, col_size>  operator[](const int row_idx) const;

  ~DataFrame();

  const std::array<std::string, col_size>& cols() const { return m_col_names; }
  const std::array<std::string, row_size>& rows() const { return m_row_names; }

  DataFrameIndex       get_data_idx(int col_pos, int row_pos);
  Series<T&, col_size> get_col();
  Series<T&, row_size> get_raw();
  // std::itr       iter_rows();
  // std::itr       iter_cols();

  private:
  std::array<std::string, col_size> m_col_names;
  std::array<std::string, row_size> m_row_names;
  int                               m_rowSize;
  int                               m_colSize;
  int                               m_dataSize;
  T                                 m_data[df_size];
};

#endif // DATA_FRAME_H