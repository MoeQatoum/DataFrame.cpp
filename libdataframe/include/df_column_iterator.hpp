#ifndef DATAFRAME_COLUMN_ITERATOR_H
#define DATAFRAME_COLUMN_ITERATOR_H

#include "df_column.hpp"
#include "df_common.hpp"

namespace df {

    template<typename dataframe, bool IsConst>
    class ColumnIterator {
        using dataframe_iterator = std::conditional_t<IsConst, typename dataframe::const_iterator, typename dataframe::iterator>;
        using column             = Column<std::conditional_t<IsConst, typename dataframe::const_value_type, typename dataframe::value_type>>;

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

        column current_col() {
            return column(m_ptr + m_current_col_idx, m_col_size, m_row_size);
        }

        std::size_t current_col_idx() const {
            return m_current_col_idx;
        }

        // operator column() {
        //     return column(m_ptr + m_current_col_idx, m_col_size, m_row_size);
        // }

        // operator ConstColumn() const {
        //     return ConstColumn(m_df_begin + m_current_col_idx, m_col_size, m_row_size);
        // }

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

        // friend bool operator<(const ColumnIterator& lhs, const std::size_t& rhs) {
        //     return lhs.m_current_col_idx < rhs;
        // }

        // friend bool operator<(const std::size_t& lhs, const ColumnIterator& rhs) {
        //     return lhs < rhs.m_current_col_idx;
        // }

        // friend bool operator>(const ColumnIterator& lhs, const std::size_t& rhs) {
        //     return lhs.m_current_col_idx > rhs;
        // }

        // friend bool operator>(const std::size_t& lhs, const ColumnIterator& rhs) {
        //     return lhs > rhs.m_current_col_idx;
        // }

      private:
        dataframe_iterator m_ptr;
        std::size_t        m_col_size;
        std::size_t        m_row_size;
        std::size_t        m_current_col_idx;
    };
} // namespace df

#endif // DATAFRAME_COLUMN_ITERATOR_H
