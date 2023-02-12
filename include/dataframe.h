#ifndef DATA_FRAME_H
#define DATA_FRAME_H

#include "common.h"

#include "cell.h"
#include "column_series.h"
#include "row_series.h"
#include "utils.h"

namespace df {

  template<NumericalTypes T>
  class DataFrame;

  template<typename Iterable>
  class Iterator {
    using ValueType = typename Iterable::ValueType;

    using size_t = df_ui32;

    template<typename>
    friend class DataFrameRowIterator;

    template<typename>
    friend class DataFrameColIterator;

public:
    Iterator(ValueType* data_p) : m_d(data_p) {
    }

    Iterator(const Iterator& other) : m_d(other.m_d) {
    }

    ~Iterator() {
    }

    Iterator& operator=(Iterator&& other) {
      return Iterator{other.m_d};
    }

    Iterator& operator=(const Iterator& other) {
      return Iterator{other.m_d};
    }

    ValueType* operator&() const {
      return m_d;
    }

    ValueType& operator*() const {
      return *m_d;
    }

    ValueType* operator->() {
      return m_d;
    }

    friend const Iterator operator+(const size_t& lhs, const Iterator& rhs) {
      Iterator tmp{rhs};
      tmp += lhs;
      return tmp;
    }

    friend const Iterator operator+(const Iterator& lhs, const size_t& rhs) {
      Iterator tmp{lhs};
      tmp += rhs;
      return tmp;
    }

    Iterator& operator++() {
      m_d++;
      return *this;
    }

    Iterator operator++(int) {
      Iterator tmp{*this};
      ++(*this);
      return tmp;
    }

    void operator+=(int off) {
      m_d += off;
    }

    friend int operator-(const Iterator& lhs, const Iterator& rhs) {
      return lhs.m_d - rhs.m_d;
    }

    friend Iterator operator-(const Iterator& lhs, const size_t& rhs) {
      Iterator tmp{lhs};
      tmp -= rhs;
      return tmp;
    }

    Iterator& operator--() {
      m_d--;
      return *this;
    }

    Iterator operator--(int) {
      Iterator tmp = *this;
      --(*this);
      return tmp;
    }

    void operator-=(int off) {
      m_d -= off;
    }

    bool operator==(const Iterator& other) const {
      return other.m_d == this->m_d;
    }

    bool operator!=(const Iterator& other) const {
      return !(other.m_d == this->m_d);
    }

    friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
      return lhs.m_d < rhs.m_d;
    }

    friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
      return lhs.m_d > rhs.m_d;
    }

    friend std::ostream& operator<<(std::ostream& os, const Iterator& itr) {
      os << itr.m_d;
      return os;
    }

