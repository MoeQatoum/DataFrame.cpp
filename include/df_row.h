
#ifndef DATA_FRAME_ROW_H
#define DATA_FRAME_ROW_H

#include "df_common.h"

namespace df {

  template<typename Iterable>
  class Iterator;

  template<typename T>
  class Cell;

  template<typename T>
  class Series;

  template<typename T>
  class DataFrame;

  template<typename T>
  class Column;

  template<typename T>
  class RowGroup_Logger;

  template<typename T>
  class LoggingContext;

  template<typename T>
  struct Row {

    using ValueType         = typename DataFrame<T>::pValueType;
    using DataFrameIterator = typename DataFrame<T>::Iterator;
    using RowIterator       = Iterator<Row>;

    Row() : m_size(0), m_d(nullptr) {
    }

    Row(DataFrameIterator df_begin, sizetype row_idx, sizetype row_size) {
      m_size = row_size;
      m_d    = new ValueType[m_size];

      for (sizetype idx = 0; idx < row_size; idx++) {
        m_d[idx] = &(df_begin + ((row_idx * row_size) + idx));
      }
    }

    Row(const Row& other) : m_size(other.m_size), m_d(new ValueType[m_size]) {
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i] = other.m_d[i];
      }
    }

    ~Row() {
      delete[] m_d;
    }

    ValueType& operator[](const sizetype& idx) {
      return m_d[idx];
    }

    const ValueType& operator[](const sizetype& idx) const {
      return m_d[idx];
    }

    ValueType operator[](const String& col_name) {
      for (sizetype i = 0; i < m_size; i++) {
        if (m_d[i]->idx.col_name == col_name) {
          return m_d[i];
        }
      }

#ifdef QT_IMPLEMENTATION
      qWarning() << "col name not found.";
      qTerminate();
#else
      std::cerr << "col name not found.";
      abort();
#endif
    }

    Row& operator=(const Row& rhs) {
      if (this != &rhs) {
        if (is_null()) {
          FORCED_ASSERT(m_d == nullptr, "m_d supposed to be null pointer, something is wrong");
          m_size = rhs.m_size;
          m_d    = new ValueType[m_size];
          for (sizetype i = 0; i < m_size; i++) {
            m_d[i] = rhs[i];
          }
        } else {
          FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
          for (sizetype i = 0; i < m_size; i++) {
            m_d[i]->value = rhs[i]->value;
          }
        }
      }
      return *this;
    }

    Row& operator=(const Series<T>& rhs) {
      FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i]->value = rhs[i];
      }
    }

    Series<T> copy_data() {
      Series<T> data(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        data[i] = m_d[i]->value;
      }
      return data;
    }

    Series<T> copy_data() const {
      Series<T> data(m_size);
      for (sizetype i = 0; i < m_size; i++) {
        data[i] = m_d[i]->value;
      }
      return data;
    }

    ValueType at_column(const String& col_name) {
      for (sizetype i = 0; i < m_size; i++) {
        if (m_d[i]->idx.col_name == col_name) {
          return m_d[i];
        }
      }

#ifdef QT_IMPLEMENTATION
      qWarning() << "col name not found.";
      qTerminate();
#else
      std::cerr << "col name not found.";
      abort();
#endif
    }

    ValueType at_column(const String& col_name) const {
      for (sizetype i = 0; i < m_size; i++) {
        if (m_d[i]->idx.col_name == col_name) {
          return m_d[i];
        }
      }

#ifdef QT_IMPLEMENTATION
      qWarning() << "col name not found.";
      qTerminate();
#else
      std::cerr << "col name not found.";
      abort();
#endif
    }

#ifdef QT_IMPLEMENTATION
    friend QDebug operator<<(QDebug dbg, const Row& row) {
      dbg.noquote().nospace();
      dbg << "Row(addr: " << &row << ", size: " << row.m_size << ", type: " << typeid(T).name() << ")";
      return dbg;
    }
#else
    friend ostream& operator<<(ostream& os, const Row& row) {
      os << "Row(addr: " << &row << ", size: " << row.m_size << ", type: " << typeid(T).name() << ")";
      return os;
    }
