#ifndef DATA_FRAME_IMPL_H
#define DATA_FRAME_IMPL_H

#include "df_common.h"

#include "df_cell.h"
#include "df_column.h"
#include "df_logger.h"
#include "df_row.h"

namespace df {

  template<typename T>
  class DataFrame;

  template<typename Iterable>
  class Iterator {
    using ValueType = typename Iterable::ValueType;

    template<typename>
    friend class RowIterator;

    template<typename>
    friend class ColumnIterator;

public:
    Iterator(ValueType* data_p) : m_d(data_p) {
    }

    Iterator(const Iterator& other) : m_d(other.m_d) {
    }

    ~Iterator() {
    }

    Iterator& operator=(const Iterator& other) {
      if (this != &other) {
        m_d = other.m_d;
      }
      return *this;
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

    friend const Iterator operator+(const sizetype& lhs, const Iterator& rhs) {
      Iterator tmp{rhs};
      tmp += lhs;
      return tmp;
    }

    friend const Iterator operator+(const Iterator& lhs, const sizetype& rhs) {
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

    void operator+=(sizetype off) {
      m_d += off;
    }

    friend ptrdiff_t operator-(const Iterator& lhs, const Iterator& rhs) {
      return lhs.m_d - rhs.m_d;
    }

    friend Iterator operator-(const Iterator& lhs, const sizetype& rhs) {
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

    void operator-=(sizetype off) {
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
  class RowIterator {
    using DataFrameIterator = typename Iterable::Iterator;
    using Row               = typename Iterable::Row;

public:
    RowIterator(DataFrameIterator df_begin, sizetype row_size)
        : m_df_begin(df_begin),
          m_row_size(row_size),
          m_current_row_idx(0) {
    }

    RowIterator(const RowIterator& other)
        : m_df_begin(other.m_df_begin),
          m_row_size(other.m_row_size),
          m_current_row_idx(other.m_current_row_idx) {
    }

    ~RowIterator() {
    }

    Row current_row() {
      return Row(m_df_begin, m_current_row_idx, m_row_size);
    }

    sizetype current_row_idx() {
      return m_current_row_idx;
    }

    RowIterator& operator=(const RowIterator& other) {
      if (this != &other) {
        m_df_begin        = other.m_df_begin;
        m_row_size        = other.m_row_size;
        m_current_row_idx = other.m_current_row_idx;
      }
      return *this;
    }

    RowIterator& operator++() {
      ++m_current_row_idx;
      return *this;
    }

    RowIterator operator++(int) {
      RowIterator tmp{*this};
      ++(*this);
      return tmp;
    }

    RowIterator& operator--() {
      --m_current_row_idx;
      return *this;
    }

    RowIterator operator--(int) {
      RowIterator tmp{*this};
      --(*this);
      return tmp;
    }

    friend bool operator<(const RowIterator& lhs, const DataFrameIterator& rhs) {
      return (lhs.m_df_begin + (lhs.m_current_row_idx * lhs.m_row_size)) < rhs;
    }

    friend bool operator<(const DataFrameIterator& lhs, const RowIterator& rhs) {
      return lhs < (rhs.m_df_begin + (rhs.m_current_row_idx * rhs.m_row_size));
    }

    friend bool operator>(const RowIterator& lhs, const DataFrameIterator& rhs) {
      return (lhs.m_df_begin + (lhs.m_current_row_idx * lhs.m_row_size)) > rhs;
    }

    friend bool operator>(const DataFrameIterator& lhs, const RowIterator& rhs) {
      return lhs > (rhs.m_df_begin + (rhs.m_current_row_idx * rhs.m_row_size));
    }

    friend bool operator<(const RowIterator& lhs, const sizetype& rhs) {
      return lhs.m_current_row_idx < rhs;
    }

    friend bool operator<(const sizetype& lhs, const RowIterator& rhs) {
      return lhs < rhs.m_current_row_idx;
    }

    friend bool operator>(const RowIterator& lhs, const sizetype& rhs) {
      return lhs.m_current_row_idx > rhs;
    }

    friend bool operator>(const sizetype& lhs, const RowIterator& rhs) {
      return lhs > rhs.m_current_row_idx;
    }

private:
    DataFrameIterator m_df_begin;
    sizetype          m_row_size;
    sizetype          m_current_row_idx;
  };

  template<typename Iterable>
  class ColumnIterator {
    using DataFrameIter = typename Iterable::Iterator;
    using Column        = typename Iterable::Column;

public:
    ColumnIterator(DataFrameIter df_begin, sizetype col_size, sizetype row_size)
        : m_df_begin(df_begin),
          m_col_size(col_size),
          m_row_size(row_size),
          m_current_col_idx(0) {
    }

    ColumnIterator(const ColumnIterator& other)
        : m_df_begin(other.m_df_begin),
          m_col_size(other.m_col_size),
          m_row_size(other.m_row_size),
          m_current_col_idx(other.m_current_col_idx) {
    }

    ~ColumnIterator() {
    }

    Column current_col() {
      return Column(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
    }

    sizetype current_col_idx() {
      return m_current_col_idx;
    }

    ColumnIterator& operator=(const ColumnIterator& other) {
      if (this != &other) {
        m_df_begin        = other.m_df_begin;
        m_col_size        = other.m_col_size;
        m_row_size        = other.m_row_size;
        m_current_col_idx = other.m_current_col_idx;
      }
      return *this;
    }

    ColumnIterator operator+(const sizetype& off) {
      ColumnIterator tmp{*this};
      tmp += off;
      return tmp;
    }

    ColumnIterator& operator++() {
      ++m_current_col_idx;
      return *this;
    }

    ColumnIterator operator++(int) {
      ColumnIterator tmp{*this};
      ++(*this);
      return tmp;
    }

    void operator+=(sizetype off) {
      m_current_col_idx += off;
    }

    friend bool operator<(const ColumnIterator& lhs, const DataFrameIter& rhs) {
      return (lhs.m_df_begin + (lhs.m_current_col_idx + (lhs.m_row_size * (lhs.m_col_size - 1)))) < rhs.m_d;
    }

    friend bool operator<(const DataFrameIter& lhs, const ColumnIterator& rhs) {
      return lhs.m_d < (rhs.m_df_begin + (rhs.m_current_col_idx + (rhs.m_row_size * rhs.m_col_size)));
    }

    friend bool operator>(const ColumnIterator& lhs, const DataFrameIter& rhs) {
      return (lhs.m_df_begin + (lhs.m_current_col_idx + (lhs.m_row_size * lhs.m_col_size))) > rhs.m_d;
    }

    friend bool operator>(const DataFrameIter& lhs, const ColumnIterator& rhs) {
      return lhs.m_d > (rhs.m_df_begin + (rhs.m_current_col_idx + (rhs.m_row_size * rhs.m_col_size)));
    }

    friend bool operator<(const ColumnIterator& lhs, const sizetype& rhs) {
      return lhs.m_current_col_idx < rhs;
    }

    friend bool operator<(const sizetype& lhs, const ColumnIterator& rhs) {
      return lhs < rhs.m_current_col_idx;
    }

    friend bool operator>(const ColumnIterator& lhs, const sizetype& rhs) {
      return lhs.m_current_col_idx > rhs;
    }

    friend bool operator>(const sizetype& lhs, const ColumnIterator& rhs) {
      return lhs > rhs.m_current_col_idx;
    }

private:
    DataFrameIter m_df_begin;
    sizetype      m_col_size;
    sizetype      m_row_size;
    sizetype      m_current_col_idx;
  };

  struct Shape {
    sizetype col_count;
    sizetype row_count;
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

  template<typename T>
  class DataFrame {
    friend class DF_Logger<T>;

public:
    using ValueType      = Cell<T>;
    using pValueType     = Cell<T>*;
    using Row            = Row<T>;
    using Column         = Column<T>;
    using Iterator       = Iterator<DataFrame<T>>;
    using RowIterator    = RowIterator<DataFrame<T>>;
    using ColumnIterator = ColumnIterator<DataFrame<T>>;
    using Logger         = DF_Logger<T>;

public:
    DataFrame(const StringList& col_names, const StringList& row_names) : logger(this, 0, 0) {
      m_col_count    = static_cast<sizetype>(col_names.size());
      m_row_count    = static_cast<sizetype>(row_names.size());
      m_col_size     = m_row_count;
      m_row_size     = m_col_count;
      m_current_size = m_col_count * m_row_count;
      m_d            = new ValueType[m_current_size];

      m_max_col_name_size = 0;
      m_max_row_name_size = 0;

      for (sizetype i = 0; i < m_col_count; i++) {
#ifdef QT_IMPLEMENTATION
        m_col_idx_map.insert(col_names[i], i);
#else
        m_col_idx_map.insert({col_names[i], i});
#endif
        if (col_names[i].size() > m_max_col_name_size) {
          m_max_col_name_size = col_names[i].size();
        }
      }

      for (sizetype i = 0; i < m_row_count; i++) {
#ifdef QT_IMPLEMENTATION
        m_row_idx_map.insert(row_names[i], i);
#else
        m_row_idx_map.insert({row_names[i], i});
#endif
        if (row_names[i].size() > m_max_row_name_size) {
          m_max_row_name_size = row_names[i].size();
        }
      }

      // 00 01 02 03 04
      // 05 06 07 08 09
      // 10 11 12 13 14
      // 15 16 17 18 19

      // 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19

      for (sizetype i = 0; i < m_current_size; i++) {
        m_d[i].idx.global_idx = i;

        m_d[i].idx.col_idx  = i % m_col_count;
        m_d[i].idx.col_name = col_names[i % m_col_count];

        m_d[i].idx.row_idx  = i / m_col_count;
        m_d[i].idx.row_name = row_names[i / m_col_count];
      }

      logger.set_max_col_name_size(m_max_col_name_size);
      logger.set_max_row_name_size(m_max_row_name_size);
    }

    DataFrame(const DataFrame& other)
        : m_col_idx_map(other.m_col_idx_map),
          m_row_idx_map(other.m_row_idx_map),
          m_current_size(other.m_current_size),
          m_col_size(other.m_col_size),
          m_col_count(other.m_col_count),
          m_row_size(other.m_row_size),
          m_row_count(other.m_row_count),
          m_max_col_name_size(other.m_max_col_name_size),
          m_max_row_name_size(other.m_max_row_name_size),
          logger(this, other.m_max_col_name_size, other.m_max_row_name_size),
          m_d(new ValueType[m_current_size]) {
      for (sizetype idx = 0; idx < m_current_size; idx++) {
        m_d[idx] = other.m_d[idx];
      }
    }

    ~DataFrame() {
      delete[] m_d;
    }

    DataFrame& operator=(const DataFrame& other) {
      FORCED_ASSERT(m_current_size == other.m_current_size,
                    "Copy assignment operator on DataFrame with other nonmatching size.");

      if (this != &other) {
        m_col_idx_map       = other.m_col_idx_map;
        m_row_idx_map       = other.m_row_idx_map;
        m_current_size      = other.m_current_size;
        m_col_size          = other.m_col_size;
        m_col_count         = other.m_col_count;
        m_row_size          = other.m_row_size;
        m_row_count         = other.m_row_count;
        m_max_col_name_size = other.m_max_col_name_size;
        m_max_row_name_size = other.m_max_row_name_size;
        logger              = other.logger;

        for (sizetype idx = 0; idx < m_current_size; idx++) {
          m_d[idx] = other.m_d[idx];
        }
      }
      return *this;
    }

    ValueType& operator[](const sizetype& idx) {
      return *(m_d + idx);
    }

    const ValueType& operator[](const sizetype& idx) const {
      return *(m_d + idx);
    }

    ValueType& operator[](const sizetype& col_idx, const sizetype& row_idx) {
      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_size - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_size - 1), "index out of range");

      return *(m_d + ((m_row_size * row_idx) + col_idx));
    }

    const ValueType& operator[](const sizetype& col_idx, const sizetype& row_idx) const {
      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_size - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_size - 1), "index out of range");

      return *(m_d + ((m_row_size * row_idx) + col_idx));
    }

    ValueType& operator[](const String& col_name, const String& row_name) {
      DF_ASSERT(m_col_idx_map.contains(col_name), col_name);
      DF_ASSERT(m_row_idx_map.contains(row_name), row_name);

      sizetype col_idx = m_col_idx_map[col_name];
      sizetype row_idx = m_row_idx_map[row_name];

      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_count - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_count - 1), "index out of range");

      ValueType& cell = *(m_d + ((m_row_size * row_idx) + col_idx));
      return cell;
    }

    const ValueType& operator[](const String& col_name, const String& row_name) const {
      DF_ASSERT(m_col_idx_map.contains(col_name), col_name);
      DF_ASSERT(m_row_idx_map.contains(row_name), row_name);

#ifdef QT_IMPLEMENTATION
      sizetype col_idx = m_col_idx_map[col_name];
      sizetype row_idx = m_row_idx_map[row_name];
#else
      sizetype col_idx = m_col_idx_map.at(col_name);
      sizetype row_idx = m_row_idx_map.at(row_name);
#endif

      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_count - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_count - 1), "index out of range");

