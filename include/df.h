#ifndef DATA_FRAME_IMPL_H
#define DATA_FRAME_IMPL_H

#include "df_common.h"

#include "df_cell.h"
#include "df_column.h"
#include "df_logger.h"
#include "df_row.h"
#include "df_series.h"

namespace df {

  template<typename T>
  class DataFrame;

  template<typename Iterable>
  class Iterator {
    using ValueType      = typename Iterable::ValueType;
    using ConstValueType = typename Iterable::ConstValueType;

    template<typename>
    friend class RowIterator;

    template<typename>
    friend class ColumnIterator;

public:
    Iterator(ValueType* p_data) : m_d(p_data) {
    }

    Iterator(const Iterator& other) : m_d(other.m_d) {
    }

    ~Iterator() {
    }

    template<typename ValueTye1, typename = std::enable_if_t<std::is_convertible_v<ValueType, ValueTye1>, bool>>
    operator ValueTye1() const {
      return m_d;
    }

    Iterator& operator=(const Iterator& other) {
      if (this != &other) {
        m_d = other.m_d;
      }
      return *this;
    }

    const Iterator& operator=(const Iterator& other) const {
      if (this != &other) {
        m_d = other.m_d;
      }
      return *this;
    }

    ValueType* operator&() {
      return m_d;
    }

    ConstValueType* operator&() const {
      return m_d;
    }

    ValueType& operator*() {
      return *m_d;
    }

    ConstValueType& operator*() const {
      return *m_d;
    }

    ValueType* operator->() {
      return m_d;
    }

    ConstValueType* operator->() const {
      return m_d;
    }

    friend Iterator operator+(const sizetype& lhs, const Iterator& rhs) {
      Iterator tmp{rhs};
      tmp += lhs;
      return tmp;
    }

    friend Iterator operator+(const Iterator& lhs, const sizetype& rhs) {
      Iterator tmp{lhs};
      tmp += rhs;
      return tmp;
    }

    Iterator& operator++() {
      m_d++;
      return *this;
    }

    Iterator operator++(int) const {
      Iterator tmp{*this};
      ++(*this);
      return tmp;
    }

