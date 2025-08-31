
#ifndef DATA_FRAME_ROW_H
#define DATA_FRAME_ROW_H

#include "df_common.hpp"

#include "df_base_iterator.hpp"
#include "df_series.hpp"

namespace df {

    template<typename T>
    class DataFrame;

    template<typename CellType>
    class Row {
      public:
        using data_type    = typename CellType::data_type;
        using value_type   = CellType*;
        using pointer_type = value_type*;
        using dataframe_iterator
        = std::conditional_t<std::is_const_v<CellType>, typename DataFrame<data_type>::const_iterator, typename DataFrame<data_type>::iterator>;
        using iterator = BaseIterator<Row<std::remove_const_t<CellType>>, std::is_const_v<CellType>>;

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

        /* TODO: need to check here */ const value_type& operator[](const std::string& col_name) const {
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

        Row& operator=(const Series<CellType>& rhs) const {
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

        Series<CellType> to_series() const {
            Series<CellType> data(m_size);
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
            os << "Row(addr: " << &row << ", size: " << row.m_size << ", type: " << typeid(CellType).name() << ")";
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

} // namespace df

#endif // DATA_FRAME_ROW_H