private:
    ValueType* m_d;
  };

  template<typename Iterable>
  class DataFrameRowIterator {
    using DataFrameIterator = typename Iterable::DataFrameIterator;
    using RowSeries         = typename Iterable::RowSeries;

    using size_t = df_ui32;

public:
    DataFrameRowIterator(DataFrameIterator df_begin, size_t row_size)
        : m_df_begin(df_begin),
          m_row_size(row_size),
          m_current_row_idx(0) {
    }

    DataFrameRowIterator(const DataFrameRowIterator& other)
        : m_df_begin(other.m_df_begin),
          m_row_size(other.m_row_size),
          m_current_row_idx(other.m_current_row_idx) {
    }

    ~DataFrameRowIterator() {
    }

    RowSeries current_row() {
      return RowSeries(m_df_begin, m_current_row_idx, m_row_size);
    }

    size_t current_row_idx() {
      return m_current_row_idx;
    }

    DataFrameRowIterator& operator++() {
      ++m_current_row_idx;
      return *this;
    }

    DataFrameRowIterator operator++(int) {
      DataFrameRowIterator tmp{*this};
      ++(*this);
      return tmp;
    }

    DataFrameRowIterator& operator--() {
      --m_current_row_idx;
      return *this;
    }

    DataFrameRowIterator operator--(int) {
      DataFrameRowIterator tmp{*this};
      --(*this);
      return tmp;
    }

    friend bool operator<(const DataFrameRowIterator& lhs, const DataFrameIterator& rhs) {
      return (lhs.m_df_begin + (lhs.m_current_row_idx * lhs.m_row_size)) < rhs;
    }

    friend bool operator<(const DataFrameIterator& lhs, const DataFrameRowIterator& rhs) {
      return lhs < (rhs.m_df_begin + (rhs.m_current_row_idx * rhs.m_row_size));
    }

    friend bool operator>(const DataFrameRowIterator& lhs, const DataFrameIterator& rhs) {
      return (lhs.m_df_begin + (lhs.m_current_row_idx * lhs.m_row_size)) > rhs;
    }

    friend bool operator>(const DataFrameIterator& lhs, const DataFrameRowIterator& rhs) {
      return lhs > (rhs.m_df_begin + (rhs.m_current_row_idx * rhs.m_row_size));
    }

    friend bool operator<(const DataFrameRowIterator& lhs, const size_t& rhs) {
      return lhs.m_current_row_idx < rhs;
    }

    friend bool operator<(const size_t& lhs, const DataFrameRowIterator& rhs) {
      return lhs < rhs.m_current_row_idx;
    }

    friend bool operator>(const DataFrameRowIterator& lhs, const size_t& rhs) {
      return lhs.m_current_row_idx > rhs;
    }

    friend bool operator>(const size_t& lhs, const DataFrameRowIterator& rhs) {
      return lhs > rhs.m_current_row_idx;
    }

private:
    DataFrameIterator m_df_begin;
    size_t            m_row_size;
    size_t            m_current_row_idx;
  };

  template<typename Iterable>
  class DataFrameColIterator {
    using ValueType     = typename Iterable::ValueType;
    using DataFrameIter = typename Iterable::DataFrameIterator;
    using ColumnSeries  = typename Iterable::ColumnSeries;
    using ColIterator   = typename ColumnSeries::Iterator;

    using size_t = df_ui32;

public:
    DataFrameColIterator(DataFrameIter df_begin, size_t col_size, size_t row_size)
        : m_df_begin(df_begin),
          m_col_size(col_size),
          m_row_size(row_size),
          m_current_col_idx(0) {
    }

    DataFrameColIterator(const DataFrameColIterator& other)
        : m_df_begin(other.m_df_begin),
          m_col_size(other.m_col_size),
          m_row_size(other.m_row_size),
          m_current_col_idx(other.m_current_col_idx) {
    }

    ~DataFrameColIterator() {
    }

    ColumnSeries current_col() {
      return ColumnSeries(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
    }

    size_t current_col_idx() {
      return m_current_col_idx;
    }

    DataFrameColIterator operator+(const size_t& off) {
      DataFrameColIterator tmp{*this};
      tmp += off;
      std::cout << "==";
      return tmp;
    }

    DataFrameColIterator& operator++() {
      ++m_current_col_idx;
      return *this;
    }

    DataFrameColIterator operator++(int) {
      DataFrameColIterator tmp{*this};
      ++(*this);
      return tmp;
    }

    void operator+=(int off) {
      m_current_col_idx += off;
    }

    friend bool operator<(const DataFrameColIterator& lhs, const DataFrameIter& rhs) {
      return (lhs.m_df_begin + (lhs.m_current_col_idx + (lhs.m_row_size * (lhs.m_col_size - 1)))) < rhs.m_d;
    }

    friend bool operator<(const DataFrameIter& lhs, const DataFrameColIterator& rhs) {
      return lhs.m_d < (rhs.m_df_begin + (rhs.m_current_col_idx + (rhs.m_row_size * rhs.m_col_size)));
    }

    friend bool operator>(const DataFrameColIterator& lhs, const DataFrameIter& rhs) {
      return (lhs.m_df_begin + (lhs.m_current_col_idx + (lhs.m_row_size * lhs.m_col_size))) > rhs.m_d;
    }

    friend bool operator>(const DataFrameIter& lhs, const DataFrameColIterator& rhs) {
      return lhs.m_d > (rhs.m_df_begin + (rhs.m_current_col_idx + (rhs.m_row_size * rhs.m_col_size)));
    }

    friend bool operator<(const DataFrameColIterator& lhs, const size_t& rhs) {
      return lhs.m_current_col_idx < rhs;
    }

    friend bool operator<(const size_t& lhs, const DataFrameColIterator& rhs) {
      return lhs < rhs.m_current_col_idx;
    }

    friend bool operator>(const DataFrameColIterator& lhs, const size_t& rhs) {
      return lhs.m_current_col_idx > rhs;
    }

    friend bool operator>(const size_t& lhs, const DataFrameColIterator& rhs) {
      return lhs > rhs.m_current_col_idx;
    }

private:
    DataFrameIter m_df_begin;
    size_t        m_col_size;
    size_t        m_row_size;
    size_t        m_current_col_idx;
  };

  struct Shape {
    size_t col_count;
    size_t row_count;

    friend std::ostream& operator<<(std::ostream& os, const Shape& shape) {
      os << "Shape(c: " << shape.col_count << ", r: " << shape.row_count << ")";
      return os;
    }
  };

  template<NumericalTypes T>
  class DataFrame {
public:
    using ValueType            = Cell<T>;
    using pValueType           = Cell<T>*;
    using RowSeries            = RowSeries<T>;
    using ColumnSeries         = ColumnSeries<T>;
    using DataFrameIterator    = Iterator<DataFrame<T>>;
    using DataFrameRowIterator = DataFrameRowIterator<DataFrame<T>>;
    using DataFrameColIterator = DataFrameColIterator<DataFrame<T>>;
    using size_t               = df_ui32;

public:
    DataFrame(const std::vector<std::string>& col_names, const std::vector<std::string>& row_names) {
      m_col_count    = col_names.size();
      m_row_count    = row_names.size();
      m_col_size     = m_row_count;
      m_row_size     = m_col_count;
      m_current_size = m_col_count * m_row_count;
      m_d            = new ValueType[m_current_size];

      for (size_t i = 0; i < m_col_count; i++) {
        m_col_idx_map.insert({col_names[i], i});
      }

      for (size_t i = 0; i < m_row_count; i++) {
        m_row_idx_map.insert({row_names[i], i});
      }

      // 00 01 02 03 04
      // 05 06 07 08 09
      // 10 11 12 13 14
      // 15 16 17 18 19

      // 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19

      for (size_t i = 0; i < m_current_size; i++) {
        m_d[i].idx.global_idx = i;

        m_d[i].idx.col_idx  = i % m_col_count;
        m_d[i].idx.col_name = col_names[i % m_col_count];

        // std::cout << i % m_col_count << " " << col_names[i % m_col_count] << "\n";

        m_d[i].idx.row_idx  = i / m_col_count;
        m_d[i].idx.row_name = row_names[i / m_col_count];
      }
    }

    DataFrame(const DataFrame& other)
        : m_col_idx_map(other.m_col_idx_map),
          m_row_idx_map(other.m_row_idx_map),
          m_current_size(other.m_current_size),
          m_col_size(other.m_col_size),
          m_col_count(other.m_col_count),
          m_row_size(other.m_row_size),
          m_row_count(other.m_row_count),
          m_d(new ValueType[m_current_size]) {
      for (size_t idx = 0; idx < m_current_size; idx++) {
        // m_d[idx].value = other.m_d[idx].value;
        // m_d[idx].idx   = other.m_d[idx].idx;
        m_d[idx] = other.m_d[idx];
      }
    }

    ~DataFrame() {
      delete[] m_d;
    }

    // DataFrame operator=(const DataFrame& other) {
    //   return DataFrame(other);
    // }

    DataFrame operator=(const DataFrame& other) = delete;

    ValueType& operator[](const size_t& idx) {
      return *(m_d + idx);
    }

    const ValueType& operator[](const size_t& idx) const {
      return *(m_d + idx);
    }

    ValueType& operator[](const size_t& col_idx, const size_t& row_idx) {
      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_size - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_size - 1), "index out of range");

      return *(m_d + ((m_row_size * row_idx) + col_idx));
    }

    const ValueType& operator[](const size_t& col_idx, const size_t& row_idx) const {
      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_siz - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_siz - 1), "index out of range");

      return *(m_d + ((m_row_size * row_idx) + col_idx));
    }

    ValueType& operator[](const std::string& col_name, const std::string& row_name) {
      DF_ASSERT(m_col_idx_map.contains(col_name), col_name);
      DF_ASSERT(m_row_idx_map.contains(row_name), row_name);

      size_t col_idx = m_col_idx_map[col_name];
      size_t row_idx = m_row_idx_map[row_name];

      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_count - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_count - 1), "index out of range");

      ValueType& cell = *(m_d + ((m_row_size * row_idx) + col_idx));
      return cell;
    }

    const ValueType& operator[](const std::string& col_name, const std::string& row_name) const {
      DF_ASSERT(m_col_idx_map.contains(col_name), col_name);
      DF_ASSERT(m_row_idx_map.contains(row_name), row_name);

      size_t col_idx = m_col_idx_map.at(col_name);
      size_t row_idx = m_row_idx_map.at(row_name);

      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_count - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_count - 1), "index out of range");

      ValueType& cell = *(m_d + ((m_row_size * row_idx) + col_idx));
      return cell;
    }

    DataFrame<T> copy() {
      return DataFrame(*this);
    }

    size_t get_col_idx(std::string col) {
      return m_col_idx_map[col];
    }

    std::optional<std::string> get_col_name(size_t col_idx) {
      for (const auto& [col_name, idx] : m_col_idx_map) {
        if (idx == col_idx) {
          return col_name;
        }
      }
      return std::nullopt;
    }

    size_t get_row_idx(std::string row) {
      return m_row_idx_map[row];
    }

    std::optional<std::string> get_row_name(size_t row_idx) {
      for (const auto& [row_name, idx] : m_row_idx_map) {
        if (idx == row_idx) {
          return row_name;
        }
      }
      return std::nullopt;
    }

    ValueType& at(size_t idx) {
      return m_d[idx];
    }

    const ValueType& at(size_t idx) const {
      return m_d[idx];
    }

    DataFrameIterator begin() {
      return DataFrameIterator(m_d);
    }

    DataFrameIterator begin() const {
      return DataFrameIterator(m_d);
    }

    DataFrameIterator end() {
      return DataFrameIterator(m_d + m_current_size);
    }

    DataFrameIterator end() const {
      return DataFrameIterator(m_d + m_current_size);
    }

    constexpr size_t size() const {
      return m_current_size;
    }

    constexpr size_t col_size() const {
      return m_col_size;
    }

    constexpr size_t row_size() const {
      return m_row_size;
    }

    constexpr Shape shape() const {
      return {.col_count = m_col_count, .row_count = m_row_count};
    }

    ColumnSeries get_col(size_t col_idx) {
      return ColumnSeries{begin() + col_idx, m_col_size, m_row_size};
    }

    ColumnSeries get_column(std::string col_name) {
      return ColumnSeries{begin() + get_col_idx(col_name), m_col_size, m_row_size};
    }

    RowSeries get_row(size_t row_idx) {
      return RowSeries{begin(), row_idx, m_row_size};
    }

    RowSeries get_row(std::string row_name) {
      return RowSeries{begin(), get_row_idx(row_name), m_row_size};
    }

    DataFrameRowIterator iter_rows() {
      return DataFrameRowIterator(begin(), m_row_size);
    }

    DataFrameColIterator iter_cols() {
      return DataFrameColIterator(begin(), m_col_size, m_row_size);
    }

    DataFrame sort_rows(std::string col_name, bool inplace = true) {
      size_t       col_idx = get_col_idx(col_name);
      ColumnSeries col     = get_column(col_name);

      pValueType* sorted_cells = new pValueType[col.size()];

      std::vector<RowSeries> rows;
      for (auto row = iter_rows(); row < end(); row++) {
        rows.push_back(row.row());
      }

      // check if row copy works ... it does.
      // for (int i = 0; i < rows.size(); i++) {
      //   // std::cout << rows[i];
      //   for (const auto& row_item : rows[i]) {
      //     std::cout << *row_item << "\n";
      //   }
      // }

      sorted_cells[0] = col[0];

      for (int idx = 0; idx < col.size(); idx++) {
        bool lower_found = false;
        int  insert_idx  = 0;
        for (int sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
          if (col[idx]->value < sorted_cells[sorted_idx]->value) {
            lower_found = true;
            insert_idx  = sorted_idx;
            break;
          }
        }
        if (lower_found) {
          for (int i = idx - 1; i >= insert_idx; i--) {
            sorted_cells[i + 1] = sorted_cells[i];
          }
          sorted_cells[insert_idx] = col[idx];
        } else {
          sorted_cells[idx] = col[idx];
        }
      }

      // sort
      size_t     idx       = 0;
      ValueType* temp_vals = new ValueType[m_current_size];

      for (size_t i = 0; i < col.size(); i++) {
        for (auto c : get_row(sorted_cells[i]->idx.row_idx)) {
          temp_vals[idx] = *c;
          idx++;
        }
        m_row_idx_map[sorted_cells[i]->idx.row_name] = i;
      }

      delete[] sorted_cells;

      for (size_t i = 0; i < m_current_size; i++) {
        m_d[i] = temp_vals[i];
      }
      delete[] temp_vals;

      return *this;
    }

    void print() {
      char* l;
      for (size_t i = 0; i < m_col_count; i++) {
        if (i == 0) {
          printf("%25s", get_col_name(i).value().c_str());
        } else if (i == m_col_count - 1) {
          printf("%20s\n", get_col_name(i).value().c_str());
        } else {
          printf("%20s", get_col_name(i).value().c_str());
        }
      }

      printf("idx\n");

      for (size_t i = 0; i < size(); i++) {
        if (i % m_col_count == 0) {
          printf("%3lu %5s", ((i + 1) / m_col_count), get_row_name(i / m_col_count).value().c_str());
        }
        printf("%20d", m_d[i].value);
        if (((i + 1) % m_col_count) == 0 && i != 0) {
          printf("\n");
        }
      }
      printf("\n");
    }

private:
    std::map<std::string, size_t> m_col_idx_map;
    std::map<std::string, size_t> m_row_idx_map;
    size_t                        m_current_size;
    size_t                        m_col_size;
    size_t                        m_col_count;
    size_t                        m_row_size;
    size_t                        m_row_count;
    ValueType*                    m_d;
  };
} // namespace df
#endif // DATA_FRAME_H