      ValueType& cell = *(m_d + ((m_row_size * row_idx) + col_idx));
      return cell;
    }

    DataFrame copy() {
      return DataFrame(*this);
    }

    sizetype get_col_idx(String col) {
      return m_col_idx_map[col];
    }

    std::optional<String> get_col_name(sizetype col_idx) {
#ifdef QT_IMPLEMENTATION
      for (const sizetype& _col_idx : m_col_idx_map.values()) {
        if (col_idx == _col_idx) {
          return m_col_idx_map.key(col_idx);
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

    sizetype get_row_idx(String row) {
      return m_row_idx_map[row];
    }

    std::optional<String> get_row_name(sizetype row_idx) {
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

    ValueType& at(sizetype idx) {
      return m_d[idx];
    }

    const ValueType& at(sizetype idx) const {
      return m_d[idx];
    }

    constexpr sizetype size() const {
      return m_current_size;
    }

    constexpr sizetype col_size() const {
      return m_col_size;
    }

    constexpr sizetype col_count() {
      return m_col_count;
    }

    constexpr sizetype col_count() const {
      return m_col_count;
    }

    constexpr sizetype row_size() const {
      return m_row_size;
    }

    constexpr sizetype row_count() {
      return m_row_count;
    }

    constexpr sizetype row_count() const {
      return m_row_count;
    }

    Shape shape() const {
      return {.col_count = m_col_count, .row_count = m_row_count};
    }

    Column column(sizetype col_idx) {
      return Column{begin() + col_idx, m_col_size, m_row_size};
    }

    Column column(String col_name) {
      return Column{begin() + get_col_idx(col_name), m_col_size, m_row_size};
    }

    Row row(sizetype row_idx) {
      return Row{begin(), row_idx, m_row_size};
    }

    Row row(String row_name) {
      return Row{begin(), get_row_idx(row_name), m_row_size};
    }

    Iterator begin() {
      return Iterator(m_d);
    }

    Iterator begin() const {
      return Iterator(m_d);
    }

    Iterator end() {
      return Iterator(m_d + m_current_size);
    }

    Iterator end() const {
      return Iterator(m_d + m_current_size);
    }

    RowIterator iter_rows() {
      return RowIterator(begin(), m_row_size);
    }

    ColumnIterator iter_cols() {
      return ColumnIterator(begin(), m_col_size, m_row_size);
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    List<Row> ascending_sorted_rows(String col_name) {
      return ascending_sort(*this, col_name);
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    List<Row> descending_sorted_rows(String col_name) {
      return descending_sort(*this, col_name);
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    DataFrame inplace_ascending_sort(String col_name) {
      sizetype col_idx = get_col_idx(col_name);
      Column   col     = column(col_name);

      pValueType* sorted_cells = new pValueType[col.size()];

      List<Row> rows;
      for (auto row_iterator = iter_rows(); row_iterator < end(); row_iterator++) {
        rows.push_back(row_iterator.current_row());
      }

      sorted_cells[0] = col[0];

      for (sizetype idx = 0; idx < col.size(); idx++) {
        bool     lower_found = false;
        sizetype insert_idx  = 0;
        for (sizetype sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
          if (col[idx]->value < sorted_cells[sorted_idx]->value) {
            lower_found = true;
            insert_idx  = sorted_idx;
            break;
          }
        }
        if (lower_found) {
          for (sizetype i = idx - 1; i >= insert_idx; i--) {
            sorted_cells[i + 1] = sorted_cells[i];

            // avoidsizetype (aka df_ui32) underflow
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
      sizetype   idx       = 0;
      ValueType* temp_vals = new ValueType[m_current_size];

      for (sizetype i = 0; i < col.size(); i++) {
        for (auto c : row(sorted_cells[i]->idx.row_idx)) {
          temp_vals[idx] = *c;
          idx++;
        }
        m_row_idx_map[sorted_cells[i]->idx.row_name] = i;
      }

      delete[] sorted_cells;

      for (sizetype i = 0; i < m_current_size; i++) {
        m_d[i] = temp_vals[i];
      }
      delete[] temp_vals;

      return *this;
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    DataFrame inplace_descending_rows(String col_name) {
      sizetype col_idx = get_col_idx(col_name);
      Column   col     = column(col_name);

      pValueType* sorted_cells = new pValueType[col.size()];

      List<Row> rows;
      for (auto row_iterator = iter_rows(); row_iterator < end(); row_iterator++) {
        rows.push_back(row_iterator.current_row());
      }

      sorted_cells[0] = col[0];

      for (sizetype idx = 0; idx < col.size(); idx++) {
        bool     higher_found = false;
        sizetype insert_idx   = 0;
        for (sizetype sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
          if (col[idx]->value > sorted_cells[sorted_idx]->value) {
            higher_found = true;
            insert_idx   = sorted_idx;
            break;
          }
        }
        if (higher_found) {
          for (sizetype i = idx - 1; i >= insert_idx; i--) {
            sorted_cells[i + 1] = sorted_cells[i];

            if (i == 0) {
              break;
            }
          }
          sorted_cells[insert_idx] = col[idx];
        } else {
          sorted_cells[idx] = col[idx];
        }
      }

      sizetype   idx       = 0;
      ValueType* temp_vals = new ValueType[m_current_size];

      for (sizetype i = 0; i < col.size(); i++) {
        for (auto c : row(sorted_cells[i]->idx.row_idx)) {
          temp_vals[idx] = *c;
          idx++;
        }
        m_row_idx_map[sorted_cells[i]->idx.row_name] = i;
      }

      delete[] sorted_cells;

      for (sizetype i = 0; i < m_current_size; i++) {
        m_d[i] = temp_vals[i];
      }
      delete[] temp_vals;

      return *this;
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    void log(int range = 0) {
      logger.log(range);
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    void log_ascending_sorted_rows(String col_name, int range = 0) {
      logger.log_sorted_rows(ascending_sorted_rows(col_name), range);
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    void log_descending_sorted_rows(String col_name, int range = 0) {
      logger.log_sorted_rows(descending_sorted_rows(col_name), range);
    }

    Logger logger;

private:
    IndexHash  m_col_idx_map;
    IndexHash  m_row_idx_map;
    sizetype   m_current_size;
    sizetype   m_col_size;
    sizetype   m_col_count;
    sizetype   m_row_size;
    sizetype   m_row_count;
    ValueType* m_d;

    // logging
    sizetype m_max_col_name_size;
    sizetype m_max_row_name_size;
  };

} // namespace df
#endif // DATA_FRAME_H