    Iterator& operator+=(sizetype off) {
      m_d += off;
      return *this;
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

    // Iterator operator--(int) const {
    //   Iterator tmp = *this;
    //   --(*this);
    //   return tmp;
    // }

    Iterator& operator-=(sizetype off) const {
      m_d -= off;
      return *this;
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

#ifdef QT_SUPPORT
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
    using ConstRow          = typename Iterable::ConstRow;
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

    ConstRow current_row() const {
      return ConstRow(m_df_begin, m_current_row_idx, m_row_size);
    }

    sizetype current_row_idx() const {
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

    // TODO: move constructor ?

    operator Row() {
      return Row(m_df_begin, m_current_row_idx, m_row_size);
    }

    operator Row() const {
      return ConstRow(m_df_begin, m_current_row_idx, m_row_size);
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

    RowIterator operator--(int) const {
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
    using ConstColumn   = typename Iterable::ConstColumn;
    using Column        = typename Iterable::Column;

public:
    ColumnIterator(DataFrameIter df_begin, sizetype col_size, sizetype row_size)
        : m_df_begin(df_begin),
          m_col_size(col_size),
          m_row_size(row_size),
          m_current_col_idx(0) {
    }

    ~ColumnIterator() {
    }

    ColumnIterator(const ColumnIterator& other)
        : m_df_begin(other.m_df_begin),
          m_col_size(other.m_col_size),
          m_row_size(other.m_row_size),
          m_current_col_idx(other.m_current_col_idx) {
    }

    // TODO: move constructor ?

    Column current_col() {
      return Column(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
    }

    ConstColumn current_col() const {
      return ConstColumn(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
    }

    sizetype current_col_idx() const {
      return m_current_col_idx;
    }

    operator Column() {
      return Column(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
    }

    operator ConstColumn() const {
      return ConstColumn(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
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

    ColumnIterator operator+(const sizetype& off) const {
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
#ifdef QT_SUPPORT
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
    using ValueType       = Cell<T>;
    using ConstValueType  = const Cell<T>;
    using pValueType      = Cell<T>*;
    using pConstValueType = const pValueType;
    using Row             = Row<T>;
    using ConstRow        = const Row;
    using RowGroup        = RowGroup<T>;
    using ConstRowGroup   = const RowGroup;
    using Column          = Column<T>;
    using ConstColumn     = const Column;
    using Iterator        = Iterator<DataFrame<T>>;
    using RowIterator     = RowIterator<DataFrame<T>>;
    using ColumnIterator  = ColumnIterator<DataFrame<T>>;
    using Logger          = DF_Logger<T>;

    DataFrame()
        : logger(this),
          logging_context({}),
          m_current_size(0),
          m_col_size(0),
          m_col_count(0),
          m_row_size(0),
          m_row_count(0),
          m_d(nullptr) {
    }

    DataFrame(const StringList& col_names, const StringList& row_names) : logger(this), logging_context({}) {
      m_col_count    = static_cast<sizetype>(col_names.size());
      m_row_count    = static_cast<sizetype>(row_names.size());
      m_col_size     = m_row_count;
      m_row_size     = m_col_count;
      m_current_size = m_col_count * m_row_count;
      m_d            = new ValueType[m_current_size];

      for (sizetype i = 0; i < m_col_count; i++) {
#ifdef QT_SUPPORT
        m_col_idx_map.insert(col_names[i], i);
#else
        m_col_idx_map.insert({col_names[i], i});
#endif
        if (col_names[i].size() > logging_context.max_col_name_size) {
          logging_context.max_col_name_size = col_names[i].size();
        }
      }

      for (sizetype i = 0; i < m_row_count; i++) {
#ifdef QT_SUPPORT
        m_row_idx_map.insert(row_names[i], i);
#else
        m_row_idx_map.insert({row_names[i], i});
#endif
        if (row_names[i].size() > logging_context.max_row_name_size) {
          logging_context.max_row_name_size = row_names[i].size();
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

      logger.with_context(logging_context);
    }

    DataFrame(const RowGroup& rows) : logger(this), logging_context({}) {
      m_col_count    = rows.row_size();
      m_row_count    = rows.size();
      m_col_size     = m_row_count;
      m_row_size     = m_col_count;
      m_current_size = m_col_count * m_row_count;
      m_d            = new ValueType[m_current_size];

      sizetype idx = 0;
      for (sizetype row_idx = 0; row_idx < m_row_count; row_idx++) {
        for (sizetype i = 0; i < m_col_count; i++) {
          m_d[idx] = rows[row_idx][i];
          idx++;
        }
      }

      for (sizetype i = 0; i < m_col_count; i++) {
#ifdef QT_SUPPORT
        m_col_idx_map.insert(m_d[i].idx.col_name, i);
#else
        m_col_idx_map.insert({m_d[i].idx.col_name, i});
#endif
        if (m_d[i].idx.col_name.size() > logging_context.max_col_name_size) {
          logging_context.max_col_name_size = m_d[i].idx.col_name.size();
        }
      }

      for (sizetype i = 0; i < m_current_size; i += m_row_size) {
#ifdef QT_SUPPORT
        m_row_idx_map.insert(m_d[i].idx.row_name, i);
#else
        m_row_idx_map.insert({m_d[i].idx.row_name, i});
#endif
        if (m_d[i].idx.row_name.size() > logging_context.max_row_name_size) {
          logging_context.max_row_name_size = m_d[i].idx.row_name.size();
        }
      }

      logger.with_context(logging_context);
    }

    DataFrame(const DataFrame& other)
        : logger(this),
          m_col_idx_map(other.m_col_idx_map),
          m_row_idx_map(other.m_row_idx_map),
          m_current_size(other.m_current_size),
          m_col_size(other.m_col_size),
          m_col_count(other.m_col_count),
          m_row_size(other.m_row_size),
          m_row_count(other.m_row_count),
          m_d(new ValueType[m_current_size]),
          logging_context(other.logging_context) {
      for (sizetype idx = 0; idx < m_current_size; idx++) {
        m_d[idx] = other.m_d[idx];
      }
      logger.with_context(logging_context);
    }

    ~DataFrame() {
      delete[] m_d;
    }

    DataFrame& operator=(const DataFrame& other) {
      if (this != &other) {
        if (is_null()) {
          FORCED_ASSERT(m_d == nullptr, "m_d supposed to be null pointer, something is wrong");
          m_col_idx_map   = other.m_col_idx_map;
          m_row_idx_map   = other.m_row_idx_map;
          m_current_size  = other.m_current_size;
          m_col_size      = other.m_col_size;
          m_col_count     = other.m_col_count;
          m_row_size      = other.m_row_size;
          m_row_count     = other.m_row_count;
          logging_context = other.logging_context;
          m_d             = new ValueType[other.m_current_size];
          for (sizetype idx = 0; idx < m_current_size; idx++) {
            m_d[idx] = other.m_d[idx];
          }
        } else {
          FORCED_ASSERT(m_current_size == other.m_current_size,
                        "Copy assignment operator on DataFrame with other nonmatching size.");
          m_col_idx_map   = other.m_col_idx_map;
          m_row_idx_map   = other.m_row_idx_map;
          m_current_size  = other.m_current_size;
          m_col_size      = other.m_col_size;
          m_col_count     = other.m_col_count;
          m_row_size      = other.m_row_size;
          m_row_count     = other.m_row_count;
          logging_context = other.logging_context;
          for (sizetype idx = 0; idx < m_current_size; idx++) {
            m_d[idx] = other.m_d[idx];
          }
        }
      }
      return *this;
    }

    ValueType& operator[](const sizetype& idx) {
      return m_d[idx];
    }

    ConstValueType& operator[](const sizetype& idx) const {
      return m_d[idx];
    }

    Column operator[](const String& col_name) {
      return column(col_name);
    }

    ConstColumn operator[](const String& col_name) const {
      return column(col_name);
    }

    ValueType& operator[](const sizetype& col_idx, const sizetype& row_idx) {
      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_size - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_size - 1), "index out of range");
      return *(m_d + ((m_row_size * row_idx) + col_idx));
    }

    ConstValueType& operator[](const sizetype& col_idx, const sizetype& row_idx) const {
      DF_ASSERT(0 <= col_idx && col_idx <= (m_col_size - 1), "index out of range");
      DF_ASSERT(0 <= row_idx && row_idx <= (m_row_size - 1), "index out of range");
      return *(m_d + ((m_row_size * row_idx) + col_idx));
    }

    ValueType& operator[](const String& col_name, const String& row_name) {
      DF_ASSERT(m_col_idx_map.contains(col_name), col_name);
      DF_ASSERT(m_row_idx_map.contains(row_name), row_name);
      sizetype   col_idx = m_col_idx_map[col_name];
      sizetype   row_idx = m_row_idx_map[row_name];
      ValueType& cell    = *(m_d + ((m_row_size * row_idx) + col_idx));
      return cell;
    }

    ConstValueType& operator[](const String& col_name, const String& row_name) const {
      DF_ASSERT(m_col_idx_map.contains(col_name), col_name);
      DF_ASSERT(m_row_idx_map.contains(row_name), row_name);
#ifdef QT_SUPPORT
      sizetype col_idx = m_col_idx_map[col_name];
      sizetype row_idx = m_row_idx_map[row_name];
#else
      sizetype col_idx = m_col_idx_map.at(col_name);
      sizetype row_idx = m_row_idx_map.at(row_name);
#endif
      ValueType& cell = *(m_d + ((m_row_size * row_idx) + col_idx));
      return cell;
    }

    DataFrame copy() {
      return DataFrame(*this);
    }

    sizetype get_col_idx(String col) const {
#ifdef QT_SUPPORT
      return m_col_idx_map[col];
#else
      return m_col_idx_map.at(col);
#endif
    }

    std::optional<String> get_col_name(sizetype col_idx) const {
#ifdef QT_SUPPORT
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

    sizetype get_row_idx(String row) const {
#ifdef QT_SUPPORT
      return m_row_idx_map[row];
#else
      return m_row_idx_map.at(row);
#endif
    }

    std::optional<String> get_row_name(sizetype row_idx) const {
#ifdef QT_SUPPORT
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

    ConstValueType& at(sizetype idx) const {
      return m_d[idx];
    }

    sizetype size() const {
      return m_current_size;
    }

    sizetype col_size() const {
      return m_col_size;
    }

    sizetype col_count() const {
      return m_col_count;
    }

    sizetype row_size() const {
      return m_row_size;
    }

    sizetype row_count() const {
      return m_row_count;
    }

    Shape shape() const {
      return {.col_count = m_col_count, .row_count = m_row_count};
    }

    Column column(sizetype col_idx) {
      return Column{begin() + col_idx, m_col_size, m_row_size};
    }

    ConstColumn column(sizetype col_idx) const {
      return ConstColumn{begin() + col_idx, m_col_size, m_row_size};
    }

    Column column(String col_name) {
      return Column{begin() + get_col_idx(col_name), m_col_size, m_row_size};
    }

    ConstColumn column(String col_name) const {
      return ConstColumn{begin() + get_col_idx(col_name), m_col_size, m_row_size};
    }

    Row row(sizetype row_idx) {
      return Row{begin(), row_idx, m_row_size};
    }

    ConstRow row(sizetype row_idx) const {
      return ConstRow{begin(), row_idx, m_row_size};
    }

    Row row(String row_name) {
      return Row{begin(), get_row_idx(row_name), m_row_size};
    }

    ConstRow row(String row_name) const {
      return ConstRow{begin(), get_row_idx(row_name), m_row_size};
    }

    RowGroup rows() {
      return RowGroup(this);
    }

    ConstRowGroup rows() const {
      return ConstRowGroup(this);
    }

    Iterator begin() const {
      return Iterator(m_d);
    }

    Iterator end() const {
      return Iterator(m_d + m_current_size);
    }

    ColumnIterator iter_cols() {
      return ColumnIterator(begin(), m_col_size, m_row_size);
    }

    ColumnIterator iter_cols() const {
      return ColumnIterator(begin(), m_col_size, m_row_size);
    }

    RowIterator iter_rows() {
      return RowIterator(begin(), m_row_size);
    }

    RowIterator iter_rows() const {
      return RowIterator(begin(), m_row_size);
    }

    bool is_null() const {
      return (m_d == nullptr) && (m_col_count == 0) && (m_col_size == 0) && (m_row_size == 0) && (m_row_size == 0)
             && (m_current_size == 0);
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    RowGroup sort(String column_name, bool ascending = false) {
      return RowGroup(this).sort(column_name, ascending);
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    void log(int range = 0) {
      logger.log(range);
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

    LoggingContext<T> logging_context;
  };

} // namespace df
#endif // DATA_FRAME_H
