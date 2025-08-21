#ifndef DATA_FRAME_COLUMN_H
#define DATA_FRAME_COLUMN_H

#include "df_common.hpp"
#include "df_series.hpp"

namespace df {

    template<typename Iterable>
    class Iterator;

    template<typename T>
    class DataFrame;

    template<typename T>
    class Column {
      public:
        using data_type          = T;
        using value_type         = typename DataFrame<data_type>::pointer_type;
        using const_value_type   = typename DataFrame<data_type>::const_pointer_type;
        using dataframe_iterator = typename DataFrame<data_type>::iterator;
        using iterator           = Iterator<Column>;

        Column() : m_size(0), m_stride(0), m_d(nullptr) {
        }

        Column(dataframe_iterator col_begin, std::size_t col_size, std::size_t stride) {
            m_size   = col_size;
            m_stride = stride;
            m_d      = new value_type[col_size];

            for (std::size_t idx = 0; idx < col_size; idx++) {
                m_d[idx] = &(col_begin + (idx * stride));
            }
        }

        Column(const Column& other) = delete;

        ~Column() {
            delete[] m_d;
        }

        // replace values directly - not recommended
        // Column& operator()(const Column& other) {
        //   FORCED_ASSERT(m_size == other.m_size, "assignment operation on nonmatching size objects");
        //   for (std::size_t i = 0; i < m_size; i++) {
        //     m_d[i]->value = other.m_d[i]->value;
        //   }
        //   return *this;
        // }

        value_type& operator[](const std::size_t idx) {
            return m_d[idx];
        }

        const value_type& operator[](const std::size_t idx) const {
            return m_d[idx];
        }

