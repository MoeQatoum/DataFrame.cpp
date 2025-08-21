
#ifndef DATA_FRAME_ROW_H
#define DATA_FRAME_ROW_H

#include "df_common.hpp"

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
    class RowGroup_Logger;

    template<typename T>
    class LoggingContext;

    template<typename T>
    class Row {
      public:
        using data_type          = T;
        using value_type         = typename DataFrame<data_type>::value_type*;
        using const_value_type   = const value_type;
        using pointer_type       = value_type*;
        using const_pointer_type = const pointer_type;
        using dataframe_iterator = typename DataFrame<data_type>::iterator;
        using iterator           = Iterator<Row>;

        Row() : m_size(0), m_d(nullptr) {
        }

        Row(dataframe_iterator df_begin, std::size_t row_idx, std::size_t row_size) {
            m_size = row_size;
            m_d    = new value_type[m_size];

            dataframe_iterator row_begin = df_begin + (row_idx * row_size);
            for (std::size_t idx = 0; idx < row_size; idx++) {
                m_d[idx] = &(row_begin + idx);
            }
        }

        Row(const Row& other) : m_size(other.m_size), m_d(new value_type[other.m_size]) {
            // for (std::size_t i = 0; i < m_size; i++) {
            //     m_d[i] = other.m_d[i];
            // }
            std::copy(other.begin(), other.end(), m_d);
        }

        Row(Row&& other) : m_size(other.m_size), m_d(other.m_d) {
            other.m_d    = nullptr;
            other.m_size = 0;
        }

        ~Row() {
            delete[] m_d;
        }

        // replace values directly - not recommended
        // Row& operator()(const Row& other) {
        //   FORCED_ASSERT(m_size == other.m_size, "assignment operation on nonmatching size objects");
        //   for (std::size_t i = 0; i < m_size; i++) {
        //     m_d[i]->value = other.m_d[i]->value;
        //   }
        //   return *this;
        // }

        value_type& operator[](const std::size_t& idx) {
            return m_d[idx];
        }

        const value_type& operator[](const std::size_t& idx) const {
            return m_d[idx];
        }

        value_type& operator[](const std::string& col_name) {
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i]->idx.col_name == col_name) { return m_d[i]; }
            }

            std::cerr << "col name not found.";
            abort();
        }

        const_value_type& operator[](const std::string& col_name) const {
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i]->idx.col_name == col_name) { return m_d[i]; }
            }

            std::cerr << "col name not found.";
            abort();
        }

        Row& operator=(const Row& rhs) {
            if (this != &rhs) {
                if (is_null()) {
                    FORCED_ASSERT(m_d == nullptr, "m_d supposed to be null pointer, something is wrong");
                    m_size = rhs.m_size;
                    m_d    = new value_type[m_size];
                } else {
                    FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
                }
                for (std::size_t i = 0; i < m_size; i++) {
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

        Row& operator=(const Series<T>& rhs) const {
            FORCED_ASSERT(m_d != nullptr, "m_d is not supposed to be null pointer, something is wrong");
            FORCED_ASSERT(m_size == rhs.size(), "assignment operation on nonmatching size objects");
            for (std::size_t i = 0; i < m_size; i++) {
                m_d[i]->value = rhs[i];
            }
            return *this;
        }

        // Series<T> copy_data() {
        //   Series<T> data(m_size);
        //   for (std::size_t i = 0; i < m_size; i++) {
        //     data[i] = m_d[i]->value;
        //   }
        //   return data;
        // }

        Series<T> to_series() const {
            Series<T> data(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                data[i] = m_d[i]->value;
            }
            return data;
        }

        value_type& at_column(const std::string& col_name) {
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i]->idx.col_name == col_name) { return m_d[i]; }
            }

            std::cerr << "col name not found.";
            abort();
        }

        const value_type& at_column(const std::string& col_name) const {
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i]->idx.col_name == col_name) { return m_d[i]; }
            }

            std::cerr << "col name not found.";
            abort();
        }

        std::size_t column_index_of(const std::string& column_name) const {
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i]->idx.col_name == column_name) { return i; }
            }

            std::cerr << "col name not found.";
            abort();
        }

        friend std::ostream& operator<<(std::ostream& os, const Row& row) {
            os << "Row(addr: " << &row << ", size: " << row.m_size << ", type: " << typeid(T).name() << ")";
            return os;
        }

        std::size_t size() const {
            return m_size;
        }

        std::size_t index() const {
            return m_d[0]->idx.row_idx;
        }

        const std::string name() const {
            return m_d[0]->idx.row_name;
        }

        iterator begin() const {
            return iterator(m_d);
        }

        iterator end() const {
            return iterator(m_d + m_size);
        }

        bool is_null() const {
            return (m_d == nullptr) && (m_size == 0);
        }

      private:
        std::size_t m_size;
        value_type* m_d;
    };

    template<typename T>
    class RowGroup {
      public:
        using value_type       = Row<T>;
        using const_value_type = const Row<T>;
        using iterator         = Iterator<RowGroup>;

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
            for (std::size_t i = 0; i < m_size; i++) {
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

        RowGroup operator=(const RowGroup& other) = delete; // why?

        RowGroup& operator=(RowGroup&& other) {
            if (this != &other) {
                logging_context = other.logging_context;
                m_size          = other.m_size;
                m_d             = other.m_d;
                m_row_size      = other.m_row_size;
                other.m_d       = nullptr;
            }
            return *this;
        }

        Row<T>& operator[](const std::size_t& idx) {
            return m_d[idx];
        }

        const Row<T>& operator[](const std::size_t& idx) const {
            return m_d[idx];
        }

        // TODO: should this function be marked as const ?
        template<typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>, bool>>
        RowGroup& sort(const std::string& column_name, const bool ascending = false) {
            std::size_t col_idx = m_d[0].column_index_of(column_name);

            std::sort(m_d, m_d + m_size, [&](const Row<T>& a, const Row<T>& b) {
                const T& a_val = a[col_idx]->value;
                const T& b_val = b[col_idx]->value;
                return ascending ? (a_val < b_val) : (a_val > b_val);
            });

            return *this;
        }

        std::size_t size() const {
            return m_size;
        }

        std::size_t row_size() const {
            return m_row_size;
        }

        Row<T>& at(std::size_t index) {
            return m_d[index];
        }

        const Row<T>& at(std::size_t index) const {
            return m_d[index];
        }

        iterator begin() const {
            return iterator(m_d);
        }

        iterator end() const {
            return iterator(m_d + m_size);
        }

        void log(int range = 0) const {
            logger.log(range);
        }

        RowGroup_Logger<T> logger;

      private:
        LoggingContext<T> logging_context;
        std::size_t       m_size;
        value_type*       m_d;
        std::size_t       m_row_size;
    };

} // namespace df

#endif // DATA_FRAME_ROW_H
