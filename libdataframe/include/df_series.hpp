#ifndef DATA_FRAME_SERIES_H
#define DATA_FRAME_SERIES_H

#include "df_base_iterator.hpp"
#include "df_common.hpp"

namespace df {
    template<typename T>
    class Series {

      public:
        using value_type     = T;
        using iterator       = BaseIterator<Series, false>;
        using const_iterator = BaseIterator<Series, true>;

        explicit Series(const std::size_t& size) : m_ptr(new T[size]), m_size(size) {
        }

        Series(const std::initializer_list<T>& il) : m_ptr(new T[il.size()]), m_size(il.size()) {
            std::copy(il.begin(), il.end(), m_ptr);
        }

        ~Series() {
            delete[] m_ptr;
        }

        Series(const Series& other) {
            m_size = other.m_size;
            m_ptr  = new T[m_size];
            std::copy(other.begin(), other.end(), m_ptr);
        }

        // TODO: move constructors ??

        Series& operator=(const Series& other) {
            FORCED_ASSERT(m_size == other.m_size, "copy assignment operator on nonmatching size objects");
            if (this != &other) {
                for (std::size_t i = 0; i < m_size; i++) {
                    m_ptr[i] = other[i];
                }
            }
            return *this;
        }

        T& operator[](std::size_t idx) {
            return *(m_ptr + idx);
        }

        const T& operator[](std::size_t idx) const {
            return *(m_ptr + idx);
        }

        // comparaison operators
        Series<bool> operator==(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_ptr[i] == other[i]);
            }
            return temp;
        }

        // TODO: is comparable with T1
        friend Series<bool> operator==(const Series& lhs, const /* T1 */ T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] == rhs);
            }
            return temp;
        }

        // TODO: is comparable with T1
        friend Series<bool> operator==(const /* T1 */ T& lhs, const Series& rhs) {
            return rhs == lhs;
        }

        Series<bool> operator!=(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_ptr[i] != other[i]);
            }
            return temp;
        }

        friend Series<bool> operator!=(const Series& lhs, const T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] != rhs);
            }
            return temp;
        }

        friend Series<bool> operator!=(const T& lhs, const Series& rhs) {
            return rhs != lhs;
        }

        Series<bool> operator>=(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            Series<bool> temp(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                temp[i] = (m_ptr[i] >= other[i]);
            }
            return temp;
        }

        // TODO: is comparable with T1
        friend Series<bool> operator>=(const Series& lhs, const /* T1 */ T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] >= rhs);
            }
            return temp;
        }

        // TODO: is comparable with T1
        friend Series<bool> operator>=(const /* T1 */ T& lhs, const Series& rhs) {
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
                temp[i] = (m_ptr[i] <= other[i]);
            }
            return temp;
        }

        friend Series<bool> operator<=(const Series& lhs, const T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] <= rhs);
            }
            return temp;
        }

        friend Series<bool> operator<=(const T& lhs, const Series& rhs) {
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
                temp[i] = (m_ptr[i] < other[i]);
            }
            return temp;
        }

        friend Series<bool> operator<(const Series& lhs, const T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] < rhs);
            }
            return temp;
        }

        friend Series<bool> operator<(const T& lhs, const Series& rhs) {
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
                temp[i] = (m_ptr[i] > other[i]);
            }
            return temp;
        }

        // TODO: is comparable with T1
        friend Series<bool> operator>(const Series& lhs, const /*TI*/ T& rhs) {
            Series<bool> temp(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                temp[i] = (lhs[i] > rhs);
            }
            return temp;
        }

        // TODO: is comparable with T1
        friend Series<bool> operator>(const /*TI*/ T& lhs, const Series& rhs) {
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
                res[i] = m_ptr[i] + rhs[i];
            }
            return res;
        }

        friend Series operator+(const Series& lhs, const T& rhs) {
            Series res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i] + rhs;
            }
            return res;
        }

        friend Series operator+(const T& lhs, const Series& rhs) {
            return rhs + lhs;
        }

        // TODO: returns a reference to the current Series
        Series operator+=(const T& rhs) {
            for (std::size_t i = 0; i < m_size; i++) {
                m_ptr[i] += rhs;
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
                ++m_ptr[i];
            }
            return *this;
        }

        Series operator*(const Series& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_ptr[i] * rhs[i];
            }
            return res;
        }

        friend Series operator*(const Series& lhs, const T& rhs) {
            Series res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i] * rhs;
            }
            return res;
        }

        friend Series operator*(const T& lhs, const Series& rhs) {
            return rhs * lhs;
        }

        Series operator-(const Series& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_ptr[i] - rhs[i];
            }
            return res;
        }

        friend Series operator-(const Series& lhs, const T& rhs) {
            Series res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i] - rhs;
            }
            return res;
        }

        friend Series operator-(const T& lhs, const Series& rhs) {
            Series res(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                res[i] = lhs - rhs[i];
            }
            return res;
        }

        Series operator-=(const T& rhs) {
            for (std::size_t i = 0; i < m_size; i++) {
                m_ptr[i] -= rhs;
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
                --m_ptr[i];
            }
            return *this;
        }

        Series operator/(const Series& rhs) const {
            FORCED_ASSERT(m_size == rhs.m_size, "arithmetic operation on nonmatching size objects");
            Series res(m_size);
            for (std::size_t i = 0; i < m_size; i++) {
                res[i] = m_ptr[i] / rhs[i];
            }
            return res;
        }

        friend Series operator/(const Series& lhs, const T& rhs) {
            Series res(lhs.m_size);
            for (std::size_t i = 0; i < lhs.m_size; i++) {
                res[i] = lhs[i] / rhs;
            }
            return res;
        }

        friend Series operator/(const T& lhs, const Series& rhs) {
            Series res(rhs.m_size);
            for (std::size_t i = 0; i < rhs.m_size; i++) {
                res[i] = lhs / rhs[i];
            }
            return res;
        }

        template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
        T max() const {
            T temp = m_ptr[0]->value;
            for (std::size_t i = 1; i < m_size; ++i) {
                if (m_ptr[i]->value > temp) { temp = m_ptr[i]->value; }
            }
            return temp;
        }

        template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
        T min() const {
            T temp = m_ptr[0]->value;
            for (std::size_t i = 1; i < m_size; ++i) {
                if (m_ptr[i]->value < temp) { temp = m_ptr[i]->value; }
            }
            return temp;
        }

        bool is_equal_with(const Series& other) const {
            FORCED_ASSERT(m_size == other.m_size, "comparaison operation on nonmatching size objects");
            for (std::size_t i = 0; i < m_size; i++) {
                if (m_ptr[i] != other[i]) { return false; }
            }
            return true;
        }

        iterator begin() {
            return iterator{m_ptr};
        }

        const_iterator begin() const {
            return const_iterator(m_ptr);
        }

        iterator end() {
            return {m_ptr + m_size};
        }

        const_iterator end() const {
            return const_iterator(m_ptr + m_size);
        }

        std::size_t size() const {
            return m_size;
        }

      private:
        value_type* m_ptr;
        std::size_t m_size;
    };

} // namespace df

#endif // DATA_FRAME_SERIES_H
