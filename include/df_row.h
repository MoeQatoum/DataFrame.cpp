
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
  class Row {
public:
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

    Row(Row&& other) : m_size(other.m_size), m_d(other.m_d) {
      other.m_d = nullptr;
    }

    ~Row() {
      delete[] m_d;
    }

    // replace values directly - not recommended
    // Row& operator()(const Row& other) {
    //   FORCED_ASSERT(m_size == other.m_size, "assignment operation on nonmatching size objects");
    //   for (sizetype i = 0; i < m_size; i++) {
    //     m_d[i]->value = other.m_d[i]->value;
    //   }
    //   return *this;
    // }

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
        } else {
          FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
        }
        for (sizetype i = 0; i < m_size; i++) {
          m_d[i] = rhs.m_d[i];
        }
      }
      return *this;
    }

    Row& operator=(Row&& rhs) {
      if (this != &rhs) {
        m_size  = rhs.m_size;
        m_d     = rhs.m_d;
        rhs.m_d = nullptr;
      }
      return *this;
    }

    Row& operator=(const Series<T>& rhs) {
      FORCED_ASSERT(m_d != nullptr, "m_d is not supposed to be null pointer, something is wrong");
      FORCED_ASSERT(m_size == rhs.size(), "assignment operation on nonmatching size objects");
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i]->value = rhs[i];
      }
      return *this;
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

    sizetype column_index(const String& column_name) {
      for (sizetype i = 0; i < m_size; i++) {
        if (m_d[i]->idx.col_name == column_name) {
          return i;
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

    sizetype column_index(const String& column_name) const {
      for (sizetype i = 0; i < m_size; i++) {
        if (m_d[i]->idx.col_name == column_name) {
          return i;
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

    sizetype size() {
      return m_size;
    }

    sizetype size() const {
      return m_size;
    }

    sizetype index() {
      return m_d[0]->idx.row_idx;
    }

    sizetype index() const {
      return m_d[0]->idx.row_idx;
    }

    String name() {
      return m_d[0]->idx.row_name;
    }

    const String name() const {
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
public:
    using ValueType        = Row<T>;
    using RowGroupIterator = Iterator<RowGroup>;

    RowGroup(const DataFrame<T>* df) : logger(this), logging_context(df->logger.context) {
      m_size     = df->row_count();
      m_d        = new Row<T>[m_size];
      m_row_size = df->row_size();
      for (auto row_iter = df->iter_rows(); row_iter < df->end(); row_iter++) {
        m_d[row_iter.current_row_idx()] = row_iter.current_row();
      }
      logger.with_context(logging_context);
    }

    RowGroup(const RowGroup& other)
        : logger(this),
          logging_context(other.logging_context),
          m_size(other.m_size),
          m_d(new Row<T>[m_size]),
          m_row_size(other.m_row_size) {
      for (sizetype i = 0; i < m_size; i++) {
        m_d[i] = other.m_d[i];
      }
      logger.with_context(logging_context);
    }

    RowGroup(RowGroup&& other)
        : logger(this),
          logging_context(other.logging_context),
          m_size(other.m_size),
          m_d(other.m_d),
          m_row_size(other.m_row_size) {
      logger.with_context(logging_context);
      other.m_d = nullptr;
    }

    ~RowGroup() {
      delete[] m_d;
    }

    RowGroup operator=(const RowGroup& other) = delete;
    RowGroup operator=(RowGroup&& other) {
      if (this != &other) {
        logging_context = other.logging_context;
        m_size          = other.m_size;
        m_d             = other.m_d;
        m_row_size      = other.m_row_size;
        other.m_d       = nullptr;
      }
      return *this;
    }

    Row<T>& operator[](const sizetype& idx) {
      return m_d[idx];
    }

    const Row<T>& operator[](const sizetype& idx) const {
      return m_d[idx];
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    RowGroup& sort(const String& column_name, bool ascending = false) {
      sizetype col_idx = m_d[0].column_index(column_name);
      sizetype insert_idx;
      bool     found;

      std::function<bool(const Cell<T>*, const Cell<T>*)> sort_condition;
      if (ascending) {
        sort_condition = [](const Cell<T>* current_value, const Cell<T>* sorted_value) {
          return current_value->value < sorted_value->value;
        };
      } else {
        sort_condition = [](const Cell<T>* current_value, const Cell<T>* sorted_value) {
          return current_value->value > sorted_value->value;
        };
      }

      for (sizetype idx = 0; idx < m_size; idx++) {
        found      = false;
        insert_idx = 0;
        for (sizetype sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
          if (sort_condition(m_d[idx][col_idx], m_d[sorted_idx][col_idx])) {
            found      = true;
            insert_idx = sorted_idx;
            break;
          }
        }
        if (found) {
          Row<T> swap_item = std::move(m_d[idx]);
          for (sizetype i = idx - 1; i >= insert_idx; i--) {
            m_d[i + 1] = std::move(m_d[i]);
            // avoid sizetype underflow
            if (i == 0) {
              break;
            }
          }
          m_d[insert_idx] = std::move(swap_item);
        }
      }

      return *this;
    }

    sizetype size() {
      return m_size;
    }

    sizetype size() const {
      return m_size;
    }

    sizetype row_size() {
      return m_row_size;
    }

    sizetype row_size() const {
      return m_row_size;
    }

    Row<T>& at(sizetype index) {
      return m_d[index];
    }

    const Row<T>& at(sizetype index) const {
      return m_d[index];
    }

    RowGroupIterator begin() {
      return RowGroupIterator(m_d);
    }

    RowGroupIterator end() {
      return RowGroupIterator(m_d + m_size);
    }

    void log(int range = 0) {
      logger.log(range);
    }

    RowGroup_Logger<T> logger;

private:
    LoggingContext<T> logging_context;
    sizetype          m_size;
    Row<T>*           m_d;
    sizetype          m_row_size;
  };

} // namespace df

#endif // COLUMN_SERIES_H