#endif

    constexpr sizetype size() {
      return m_size;
    }

    constexpr sizetype size() const {
      return m_size;
    }

    constexpr const sizetype& index() {
      return m_d[0]->idx.row_idx;
    }

    constexpr const sizetype& index() const {
      return m_d[0]->idx.row_idx;
    }

    String& name() {
      return m_d[0]->idx.row_name;
    }

    const String& name() const {
      return m_d[0]->idx.row_name;
    }

    RowIterator begin() {
      return RowIterator(m_d);
    }

    RowIterator begin() const {
      return RowIterator(m_d);
    }

    RowIterator end() {
      return RowIterator(m_d + m_size);
    }

    RowIterator end() const {
      return RowIterator(m_d + m_size);
    }

    bool is_null() {
      return (m_d == nullptr) && (m_size == 0);
    }

private:
    sizetype   m_size;
    ValueType* m_d;
  };

  template<typename T>
  class RowGroup {
    using value_t = typename Row<T>::ValueType;

public:
    RowGroup(const DataFrame<T>& df) : logger(this), logging_context(df.logger.context) {
      for (auto row_iter = df.iter_rows(); row_iter < df.end(); row_iter++) {
#ifdef QT_IMPLEMENTATION
        m_rows.push_back(row_iter.current_row());
#else
        m_rows.push_back(row_iter.current_row());
#endif
      }

      logger.with_context(logging_context);
    }

    RowGroup(const RowGroup& other) : m_rows(other.m_rows), logger(this), logging_context(other.logging_context) {
      logger.with_context(logging_context);
    }

    Row<T> operator[](const sizetype& idx) {
      return m_rows[idx];
    }

    Row<T> operator[](const sizetype& idx) const {
      return m_rows[idx];
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    RowGroup& descending_sorted(Column<T> column) {
      Series<value_t> sorted_cells(size());
      bool            lower_found;
      sizetype        insert_idx;
      for (sizetype idx = 0; idx < m_rows.size(); idx++) {
        lower_found = false;
        insert_idx  = 0;
        for (sizetype sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
          if (column[idx]->value > sorted_cells[sorted_idx]->value) {
            lower_found = true;
            insert_idx  = sorted_idx;
            break;
          }
        }
        if (lower_found) {
          for (sizetype i = idx - 1; i >= insert_idx; i--) {
            sorted_cells[i + 1] = sorted_cells[i];
            // avoid sizetype underflow
            if (i == 0) {
              break;
            }
          }
          sorted_cells[insert_idx] = column[idx];
        } else {
          sorted_cells[idx] = column[idx];
        }
      }
      List<Row<T>> unsorted_rows;
      unsorted_rows.resize(m_rows.size());
      for (sizetype i = 0; i < m_rows.size(); i++) {
        unsorted_rows[i] = m_rows[sorted_cells[i]->idx.row_idx];
      }
      m_rows.swap(unsorted_rows);
      return *this;
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    RowGroup ascending_sorted(const Column<T>& column) {
      Series<value_t> sorted_cells(size());
      bool            lower_found;
      sizetype        insert_idx;
      for (sizetype idx = 0; idx < m_rows.size(); idx++) {
        lower_found = false;
        insert_idx  = 0;
        for (sizetype sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
          if (column[idx]->value < sorted_cells[sorted_idx]->value) {
            lower_found = true;
            insert_idx  = sorted_idx;
            break;
          }
        }
        if (lower_found) {
          for (sizetype i = idx - 1; i >= insert_idx; i--) {
            sorted_cells[i + 1] = sorted_cells[i];
            // avoid sizetype underflow
            if (i == 0) {
              break;
            }
          }
          sorted_cells[insert_idx] = column[idx];
        } else {
          sorted_cells[idx] = column[idx];
        }
      }
      List<Row<T>> unsorted_rows;
      unsorted_rows.resize(m_rows.size());
      for (sizetype i = 0; i < m_rows.size(); i++) {
        unsorted_rows[i] = m_rows[sorted_cells[i]->idx.row_idx];
      }
      m_rows.swap(unsorted_rows);
      return *this;
    }

    sizetype size() {
      return m_rows.size();
    }

    sizetype size() const {
      return m_rows.size();
    }

    Row<T> at(sizetype index) {
      return m_rows[index];
    }

    Row<T> at(sizetype index) const {
      return m_rows[index];
    }

    typename List<Row<T>>::iterator begin() {
      return m_rows.begin();
    }

    typename List<Row<T>>::iterator end() {
      return m_rows.end();
    }

    void log(int range = 0) {
      logger.log(range);
    }

    RowGroup_Logger<T> logger;

private:
    List<Row<T>> m_rows;

    LoggingContext<T> logging_context;
  };

} // namespace df

#endif // COLUMN_SERIES_H
