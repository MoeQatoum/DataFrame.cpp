#ifndef DATA_FRAME_IMPL_H
#define DATA_FRAME_IMPL_H

#include "df_common.hpp"

#include "df_cell.hpp"
#include "df_column.hpp"
#include "df_logger.hpp"
#include "df_row.hpp"
#include "df_series.hpp"

namespace df {

    template<typename T>
    class DataFrame;

    template<typename Iterable>
    class Iterator {
        using value_type       = typename Iterable::value_type;
        using const_value_type = typename Iterable::const_value_type;

        template<typename>
        friend class RowIterator;

        template<typename>
        friend class ColumnIterator;

      public:
        Iterator(value_type* p_data) : m_d(p_data) {
        }

        Iterator(const Iterator& other) : m_d(other.m_d) {
        }

        ~Iterator() {
        }

        template<typename ValueType1, typename = std::enable_if_t<std::is_convertible_v<value_type, ValueType1>, bool>>
        operator ValueType1() const {
            return m_d;
        }

        Iterator& operator=(const Iterator& other) {
            if (this != &other) { m_d = other.m_d; }
            return *this;
        }

        value_type* operator&() {
            return m_d;
        }

        const value_type* operator&() const {
            return m_d;
        }

        value_type& operator*() {
            return *m_d;
        }

        const value_type& operator*() const {
            return *m_d;
        }

        value_type* operator->() {
            return m_d;
        }

        const value_type* operator->() const {
            return m_d;
        }

        friend Iterator operator+(const std::size_t& lhs, const Iterator& rhs) {
            Iterator tmp{rhs};
            tmp += lhs;
            return tmp;
        }

        friend Iterator operator+(const Iterator& lhs, const std::size_t& rhs) {
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

        Iterator& operator+=(std::size_t off) {
            m_d += off;
            return *this;
        }

        friend ptrdiff_t operator-(const Iterator& lhs, const Iterator& rhs) {
            return lhs.m_d - rhs.m_d;
        }

        friend Iterator operator-(const Iterator& lhs, const std::size_t& rhs) {
            Iterator tmp{lhs};
            tmp -= rhs;
            return tmp;
        }

        Iterator& operator--() {
            m_d--;
            return *this;
        }

        Iterator operator--(int) {
            Iterator tmp{*this};
            --(*this);
            return tmp;
        }

        Iterator& operator-=(std::size_t off) {
            m_d -= off;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return other.m_d == this->m_d;
        }

        bool operator!=(const Iterator& other) const {
            return this->m_d != other.m_d;
        }

        friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
            return lhs.m_d < rhs.m_d;
        }

        friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
            return lhs.m_d > rhs.m_d;
        }

        friend std::ostream& operator<<(std::ostream& os, const Iterator& itr) {
            os << "Iterator(current addr: 0x" << std::hex << reinterpret_cast<void*>(itr.m_d) << std::dec << ")";
            return os;
        }

