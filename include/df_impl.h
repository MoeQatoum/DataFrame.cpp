#ifndef DATA_FRAME_IMPL_H
#define DATA_FRAME_IMPL_H

#include "df_common.h"

#include "df_algo.h"
#include "df_cell.h"
#include "df_col_series.h"
#include "df_row_series.h"

namespace df {

  template<NumericalTypes T>
  class DataFrame;

  template<typename Iterable>
  class Iterator {
    using ValueType = typename Iterable::ValueType;

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

    void operator+=(size_t off) {
      m_d += off;
    }

    friend i32 operator-(const Iterator& lhs, const Iterator& rhs) {
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

    void operator-=(size_t off) {
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

#ifdef QT_IMPLEMENTATION
    friend QDebug operator<<(QDebug dbg, const Iterator& itr) {
      dbg.noquote();
      dbg << "Iterator(current addr: " << itr.m_d << ")";
      return dbg;
    }
#else
    friend ostream& operator<<(ostream& os, const Iterator& itr) {
      os << "Iterator(current addr: " << itr.m_d << ")";
      return os;
    }
#endif

private:
    ValueType* m_d;
  };

  template<typename Iterable>
  class DataFrameRowIterator {
    using DataFrameIterator = typename Iterable::DataFrameIterator;
    using RowSeries         = typename Iterable::RowSeries;

    using size_t = size_t;

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

    using size_t = size_t;

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

    void operator+=(size_t off) {
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
#ifdef QT_IMPLEMENTATION
    friend QDebug operator<<(QDebug dbg, const Shape& shape) {
      dbg.noquote();
      dbg << "Shape(c: " << shape.col_count << ", r: " << shape.row_count << ")";
      return dbg;
    }
#else
    friend ostream& operator<<(ostream& os, const Shape& shape) {
      os << "Shape(c: " << shape.col_count << ", r: " << shape.row_count << ")";
      return os;
    }
#endif
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

public:
    DataFrame(const StringList& col_names, const StringList& row_names) {
      m_col_count    = static_cast<size_t>(col_names.size());
      m_row_count    = static_cast<size_t>(row_names.size());
      m_col_size     = m_row_count;
      m_row_size     = m_col_count;
      m_current_size = m_col_count * m_row_count;
      m_d            = new ValueType[m_current_size];

      for (size_t i = 0; i < m_col_count; i++) {
#ifdef QT_IMPLEMENTATION
        m_col_idx_map.insert(col_names[i], i);
#else
        m_col_idx_map.insert({col_names[i], i});
#endif
      }

      for (size_t i = 0; i < m_row_count; i++) {
#ifdef QT_IMPLEMENTATION
        m_row_idx_map.insert(row_names[i], i);
#else
        m_row_idx_map.insert({row_names[i], i});
#endif
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
      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_size - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_size - 1), "index out of range");

      return *(m_d + ((m_row_size * row_idx) + col_idx));
    }

    ValueType& operator[](const String& col_name, const String& row_name) {
      DF_ASSERT(m_col_idx_map.contains(col_name), col_name);
      DF_ASSERT(m_row_idx_map.contains(row_name), row_name);

      size_t col_idx = m_col_idx_map[col_name];
      size_t row_idx = m_row_idx_map[row_name];

      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_count - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_count - 1), "index out of range");

      ValueType& cell = *(m_d + ((m_row_size * row_idx) + col_idx));
      return cell;
    }

    const ValueType& operator[](const String& col_name, const String& row_name) const {
      DF_ASSERT(m_col_idx_map.contains(col_name), col_name);
      DF_ASSERT(m_row_idx_map.contains(row_name), row_name);

#ifdef QT_IMPLEMENTATION
      size_t col_idx = m_col_idx_map[col_name];
      size_t row_idx = m_row_idx_map[row_name];
#else
      size_t col_idx = m_col_idx_map.at(col_name);
      size_t row_idx = m_row_idx_map.at(row_name);
#endif

      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_count - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_count - 1), "index out of range");

