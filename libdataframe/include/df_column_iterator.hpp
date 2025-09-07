#ifndef DATAFRAME_COLUMN_ITERATOR_H
#define DATAFRAME_COLUMN_ITERATOR_H

#include "df_column.hpp"
#include "df_common.hpp"

namespace df {

    template<typename dataframe, bool IsConst>
    class ColumnIterator {
        using dataframe_iterator = std::conditional_t<IsConst, typename dataframe::const_iterator, typename dataframe::iterator>;
        using column             = std::conditional_t<IsConst, typename dataframe::const_column_type, typename dataframe::column_type>;

      public:
        ColumnIterator(dataframe_iterator df_begin, std::size_t col_size, std::size_t row_size)
            : m_ptr(df_begin),
              m_col_size(col_size),
              m_row_size(row_size),
              m_current_col_idx(0) {
        }

        ~ColumnIterator() {
        }

        ColumnIterator(const ColumnIterator<dataframe, true>& other)
            requires(IsConst)
            : m_ptr(other.m_ptr) {
        }

        ColumnIterator(const ColumnIterator& other)
            : m_ptr(other.m_ptr),
              m_col_size(other.m_col_size),
              m_row_size(other.m_row_size),
              m_current_col_idx(other.m_current_col_idx) {
        }

        // TODO: move constructor ?

        // Implicit conversion to column; use for syntactic convenience (e.g., auto r = *it).
        operator column() {
            return column(m_ptr + m_current_col_idx, m_col_size, m_row_size);
        }

        column current_col() {
            return column(m_ptr + m_current_col_idx, m_col_size, m_row_size);
        }

        std::size_t current_col_idx() const {
            return m_current_col_idx;
        }

        ColumnIterator& operator=(const ColumnIterator& other) {
            if (this != &other) {
                m_ptr             = other.m_ptr;
                m_col_size        = other.m_col_size;
                m_row_size        = other.m_row_size;
                m_current_col_idx = other.m_current_col_idx;
            }
            return *this;
        }

        ColumnIterator operator+(const std::size_t& off) const {
            ColumnIterator tmp{*this};
            if (tmp += off; tmp.m_current_col_idx + off >= tmp.m_col_size) { throw std::out_of_range("ColumnIterator::operator+ out of range"); }
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

        ColumnIterator& operator--() {
            --m_current_col_idx;
            return *this;
        }

        ColumnIterator operator--(int) {
            ColumnIterator tmp{*this};
            --(*this);
            return tmp;
        }

        ColumnIterator& operator+=(std::size_t off) {
            m_current_col_idx += off;
            return *this;
        }

        ColumnIterator& operator-=(std::size_t off) {
            m_current_col_idx -= off;
            return *this;
        }

        template<bool B>
        friend bool operator<(const ColumnIterator& lhs, const BaseIterator<dataframe, B>& rhs) {
            return (lhs.m_ptr + (lhs.m_current_col_idx + (lhs.m_row_size * (lhs.m_col_size - 1)))) < rhs;
        }

        template<bool B>
        friend bool operator<(const BaseIterator<dataframe, B>& lhs, const ColumnIterator& rhs) {
            return lhs.m_ptr < (rhs.m_ptr + (rhs.m_current_col_idx + (rhs.m_row_size * rhs.m_col_size)));
        }

        template<bool B>
        friend bool operator>(const ColumnIterator& lhs, const BaseIterator<dataframe, B>& rhs) {
            return (lhs.m_ptr + (lhs.m_current_col_idx + (lhs.m_row_size * lhs.m_col_size))) > rhs.m_ptr;
        }

        template<bool B>
        friend bool operator>(const BaseIterator<dataframe, B>& lhs, const ColumnIterator& rhs) {
            return lhs.m_ptr > (rhs.m_ptr + (rhs.m_current_col_idx + (rhs.m_row_size * rhs.m_col_size)));
        }

      private:
        dataframe_iterator m_ptr;
        std::size_t        m_col_size;
        std::size_t        m_row_size;
        std::size_t        m_current_col_idx;
    };
} // namespace df

#endif // DATAFRAME_COLUMN_ITERATOR_H