      private:
        value_type* m_d;
    };

    template<typename dataframe>
    class RowIterator {

      public:
        using iterator = typename dataframe::iterator;
        using row      = Row<typename dataframe::data_type>;

        RowIterator(const iterator df_begin, std::size_t row_size) : m_df_begin(df_begin), m_row_size(row_size), m_current_row_idx(0) {
        }

        RowIterator(const RowIterator& other)
            : m_df_begin(other.m_df_begin),
              m_row_size(other.m_row_size),
              m_current_row_idx(other.m_current_row_idx) {
        }

        ~RowIterator() {
        }

        // Returns the current row explicitly; use when you want to access the row at the current iterator position.
        row current_row() {
            return row(m_df_begin, m_current_row_idx, m_row_size);
        }

        row current_row() const {
            return row(m_df_begin, m_current_row_idx, m_row_size);
        }

        std::size_t current_row_idx() const {
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

        // Implicit conversion to Row; use for syntactic convenience (e.g., auto r = *it).
        operator row() {
            return row(m_df_begin, m_current_row_idx, m_row_size);
        }

        // operator ConstRow() const {
        //     return ConstRow(m_df_begin, m_current_row_idx, m_row_size);
        // }

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

        friend bool operator<(const RowIterator& lhs, const iterator& rhs) {
            return (lhs.m_df_begin + (lhs.m_current_row_idx * lhs.m_row_size)) < rhs;
        }

        friend bool operator<(const iterator& lhs, const RowIterator& rhs) {
            return lhs < (rhs.m_df_begin + (rhs.m_current_row_idx * rhs.m_row_size));
        }

        friend bool operator>(const RowIterator& lhs, const iterator& rhs) {
            return (lhs.m_df_begin + (lhs.m_current_row_idx * lhs.m_row_size)) > rhs;
        }

        friend bool operator>(const iterator& lhs, const RowIterator& rhs) {
            return lhs > (rhs.m_df_begin + (rhs.m_current_row_idx * rhs.m_row_size));
        }

        friend bool operator<(const RowIterator& lhs, const std::size_t& rhs) {
            return lhs.m_current_row_idx < rhs;
        }

        friend bool operator<(const std::size_t& lhs, const RowIterator& rhs) {
            return lhs < rhs.m_current_row_idx;
        }

        friend bool operator>(const RowIterator& lhs, const std::size_t& rhs) {
            return lhs.m_current_row_idx > rhs;
        }

        friend bool operator>(const std::size_t& lhs, const RowIterator& rhs) {
            return lhs > rhs.m_current_row_idx;
        }

      private:
        iterator    m_df_begin;
        std::size_t m_row_size;
        std::size_t m_current_row_idx;
    };

    template<typename dataframe>
    class ColumnIterator {
        using dataframe_iterator = typename dataframe::iterator;
        using column             = Column<typename dataframe::data_type>;

      public:
        ColumnIterator(dataframe_iterator df_begin, std::size_t col_size, std::size_t row_size)
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

        column current_col() {
            return column(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
        }

        column current_col() const {
            return column(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
        }

        std::size_t current_col_idx() const {
            return m_current_col_idx;
        }

        operator column() {
            return column(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
        }

        // operator ConstColumn() const {
        //     return ConstColumn(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
        // }

        ColumnIterator& operator=(const ColumnIterator& other) {
            if (this != &other) {
                m_df_begin        = other.m_df_begin;
                m_col_size        = other.m_col_size;
                m_row_size        = other.m_row_size;
                m_current_col_idx = other.m_current_col_idx;
            }
            return *this;
        }

        ColumnIterator operator+(const std::size_t& off) const {
            ColumnIterator tmp{*this};
            if (tmp.m_current_col_idx + off >= tmp.m_col_size) { throw std::out_of_range("ColumnIterator::operator+ out of range"); }
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

        ColumnIterator& operator+=(std::size_t off) {
            m_current_col_idx += off;
            return *this;
        }

        friend bool operator<(const ColumnIterator& lhs, const dataframe_iterator& rhs) {
            return (lhs.m_df_begin + (lhs.m_current_col_idx + (lhs.m_row_size * (lhs.m_col_size - 1)))) < rhs.m_d;
        }

        friend bool operator<(const dataframe_iterator& lhs, const ColumnIterator& rhs) {
            return lhs.m_d < (rhs.m_df_begin + (rhs.m_current_col_idx + (rhs.m_row_size * rhs.m_col_size)));
        }

        friend bool operator>(const ColumnIterator& lhs, const dataframe_iterator& rhs) {
            return (lhs.m_df_begin + (lhs.m_current_col_idx + (lhs.m_row_size * lhs.m_col_size))) > rhs.m_d;
        }

        friend bool operator>(const dataframe_iterator& lhs, const ColumnIterator& rhs) {
            return lhs.m_d > (rhs.m_df_begin + (rhs.m_current_col_idx + (rhs.m_row_size * rhs.m_col_size)));
        }

        friend bool operator<(const ColumnIterator& lhs, const std::size_t& rhs) {
            return lhs.m_current_col_idx < rhs;
        }

        friend bool operator<(const std::size_t& lhs, const ColumnIterator& rhs) {
            return lhs < rhs.m_current_col_idx;
        }

        friend bool operator>(const ColumnIterator& lhs, const std::size_t& rhs) {
            return lhs.m_current_col_idx > rhs;
        }

        friend bool operator>(const std::size_t& lhs, const ColumnIterator& rhs) {
            return lhs > rhs.m_current_col_idx;
        }

      private:
        dataframe_iterator m_df_begin;
        std::size_t        m_col_size;
        std::size_t        m_row_size;
        std::size_t        m_current_col_idx;
    };

    struct Shape {
        std::size_t          col_count;
        std::size_t          row_count;
        friend std::ostream& operator<<(std::ostream& os, const Shape& shape) {
            os << "Shape(c: " << shape.col_count << ", r: " << shape.row_count << ")";
            return os;
        }
    };

    template<typename T>
    class DataFrame {
        friend class DF_Logger<T>;

      public:
        using data_type          = T;
        using value_type         = Cell<data_type>;
        using const_value_type   = const Cell<data_type>;
        using pointer_type       = Cell<data_type>*;
        using const_pointer_type = const Cell<data_type>*;
        using iterator           = Iterator<DataFrame<T>>;
        using row_iterator       = RowIterator<DataFrame<T>>;
        using column_iterator    = ColumnIterator<DataFrame<T>>;
        using dataframe_logger   = DF_Logger<T>;

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

        DataFrame(const std::vector<std ::string>& col_names, const std::vector<std::string>& row_names) : logger(this), logging_context({}) {
            m_col_count    = static_cast<std::size_t>(col_names.size());
            m_row_count    = static_cast<std::size_t>(row_names.size());
            m_col_size     = m_row_count;
            m_row_size     = m_col_count;
            m_current_size = m_col_count * m_row_count;
            m_d            = new value_type[m_current_size];

            for (std::size_t i = 0; i < m_col_count; i++) {
                m_col_idx_map.insert({col_names[i], i});
                if (col_names[i].size() > logging_context.max_col_name_size) { logging_context.max_col_name_size = col_names[i].size(); }
            }

            for (std::size_t i = 0; i < m_row_count; i++) {
                m_row_idx_map.insert({row_names[i], i});
                if (row_names[i].size() > logging_context.max_row_name_size) { logging_context.max_row_name_size = row_names[i].size(); }
            }

            // 00 01 02 03 04
            // 05 06 07 08 09
            // 10 11 12 13 14
            // 15 16 17 18 19

            // 00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18 19

            for (std::size_t i = 0; i < m_current_size; i++) {
                m_d[i].idx.global_idx = i;

                m_d[i].idx.col_idx  = i % m_col_count;
                m_d[i].idx.col_name = col_names[i % m_col_count];

                m_d[i].idx.row_idx  = i / m_col_count;
                m_d[i].idx.row_name = row_names[i / m_col_count];
            }

            logger.with_context(logging_context);
        }

        DataFrame(const RowGroup<data_type>& rows) : logger(this), logging_context({}) {
            m_col_count    = rows.row_size();
            m_row_count    = rows.size();
            m_col_size     = m_row_count;
            m_row_size     = m_col_count;
            m_current_size = m_col_count * m_row_count;
            m_d            = new value_type[m_current_size];

            std::size_t idx = 0;
            for (std::size_t row_idx = 0; row_idx < m_row_count; row_idx++) {
                for (std::size_t i = 0; i < m_col_count; i++) {
                    m_d[idx] = rows[row_idx][i];
                    idx++;
                }
            }

            for (std::size_t i = 0; i < m_col_count; i++) {
                m_col_idx_map.insert({m_d[i].idx.col_name, i});
                if (m_d[i].idx.col_name.size() > logging_context.max_col_name_size) {
                    logging_context.max_col_name_size = m_d[i].idx.col_name.size();
                }
            }

            for (std::size_t i = 0; i < m_current_size; i += m_row_size) {
                m_row_idx_map.insert({m_d[i].idx.row_name, i});
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
              m_d(new value_type[m_current_size]),
              logging_context(other.logging_context) {
            for (std::size_t idx = 0; idx < m_current_size; idx++) {
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
                    m_d             = new value_type[other.m_current_size];
                    for (std::size_t idx = 0; idx < m_current_size; idx++) {
                        m_d[idx] = other.m_d[idx];
                    }
                } else {
                    FORCED_ASSERT(m_current_size == other.m_current_size,
                                  "Copy assignment operator on DataFrame with other "
                                  "nonmatching size.");
                    m_col_idx_map   = other.m_col_idx_map;
                    m_row_idx_map   = other.m_row_idx_map;
                    m_current_size  = other.m_current_size;
                    m_col_size      = other.m_col_size;
                    m_col_count     = other.m_col_count;
                    m_row_size      = other.m_row_size;
                    m_row_count     = other.m_row_count;
                    logging_context = other.logging_context;
                    for (std::size_t idx = 0; idx < m_current_size; idx++) {
                        m_d[idx] = other.m_d[idx];
                    }
                }
            }
            return *this;
        }

        value_type& operator[](const std::size_t& idx) {
            return m_d[idx];
        }

        const_value_type& operator[](const std::size_t& idx) const {
            return m_d[idx];
        }

        Column<data_type> operator[](const std::string& col_name) {
            return column(col_name);
        }

        Column<data_type> operator[](const std::string& col_name) const {
            return column(col_name);
        }

        value_type& operator[](const std::size_t& col_idx, const std::size_t& row_idx) {
            if (col_idx >= m_col_size) { throw std::out_of_range("column index out of range"); };
            if (row_idx >= m_row_size) { throw std::out_of_range("row index out of range"); };
            return *(m_d + ((m_row_size * row_idx) + col_idx));
        }

        const_value_type& operator[](const std::size_t& col_idx, const std::size_t& row_idx) const {
            if (col_idx >= m_col_size) { throw std::out_of_range("column index out of range"); };
            if (row_idx >= m_row_size) { throw std::out_of_range("row index out of range"); };
            return *(m_d + ((m_row_size * row_idx) + col_idx));
        }

        value_type& operator[](const std::string& col_name, const std::string& row_name) {
            if (!m_col_idx_map.contains(col_name) || !m_row_idx_map.contains(row_name)) {
                throw std::out_of_range("Column/row name not found: " + col_name + "/" + row_name);
            }
            std::size_t col_idx = m_col_idx_map[col_name];
            std::size_t row_idx = m_row_idx_map[row_name];
            return *(m_d + ((m_row_size * row_idx) + col_idx));
        }

        const_value_type& operator[](const std::string& col_name, const std::string& row_name) const {
            if (!m_col_idx_map.contains(col_name) && !m_row_idx_map.contains(row_name)) {
                throw std::out_of_range("Column/row name not found: " + col_name + "/" + row_name);
            }
            std::size_t col_idx = m_col_idx_map.at(col_name);
            std::size_t row_idx = m_row_idx_map.at(row_name);
            return *(m_d + ((m_row_size * row_idx) + col_idx));
        }

        DataFrame copy() {
            return DataFrame(*this);
        }

        std::size_t get_col_idx(std::string col) const {
            return m_col_idx_map.at(col);
        }

        std::string get_col_name(std::size_t col_idx) const {
            for (const auto& [col_name, idx] : m_col_idx_map) {
                if (idx == col_idx) { return col_name; }
            }
            throw std::logic_error("Column index not found: " + std::to_string(col_idx));
        }

        std::size_t get_row_idx(std::string row) const {
            return m_row_idx_map.at(row);
        }

        std::string get_row_name(std::size_t row_idx) const {
            for (const auto& [row_name, idx] : m_row_idx_map) {
                if (idx == row_idx) { return row_name; }
            }
            throw std::logic_error("Row index not found: " + std::to_string(row_idx));
        }

        value_type& at(std::size_t idx) {
            return m_d[idx];
        }

        const_value_type& at(std::size_t idx) const {
            return m_d[idx];
        }

        std::size_t size() const {
            return m_current_size;
        }

        std::size_t col_size() const {
            return m_col_size;
        }

        std::size_t col_count() const {
            return m_col_count;
        }

        std::size_t row_size() const {
            return m_row_size;
        }

        std::size_t row_count() const {
            return m_row_count;
        }

        Shape shape() const {
            return {.col_count = m_col_count, .row_count = m_row_count};
        }

        Column<data_type> column(std::size_t col_idx) {
            return {begin() + col_idx, m_col_size, m_row_size};
        }

        Column<data_type> column(std::size_t col_idx) const {
            return {begin() + col_idx, m_col_size, m_row_size};
        }

        Column<data_type> column(std::string col_name) {
            return {begin() + get_col_idx(col_name), m_col_size, m_row_size};
        }

        Column<data_type> column(std::string col_name) const {
            return {begin() + get_col_idx(col_name), m_col_size, m_row_size};
        }

        Row<data_type> row(std::size_t row_idx) {
            return {begin(), row_idx, m_row_size};
        }

        Row<data_type> row(std::size_t row_idx) const {
            return {begin(), row_idx, m_row_size};
        }

        Row<data_type> row(std::string row_name) {
            return {begin(), get_row_idx(row_name), m_row_size};
        }

        Row<data_type> row(std::string row_name) const {
            return {begin(), get_row_idx(row_name), m_row_size};
        }

        RowGroup<data_type> rows() {
            return {this};
        }

        RowGroup<data_type> rows() const {
            return {this};
        }

        iterator begin() {
            return iterator(m_d);
        }

        iterator end() {
            return iterator(m_d + m_current_size);
        }

        iterator begin() const {
            return iterator(m_d);
        }

        iterator end() const {
            return iterator(m_d + m_current_size);
        }

        column_iterator iter_cols() {
            return column_iterator(begin(), m_col_size, m_row_size);
        }

        column_iterator iter_cols() const {
            return column_iterator(begin(), m_col_size, m_row_size);
        }

        row_iterator iter_rows() {
            return row_iterator(begin(), m_row_size);
        }

        row_iterator iter_rows() const {
            return row_iterator(begin(), m_row_size);
        }

        bool is_null() const {
            return (m_d == nullptr) && (m_col_count == 0) && (m_col_size == 0) && (m_row_size == 0) && (m_row_count == 0) && (m_current_size == 0);
        }

        template<std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        RowGroup<data_type> sort(std::string column_name, bool ascending = false) {
            return RowGroup(this).sort(column_name, ascending);
        }

        template<std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        void log(int range = 0) {
            logger.log(range);
        }

        dataframe_logger logger;

      private:
        std::map<std::string, std::size_t> m_col_idx_map;
        std::map<std::string, std::size_t> m_row_idx_map;
        std::size_t                        m_current_size;
        std::size_t                        m_col_size;
        std::size_t                        m_col_count;
        std::size_t                        m_row_size;
        std::size_t                        m_row_count;
        value_type*                        m_d;

        LoggingContext<data_type> logging_context;
    };

} // namespace df
#endif // DATA_FRAME_IMPL_H