      ValueType& cell = *(m_d + ((m_row_size * row_idx) + col_idx));
      return cell;
    }

    DataFrame<T> copy() {
      return DataFrame(*this);
    }

    size_t get_col_idx(String col) {
      return m_col_idx_map[col];
    }

    std::optional<String> get_col_name(size_t col_idx) {
#ifdef QT_IMPLEMENTATION
      for (const String& col_name : m_col_idx_map.keys()) {
        if (m_col_idx_map[col_name] == col_idx) {
          return col_name;
        }
      }
#else
      for (const auto& [col_name, idx] : m_col_idx_map) {
        if (idx == col_idx) {
          return col_name;
        }
      }
#endif
      return std::nullopt;
    }

    size_t get_row_idx(String row) {
      return m_row_idx_map[row];
    }

    std::optional<String> get_row_name(size_t row_idx) {
#ifdef QT_IMPLEMENTATION
      for (const String& row_name : m_row_idx_map.keys()) {
        if (m_row_idx_map[row_name] == row_idx) {
          return row_name;
        }
      }
#else
      for (const auto& [row_name, idx] : m_row_idx_map) {
        if (idx == row_idx) {
          return row_name;
        }
      }
#endif
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

    constexpr size_t col_count() {
      return m_col_count;
    }

    constexpr size_t col_count() const {
      return m_col_count;
    }

    constexpr size_t row_size() const {
      return m_row_size;
    }

    constexpr size_t row_count() {
      return m_row_count;
    }

    constexpr size_t row_count() const {
      return m_row_count;
    }

    constexpr Shape shape() const {
      return {.col_count = m_col_count, .row_count = m_row_count};
    }

    ColumnSeries get_col(size_t col_idx) {
      return ColumnSeries{begin() + col_idx, m_col_size, m_row_size};
    }

    ColumnSeries get_column(String col_name) {
      return ColumnSeries{begin() + get_col_idx(col_name), m_col_size, m_row_size};
    }

    RowSeries get_row(size_t row_idx) {
      return RowSeries{begin(), row_idx, m_row_size};
    }

    RowSeries get_row(String row_name) {
      return RowSeries{begin(), get_row_idx(row_name), m_row_size};
    }

    DataFrameRowIterator iter_rows() {
      return DataFrameRowIterator(begin(), m_row_size);
    }

    DataFrameColIterator iter_cols() {
      return DataFrameColIterator(begin(), m_col_size, m_row_size);
    }

    DataFrame aec_sort_rows(String col_name, bool inplace = true) {
      size_t       col_idx = get_col_idx(col_name);
      ColumnSeries col     = get_column(col_name);

      pValueType* sorted_cells = new pValueType[col.size()];

      List<RowSeries> rows;
      for (auto row_iterator = iter_rows(); row_iterator < end(); row_iterator++) {
        rows.push_back(row_iterator.current_row());
      }

      sorted_cells[0] = col[0];

      for (size_t idx = 0; idx < col.size(); idx++) {
        bool   lower_found = false;
        size_t insert_idx  = 0;
        for (size_t sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
          if (col[idx]->value < sorted_cells[sorted_idx]->value) {
            lower_found = true;
            insert_idx  = sorted_idx;
            break;
          }
        }
        if (lower_found) {
          for (size_t i = idx - 1; i >= insert_idx; i--) {
            sorted_cells[i + 1] = sorted_cells[i];

            // avoid size_t (aka df_ui32) underflow
            if (i == 0) {
              break;
            }
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

    DataFrame dec_sort_rows(String col_name, bool inplace = true) {
      size_t       col_idx = get_col_idx(col_name);
      ColumnSeries col     = get_column(col_name);

      pValueType* sorted_cells = new pValueType[col.size()];

      List<RowSeries> rows;
      for (auto row_iterator = iter_rows(); row_iterator < end(); row_iterator++) {
        rows.push_back(row_iterator.current_row());
      }

      sorted_cells[0] = col[0];

      for (size_t idx = 0; idx < col.size(); idx++) {
        bool   higher_found = false;
        size_t insert_idx   = 0;
        for (size_t sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
          if (col[idx]->value > sorted_cells[sorted_idx]->value) {
            higher_found = true;
            insert_idx   = sorted_idx;
            break;
          }
        }
        if (higher_found) {
          for (size_t i = idx - 1; i >= insert_idx; i--) {
            sorted_cells[i + 1] = sorted_cells[i];

            // avoid size_t (aka df_ui32) underflow
            if (i == 0) {
              break;
            }
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

    void print(size_t head = 0, size_t tail = 0) {
      for (size_t i = 0; i < m_col_count; i++) {
        if (i == 0) {
#ifdef QT_IMPLEMENTATION
          printf("%25s", get_col_name(i).value().toUtf8().constData());
#else
          printf("%25s", get_col_name(i).value().c_str());
#endif
        } else if (i == m_col_count - 1) {
#ifdef QT_IMPLEMENTATION
          printf("%20s\n", get_col_name(i).value().toUtf8().constData());
#else
          printf("%20s\n", get_col_name(i).value().c_str());
#endif
        } else {
#ifdef QT_IMPLEMENTATION
          printf("%20s", get_col_name(i).value().toUtf8().constData());
#else
          printf("%20s", get_col_name(i).value().c_str());
#endif
        }
      }

      printf("idx\n");

      if (head > 0 || tail > 0) {
        if (head > 0) {
          for (size_t i = 0; i < head * m_row_size; i++) {
            if (i % m_col_count == 0) {
#ifdef QT_IMPLEMENTATION
              printf("%3lu %5s", ((i + 1) / m_col_count), get_row_name(i / m_col_count).value().toUtf8().constData());
#else
              printf("%3lu %5s", ((i + 1) / m_col_count), get_row_name(i / m_col_count).value().c_str());
#endif
            }
            printf("%20d", m_d[i].value);
            if (((i + 1) % m_col_count) == 0 && i != 0) {
              printf("\n");
            }
          }
        }

        if (tail > 0) {
          for (size_t i = m_current_size - (tail * m_row_size); i < m_current_size; i++) {
            if (i % m_col_count == 0) {
#ifdef QT_IMPLEMENTATION
              printf("%3lu %5s", ((i + 1) / m_col_count), get_row_name(i / m_col_count).value().toUtf8().constData());
#else
              printf("%3lu %5s", ((i + 1) / m_col_count), get_row_name(i / m_col_count).value().c_str());
#endif
            }
            printf("%20d", m_d[i].value);
            if (((i + 1) % m_col_count) == 0 && i != 0) {
              printf("\n");
            }
          }
        }
      } else {
        for (size_t i = 0; i < m_current_size; i++) {
          if (i % m_col_count == 0) {
#ifdef QT_IMPLEMENTATION
            printf("%3lu %5s", ((i + 1) / m_col_count), get_row_name(i / m_col_count).value().toUtf8().constData());
#else
            printf("%3lu %5s", ((i + 1) / m_col_count), get_row_name(i / m_col_count).value().c_str());
#endif
          }
          printf("%20d", m_d[i].value);
          if (((i + 1) % m_col_count) == 0 && i != 0) {
            printf("\n");
          }
        }
      }
      printf("\n");
    }

private:
    IndexHash  m_col_idx_map;
    IndexHash  m_row_idx_map;
    size_t     m_current_size;
    size_t     m_col_size;
    size_t     m_col_count;
    size_t     m_row_size;
    size_t     m_row_count;
    ValueType* m_d;
  };
} // namespace df
#endif // DATA_FRAME_H
