#ifndef DATA_FRAME_SERIES_H
#define DATA_FRAME_SERIES_H

#include "df_base_iterator.hpp"
#include "df_common.hpp"

namespace df {
    template<typename T>
    class Series {

      public:
        using data_type      = T;
        using value_type     = data_type;
        using iterator       = BaseIterator<Series, false>;
        using const_iterator = BaseIterator<Series, true>;

        explicit Series(const std::size_t& size) : m_d(new data_type[size]), m_size(size) {
        }

        Series(const std::initializer_list<T>& il) : m_d(new data_type[il.size()]), m_size(il.size()) {
            std::copy(il.begin(), il.end(), m_d);
        }

        ~Series() {
            delete[] m_d;
        }

        Series(const Series& other) {
            m_size = other.m_size;
            m_d    = new T[m_size];
            std::copy(other.begin(), other.end(), m_d);
        }

        Series& operator=(const Series& other) {
            FORCED_ASSERT(m_size == other.m_size, "copy assignment operator on nonmatching size objects");
            if (this != &other) {
                for (std::size_t i = 0; i < m_size; i++) {
                    m_d[i] = other[i];
                }
            }
            return *this;
        }

        data_type& operator[](std::size_t idx) {
            return *(m_d + idx);
        }

        const data_type& operator[](std::size_t idx) const {
            return *(m_d + idx);
        }

        // comparaison operators
        Series<bool> operator==(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i] == other[i]);
            }
            return temp;
        }

        friend Series<bool> operator==(const Series& lhs, const data_type& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] == rhs);
            }
            return temp;
        }

        friend Series<bool> operator==(const data_type& lhs, const Series& rhs) {
            return rhs == lhs;
        }

        Series<bool> operator!=(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i] != other[i]);
            }
            return temp;
        }

        friend Series<bool> operator!=(const Series& lhs, const data_type& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] != rhs);
            }
            return temp;
        }

        friend Series<bool> operator!=(const data_type& lhs, const Series& rhs) {
            return rhs != lhs;
        }

        Series<bool> operator>=(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i] >= other[i]);
            }
            return temp;
        }

        friend Series<bool> operator>=(const Series& lhs, const data_type& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] >= rhs);
            }
            return temp;
        }

        friend Series<bool> operator>=(const data_type& lhs, const Series& rhs) {
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs >= rhs[i]);
            }
            return temp;
        }

        Series<bool> operator<=(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i] <= other[i]);
            }
            return temp;
        }

        friend Series<bool> operator<=(const Series& lhs, const data_type& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] <= rhs);
            }
            return temp;
        }

        friend Series<bool> operator<=(const data_type& lhs, const Series& rhs) {
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs <= rhs[i]);
            }
            return temp;
        }

        Series<bool> operator<(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i] < other[i]);
            }
            return temp;
        }

        friend Series<bool> operator<(const Series& lhs, const data_type& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] < rhs);
            }
            return temp;
        }

        friend Series<bool> operator<(const data_type& lhs, const Series& rhs) {
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs < rhs[i]);
            }
            return temp;
        }

        Series<bool> operator>(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_d[i] > other[i]);
            }
            return temp;
        }

        friend Series<bool> operator>(const Series& lhs, const data_type& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] > rhs);
            }
            return temp;
        }

        friend Series<bool> operator>(const data_type& lhs, const Series& rhs) {
            Series<bool> temp(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                temp[i] = (lhs > rhs[i]);
            }
            return temp;
        }

        // arithmetic operators
        Series operator+(const Series& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_d[i] + rhs[i];
            }
            return res;
        }

        friend Series operator+(const Series& lhs, const data_type& rhs) {
            Series res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i] + rhs;
            }
            return res;
        }

        friend Series operator+(const data_type& lhs, const Series& rhs) {
            return rhs + lhs;
        }

        Series operator+=(const Series& rhs) {
            for (std::size_t i = 0; i < m_size; i++) {
                m_d[i] += rhs[i];
            }
            return *this;
        }

        Series operator+=(const data_type& rhs) {
            for (std::size_t i = 0; i < m_size; i++) {
                m_d[i] += rhs;
            }
            return *this;
        }

        Series operator++(int) {
            Series temp{*this};
            ++(*this);
            return temp;
        }

        Series operator++() {
            for (std::size_t i = 0; i < m_size; i++) {
                ++m_d[i];
            }
            return *this;
        }

        Series operator*(const Series& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_d[i] * rhs[i];
            }
            return res;
        }

        friend Series operator*(const Series& lhs, const data_type& rhs) {
            Series res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i] * rhs;
            }
            return res;
        }

        friend Series operator*(const data_type& lhs, const Series& rhs) {
            return rhs * lhs;
        }

        Series operator-(const Series& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_d[i] - rhs[i];
            }
            return res;
        }

        friend Series operator-(const Series& lhs, const data_type& rhs) {
            Series res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i] - rhs;
            }
            return res;
        }

        friend Series operator-(const data_type& lhs, const Series& rhs) {
            Series res(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                res[i] = lhs - rhs[i];
            }
            return res;
        }

        Series operator-=(const Series& rhs) {
            for (std::size_t i = 0; i < m_size; i++) {
                m_d[i] -= rhs[i];
            }
            return *this;
        }

        Series operator-=(const data_type& rhs) {
            for (std::size_t i = 0; i < m_size; i++) {
                m_d[i] -= rhs;
            }
            return *this;
        }

        Series operator--(int) {
            Series tmp{*this};
            --(*this);
            return tmp;
        }

        Series operator--() {
            for (std::size_t i = 0; i < m_size; i++) {
                --m_d[i];
            }
            return *this;
        }

        Series operator/(const Series& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_d[i] / rhs[i];
            }
            return res;
        }

        friend Series operator/(const Series& lhs, const data_type& rhs) {
            Series res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i] / rhs;
            }
            return res;
        }

        friend Series operator/(const data_type& lhs, const Series& rhs) {
            Series res(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                res[i] = lhs / rhs[i];
            }
            return res;
        }

        template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
        T max() const {
            return *std::max_element(m_d, m_d + m_size);
        }

        template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
        T min() const {
            return *std::min_element(begin(), end());
        }

        bool is_equal_with(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_d[i] != other[i]) { return false; }
            }
            return true;
        }

        iterator begin() {
            return iterator{m_d};
        }

        const_iterator begin() const {
            return const_iterator(m_d);
        }

        iterator end() {
            return iterator{m_d + m_size};
        }

        const_iterator end() const {
            return const_iterator(m_d + m_size);
        }

        std::size_t size() const {
            return m_size;
        }

      private:
        data_type*  m_d;
        std::size_t m_size;
    };

} // namespace df

#endif // DATA_FRAME_SERIES_H