        value_type& operator[](const std::string& row_name) {
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i]->idx.row_name == row_name) { return m_d[i]; }
            }

            std::cerr << "row name not found.";
            abort();
        }

        const value_type& operator[](const std::string& row_name) const {
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i]->idx.row_name == row_name) { return m_d[i]; }
            }

            std::cerr << "row name not found.";
            abort();
        }

        Column& operator=(const Column& rhs) {
            if (this != &rhs) {
                if (is_null()) {
                    FORCED_ASSERT(m_d == nullptr, "m_d supposed to be null pointer, something is wrong");
                    m_size = rhs.m_size;
                    m_d    = new value_type[m_size];
                } else {
                    FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
                }
                for (std::size_t i = 0; i < m_size; i++) {
                    m_d[i] = rhs[i];
                }
            }
            return *this;
        }

        Column& operator=(Column&& rhs) {
            FORCED_ASSERT(m_size == rhs.m_size, "assignment operation on nonmatching size objects");
            m_size  = rhs.m_size;
            m_d     = rhs.m_d;
            rhs.m_d = nullptr;
            return *this;
        }

        Column& operator=(const Series<T>& rhs) {
            FORCED_ASSERT(m_d != nullptr, "m_d is not supposed to be null pointer, something is wrong");
            FORCED_ASSERT(m_size == rhs.size(), "assignment operation on nonmatching size objects");
            for (std::size_t i = 0; i < m_size; i++) {
                m_d[i]->value = rhs[i];
            }
            return *this;
        }

        // comparaison operators
        Series<bool> operator==(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i]->value == rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator==(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value == rhs[i]);
            }
            return temp;
        }

        friend Series<bool> operator==(const Series<T>& lhs, const Column& rhs) {
            return rhs == lhs;
        }

        friend Series<bool> operator==(const Column& lhs, const T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value == rhs);
            }
            return temp;
        }

        friend Series<bool> operator==(const T& lhs, const Column& rhs) {
            return rhs == lhs;
        }

        Series<bool> operator!=(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i]->value != rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator!=(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value != rhs[i]);
            }
            return temp;
        }

        friend Series<bool> operator!=(const Series<T>& lhs, const Column& rhs) {
            return rhs != lhs;
        }

        friend Series<bool> operator!=(const Column& lhs, const T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value != rhs);
            }
            return temp;
        }

        friend Series<bool> operator!=(const T& lhs, const Column& rhs) {
            return rhs != lhs;
        }

        Series<bool> operator>=(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i]->value >= rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator>=(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value >= rhs[i]);
            }
            return temp;
        }

        friend Series<bool> operator>=(const Series<T>& lhs, const Column& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs[i] >= rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator>=(const Column& lhs, const T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value >= rhs);
            }
            return temp;
        }

        friend Series<bool> operator>=(const T& lhs, const Column& rhs) {
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs >= rhs[i]->value);
            }
            return temp;
        }

        Series<bool> operator<=(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i]->value <= rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator<=(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value <= rhs[i]);
            }
            return temp;
        }

        friend Series<bool> operator<=(const Series<T>& lhs, const Column& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs[i] <= rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator<=(const Column& lhs, const T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value <= rhs);
            }
            return temp;
        }

        friend Series<bool> operator<=(const T& lhs, const Column& rhs) {
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs <= rhs[i]->value);
            }
            return temp;
        }

        Series<bool> operator<(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i]->value < rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator<(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value < rhs[i]);
            }
            return temp;
        }

        friend Series<bool> operator<(const Series<T>& lhs, const Column& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs[i] < rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator<(const Column& lhs, const T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value < rhs);
            }
            return temp;
        }

        friend Series<bool> operator<(const T& lhs, const Column& rhs) {
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs < rhs[i]->value);
            }
            return temp;
        }

        Series<bool> operator>(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i]->value > rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator>(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value > rhs[i]);
            }
            return temp;
        }

        friend Series<bool> operator>(const Series<T>& lhs, const Column& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "comparaison operation on nonmatching size objects");
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs[i] > rhs[i]->value);
            }
            return temp;
        }

        friend Series<bool> operator>(const Column& lhs, const T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i]->value > rhs);
            }
            return temp;
        }

        friend Series<bool> operator>(const T& lhs, const Column& rhs) {
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs > rhs[i]->value);
            }
            return temp;
        }

        // arithmetic operators
        Series<T> operator+(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series<T> res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_d[i]->value + rhs[i]->value;
            }
            return res;
        }

        friend Series<T> operator+(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "arithmetic operation on nonmatching size objects");
            Series<T> res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i]->value + rhs[i];
            }
            return res;
        }

        friend Series<T> operator+(const Series<T>& lhs, const Column& rhs) {
            return rhs + lhs;
        }

        // Column& operator+=(const T& rhs) {
        //   for (std::size_t i = 0; i < m_size; i++) {
        //     m_d[i]->value += rhs;
        //   }
        //   return *this;
        // }

        // Column& operator++() {
        //   for (std::size_t i = 0; i < m_size; i++) {
        //     ++m_d[i]->value;
        //   }
        //   return *this;
        // }

        Series<T> operator*(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series<T> res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_d[i]->value * rhs[i]->value;
            }
            return res;
        }

        friend Series<T> operator*(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "arithmetic operation on nonmatching size objects");
            Series<T> res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i]->value * rhs[i];
            }
            return res;
        }

        friend Series<T> operator*(const Series<T>& lhs, const Column& rhs) {
            return rhs * lhs;
        }

        Series<T> operator-(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series<T> res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_d[i]->value - rhs[i]->value;
            }
            return res;
        }

        friend Series<T> operator-(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "arithmetic operation on nonmatching size objects");
            Series<T> res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i]->value - rhs[i];
            }
            return res;
        }

        friend Series<T> operator-(const Series<T>& lhs, const Column& rhs) {
            FORCED_ASSERT(lhs.size() == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series<T> res(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                res[i] = lhs[i] - rhs[i]->value;
            }
            return res;
        }

        // Column& operator-=(const T& rhs) {
        //   for (std::size_t i = 0; i < m_size; i++) {
        //     m_d[i]->value -= rhs;
        //   }
        //   return *this;
        // }

        // Column& operator--() {
        //   for (std::size_t i = 0; i < m_size; i++) {
        //     --m_d[i]->value;
        //   }
        //   return *this;
        // }

        Series<T> operator/(const Column& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series<T> res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_d[i]->value / rhs[i]->value;
            }
            return res;
        }

        friend Series<T> operator/(const Column& lhs, const Series<T>& rhs) {
            FORCED_ASSERT(lhs.m_size == rhs.size(), "arithmetic operation on nonmatching size objects");
            Series<T> res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i]->value / rhs[i];
            }
            return res;
        }

        friend Series<T> operator/(const Series<T>& lhs, const Column& rhs) {
            FORCED_ASSERT(lhs.size() == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series<T> res(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                res[i] = lhs[i] / rhs[i]->value;
            }
            return res;
        }

        value_type& at_row(const std::string& row_name) {
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i]->idx.row_name == row_name) { return m_d[i]; }
            }

            std::cerr << "row name not found.";
            abort();
        }

        const value_type& at_row(const std::string& row_name) const {
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i]->idx.row_name == row_name) { return m_d[i]; }
            }

            std::cerr << "row name not found.";
            abort();
        }

        template<typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>, bool>>
        T max() const {
            T temp = m_d[0]->value;
            for (std::size_t i = 1; i < m_size; ++i) {
                if (m_d[i]->value > temp) { temp = m_d[i]->value; }
            }
            return temp;
        }

        template<typename U = T, typename = std::enable_if_t<std::is_arithmetic_v<U>, bool>>
        T min() const {
            T temp = m_d[0]->value;
            for (std::size_t i = 1; i < m_size; ++i) {
                if (m_d[i]->value < temp) { temp = m_d[i]->value; }
            }
            return temp;
        }

        Series<T> to_series() const {
            Series<T> data(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                data[i] = m_d[i]->value;
            }
            return data;
        }

        friend std::ostream& operator<<(std::ostream& os, const Column& col) {
            os << "Column(addr: " << &col << ", size: " << col.m_size << ", type: " << typeid(T).name() << ")";
            return os;
        }

        std::size_t size() const {
            return m_size;
        }

        std::size_t stride() const {
            return m_stride;
        }

        std::size_t index() const {
            return m_d[0]->idx.col_idx;
        }

        const std::string name() const {
            return m_d[0]->idx.col_name;
        }

        iterator begin() const {
            return iterator(m_d);
        }

        iterator end() const {
            return iterator(m_d + m_size);
        }

        constexpr bool is_null() const {
            return (m_d == nullptr) && (m_size == 0) && (m_stride == 0);
        }

      private:
        std::size_t m_size;
        std::size_t m_stride;
        value_type* m_d;
    };
} // namespace df

#endif // COLUMN_SERIES_H
