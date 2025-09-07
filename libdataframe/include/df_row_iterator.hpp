#ifndef DATAFRAME_ROW_ITERATOR_H
#define DATAFRAME_ROW_ITERATOR_H

#include "df_column.hpp"
#include "df_row.hpp"

namespace df {

    template<typename dataframe, bool IsConst>
    class RowIterator {

      public:
        using dataframe_iterator = std::conditional_t<IsConst, typename dataframe::const_iterator, typename dataframe::iterator>;
        using row                = std::conditional_t<IsConst, typename dataframe::const_row_type, typename dataframe::row_type>;

        RowIterator(const dataframe_iterator df_begin, std::size_t row_size) : m_ptr(df_begin), m_row_size(row_size), m_current_row_idx(0) {
        }

        ~RowIterator() {
        }

        RowIterator(const RowIterator<dataframe, true>& other)
            requires(IsConst)
            : m_ptr(other.m_ptr) {
        }

        RowIterator(const RowIterator& other) : m_ptr(other.m_ptr), m_row_size(other.m_row_size), m_current_row_idx(other.m_current_row_idx) {
        }

        // TODO: move constructor ?

        // Implicit conversion to row; use for syntactic convenience (e.g., auto r = *it).
        operator row() {
            return row(m_ptr, m_current_row_idx, m_row_size);
        }

        row current_row() {
            return row(m_ptr, m_current_row_idx, m_row_size);
        }

        std::size_t current_row_idx() const {
            return m_current_row_idx;
        }

        RowIterator& operator=(const RowIterator& other) {
            if (this != &other) {
                m_ptr             = other.m_ptr;
                m_row_size        = other.m_row_size;
                m_current_row_idx = other.m_current_row_idx;
            }
            return *this;
        }

        RowIterator operator+(const std::size_t& off) const {
            RowIterator tmp{*this};
            if (tmp += off; tmp.m_current_row_idx >= tmp.m_row_size) { throw std::out_of_range("ColumnIterator::operator+ out of range"); }
            return tmp;
        }

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

        RowIterator& operator+=(std::size_t off) {
            m_current_row_idx += off;
            return *this;
        }

        RowIterator& operator-=(std::size_t off) {
            m_current_row_idx -= off;
            return *this;
        }

        template<bool B>
        friend bool operator<(const RowIterator& lhs, const BaseIterator<dataframe, B>& rhs) {
            return (lhs.m_ptr + (lhs.m_current_row_idx * lhs.m_row_size)) < rhs;
        }

        template<bool B>
        friend bool operator<(const BaseIterator<dataframe, B>& lhs, const RowIterator& rhs) {
            return lhs < (rhs.m_ptr + (rhs.m_current_row_idx * rhs.m_row_size));
        }

        template<bool B>
        friend bool operator>(const RowIterator& lhs, const BaseIterator<dataframe, B>& rhs) {
            return (lhs.m_ptr + (lhs.m_current_row_idx * lhs.m_row_size)) > rhs;
        }

        template<bool B>
        friend bool operator>(const BaseIterator<dataframe, B>& lhs, const RowIterator& rhs) {
            return lhs > (rhs.m_ptr + (rhs.m_current_row_idx * rhs.m_row_size));
        }

      private:
        dataframe_iterator m_ptr;
        std::size_t        m_row_size;
        std::size_t        m_current_row_idx;
    };
} // namespace df

#endif // DATAFRAME_ROW_ITERATOR_H
