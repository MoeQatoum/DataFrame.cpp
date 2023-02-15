#ifndef DATA_FRAME_IMPL_H
#define DATA_FRAME_IMPL_H

#include "df_common.h"

#include "df_algo.h"
#include "df_cell.h"
#include "df_col_series.h"
#include "df_row_series.h"

namespace df {

  template<typename T>
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

    friend long operator-(const Iterator& lhs, const Iterator& rhs) {
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
  class DataFrameRowIterator {
    using DataFrameIterator = typename Iterable::DataFrameIterator;
    using RowSeries         = typename Iterable::RowSeries;

public:
    DataFrameRowIterator(DataFrameIterator df_begin, sizetype row_size)
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

    sizetype current_row_idx() {
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

    friend bool operator<(const DataFrameRowIterator& lhs, const sizetype& rhs) {
      return lhs.m_current_row_idx < rhs;
    }

    friend bool operator<(const sizetype& lhs, const DataFrameRowIterator& rhs) {
      return lhs < rhs.m_current_row_idx;
    }

    friend bool operator>(const DataFrameRowIterator& lhs, const sizetype& rhs) {
      return lhs.m_current_row_idx > rhs;
    }

    friend bool operator>(const sizetype& lhs, const DataFrameRowIterator& rhs) {
      return lhs > rhs.m_current_row_idx;
    }

private:
    DataFrameIterator m_df_begin;
    sizetype          m_row_size;
    sizetype          m_current_row_idx;
  };

  template<typename Iterable>
  class DataFrameColIterator {
    using DataFrameIter = typename Iterable::DataFrameIterator;
    using ColumnSeries  = typename Iterable::ColumnSeries;

public:
    DataFrameColIterator(DataFrameIter df_begin, sizetype col_size, sizetype row_size)
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

    sizetype current_col_idx() {
      return m_current_col_idx;
    }

    DataFrameColIterator operator+(const sizetype& off) {
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

    void operator+=(sizetype off) {
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

    friend bool operator<(const DataFrameColIterator& lhs, const sizetype& rhs) {
      return lhs.m_current_col_idx < rhs;
    }

    friend bool operator<(const sizetype& lhs, const DataFrameColIterator& rhs) {
      return lhs < rhs.m_current_col_idx;
    }

    friend bool operator>(const DataFrameColIterator& lhs, const sizetype& rhs) {
      return lhs.m_current_col_idx > rhs;
    }

    friend bool operator>(const sizetype& lhs, const DataFrameColIterator& rhs) {
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
      m_col_count    = static_cast<sizetype>(col_names.size());
      m_row_count    = static_cast<sizetype>(row_names.size());
      m_col_size     = m_row_count;
      m_row_size     = m_col_count;
      m_current_size = m_col_count * m_row_count;
      m_d            = new ValueType[m_current_size];

      m_max_col_name_size = 0;
      m_max_row_name_size = 0;
      m_floatPrecision    = 8;

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
    }

    DataFrame(const DataFrame& other)
        : m_col_idx_map(other.m_col_idx_map),
          m_row_idx_map(other.m_row_idx_map),
          m_current_size(other.m_current_size),
          m_col_size(other.m_col_size),
          m_col_count(other.m_col_count),
          m_row_size(other.m_row_size),
          m_row_count(other.m_row_count),
          m_floatPrecision(other.m_floatPrecision),
          m_spaceAdjustment(other.m_spaceAdjustment),
          m_max_col_name_size(other.m_max_col_name_size),
          m_max_row_name_size(other.m_max_row_name_size),
          m_d(new ValueType[m_current_size]) {
      for (sizetype idx = 0; idx < m_current_size; idx++) {
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

    DataFrame<T> copy() {
      return DataFrame(*this);
    }

    sizetype get_col_idx(String col) {
      return m_col_idx_map[col];
    }

    std::optional<String> get_col_name(sizetype col_idx) {
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

    constexpr Shape shape() const {
      return {.col_count = m_col_count, .row_count = m_row_count};
    }

    ColumnSeries get_col(sizetype col_idx) {
      return ColumnSeries{begin() + col_idx, m_col_size, m_row_size};
    }

    ColumnSeries get_column(String col_name) {
      return ColumnSeries{begin() + get_col_idx(col_name), m_col_size, m_row_size};
    }

    RowSeries get_row(sizetype row_idx) {
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

    template<std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    DataFrame asc_sort_rows(String col_name, bool inplace = true) {
      sizetype     col_idx = get_col_idx(col_name);
      ColumnSeries col     = get_column(col_name);

      pValueType* sorted_cells = new pValueType[col.size()];

      List<RowSeries> rows;
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
        for (auto c : get_row(sorted_cells[i]->idx.row_idx)) {
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

    template<std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
    DataFrame dec_sort_rows(String col_name, bool inplace = true) {
      sizetype     col_idx = get_col_idx(col_name);
      ColumnSeries col     = get_column(col_name);

      pValueType* sorted_cells = new pValueType[col.size()];

      List<RowSeries> rows;
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
        for (auto c : get_row(sorted_cells[i]->idx.row_idx)) {
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

    sizetype setFloatPrecision(sizetype precision) {
      m_floatPrecision = precision;
    }

    sizetype floatPrecision() {
      return m_floatPrecision;
    }

    sizetype floatPrecision() const {
      return m_floatPrecision;
    }

    sizetype setSpaceAdjustment(sizetype spaceAdjustment) {
      m_spaceAdjustment = spaceAdjustment;
    }

    sizetype spaceAdjustment() {
      return m_spaceAdjustment;
    }

    sizetype spaceAdjustment() const {
      return m_spaceAdjustment;
    }

    sizetype set_max_col_name_size(sizetype size) {
      m_max_col_name_size = size;
    }

    sizetype max_col_name_size() {
      return m_max_col_name_size;
    }

    sizetype max_col_name_size() const {
      return m_max_col_name_size;
    }

    sizetype set_max_row_name_size(sizetype size) {
      m_max_row_name_size = size;
    }

    sizetype max_row_name_size() {
      return m_max_row_name_size;
    }

    sizetype max_row_name_size() const {
      return m_max_row_name_size;
    }

#ifdef QT_IMPLEMENTATION

    void print(long range = 0) {
      DF_ASSERT(tail <= 0, "tail must be grater than 0");

      QDebug dbg       = clog.noquote().nospace();
      long   spacing   = 5;
      long   idx_space = 4;

      long row_name_space = m_max_row_name_size + spacing;
      long col_spacing    = m_max_col_name_size + spacing;

      dbg << String("%1").arg("idx", -(m_max_row_name_size + spacing + idx_space));
      for (const String& col_name : m_col_idx_map.keys()) {
        dbg << String("%1").arg(col_name, -(m_max_col_name_size + spacing));
      }
      dbg << "\n";

      DF_ASSERT(tail > m_row_count, "tail is grater than row count");

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = m_row_count + range;
        range_end   = m_row_count;
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const auto& row = get_row(idx);
        dbg << String("%1").arg(row.idx(), -idx_space) << String("%1").arg(row.name(), -(row_name_space));
        for (const auto& c : row) {
          if (std::is_floating_point_v<T>) {
            dbg << String("%1").arg(String::number(c->value, 'f', m_floatPrecision), -10);
          } else {
            dbg << String("%1").arg(c->value, -(col_spacing));
          }
        }
        dbg << "\n";
      }
    }

    friend QDebug operator<<(QDebug dbg, DataFrame& df) {
      dbg.noquote().nospace();
      sizetype spacing   = 5;
      sizetype idx_space = 4;

      sizetype row_name_space = df.m_max_row_name_size + spacing;
      sizetype col_spacing    = df.m_max_col_name_size + spacing;

      dbg << String("%1").arg("idx", -(df.m_max_row_name_size + spacing + idx_space));
      for (const String& col_name : df.m_col_idx_map.keys()) {
        dbg << String("%1").arg(col_name, -(df.m_max_col_name_size + spacing));
      }
      dbg << "\n";

      for (sizetype i = 0; i < df.m_row_count; i++) {
        const auto& row = df.get_row(i);
        dbg << String("%1").arg(row.idx(), -idx_space) << String("%1").arg(row.name(), -(row_name_space));
        for (const auto& c : row) {
          if (std::is_floating_point_v<T>) {
            dbg << String("%1").arg(String::number(c->value, 'f', df.m_floatPrecision), -10);
          } else {
            dbg << String("%1").arg(c->value, -(col_spacing));
          }
        }
        dbg << "\n";
      }
      return dbg;
    }

#else
    void print(long range = 0) {
      DF_ASSERT(tail <= 0, "tail must be grater than 0.");

      sizetype spacing   = 5;
      sizetype idx_space = 4;

      sizetype row_name_space = m_max_row_name_size + spacing;
      sizetype col_spacing    = m_max_col_name_size + spacing;

      clog << std::left << std::setw((m_max_row_name_size + spacing + idx_space)) << "idx";
      for (const auto& [col_name, v] : m_col_idx_map) {
        clog << std::left << std::setw(m_max_col_name_size + spacing) << col_name;
      }
      clog << "\n";

      DF_ASSERT(tail > m_row_count && tail >= 1, "tail is grater then row count");

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = m_row_count + range;
        range_end   = m_row_count;
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const auto& row = get_row(idx);
        clog << std::left << std::setw(idx_space) << row.idx() << std ::left << std::setw(row_name_space) << row.name();
        for (const auto& c : row) {
          if (std::is_floating_point_v<T>) {
            clog.precision(m_floatPrecision);
            clog << std::left << std::setw(col_spacing + m_floatPrecision) << c->value;
            clog.precision(0);
          } else {
            clog << std::left << std::setw(col_spacing) << c->value;
          }
        }
        clog << "\n";
      }
    }

    friend ostream& operator<<(ostream& os, DataFrame& df) {
      sizetype spacing   = 5;
      sizetype idx_space = 4;

      sizetype row_name_space = df.m_max_row_name_size + spacing;
      sizetype col_spacing    = df.m_max_col_name_size + spacing;

      os << std::left << std::setw(df.m_max_row_name_size + spacing + idx_space) << "idx";
      for (const auto& [col_name, v] : df.m_col_idx_map) {
        os << std::left << std::setw(df.m_max_col_name_size + spacing) << col_name;
      }
      os << "\n";

      for (sizetype i = 0; i < df.m_row_count; i++) {
        const auto& row = df.get_row(i);
        os << std::left << std::setw(idx_space) << row.idx() << std ::left << std::setw(row_name_space) << row.name();
        for (const auto& c : row) {
          if (std::is_floating_point_v<T>) {
            os.precision(df.m_floatPrecision);
            os << std::left << std::setw(col_spacing) << c->value;
            os.precision(0);
          } else {
            os << std::left << std::setw(col_spacing) << c->value;
          }
        }
        os << "\n";
      }
      return os;
    }
#endif

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
    long m_floatPrecision;
    long m_spaceAdjustment;
    long m_max_col_name_size;
    long m_max_row_name_size;
  };

} // namespace df
#endif // DATA_FRAME_H
