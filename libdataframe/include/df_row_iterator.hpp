#ifndef DATAFRAME_ROW_ITERATOR_H
#define DATAFRAME_ROW_ITERATOR_H

#include "df_column.hpp"
#include "df_row.hpp"

namespace df {

    template<typename dataframe, bool IsConst>
    class RowIterator {

      public:
        using dataframe_iterator = std::conditional_t<IsConst, typename dataframe::const_iterator, typename dataframe::iterator>;
        using row                = RowView<std::conditional_t<IsConst, typename dataframe::const_value_type, typename dataframe::value_type>>;

        RowIterator(const dataframe_iterator df_begin, std::size_t row_size) : m_ptr(df_begin), m_row_size(row_size), m_current_row_idx(0) {
        }

        RowIterator(const RowIterator<dataframe, true>& other)
            requires(IsConst)
            : m_ptr(other.m_ptr) {
        }

        RowIterator(const RowIterator& other) : m_ptr(other.m_ptr), m_row_size(other.m_row_size), m_current_row_idx(other.m_current_row_idx) {
        }

        // TODO: move constructor ?

        ~RowIterator() {
        }

        row current_row() {
            return row(m_ptr, m_current_row_idx, m_row_size);
        }

        // Implicit conversion to Row; use for syntactic convenience (e.g., auto r = *it).
        operator row() {
            return row(m_ptr, m_current_row_idx, m_row_size);
        }

        // operator const_row() const {
        //     return row(m_df_iter, m_current_row_idx, m_row_size);
        // }

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

        // friend bool operator<(const RowIterator& lhs, const std::size_t& rhs) {
        //     return lhs.m_current_row_idx < rhs;
        // }

        // friend bool operator<(const std::size_t& lhs, const RowIterator& rhs) {
        //     return lhs < rhs.m_current_row_idx;
        // }

        // friend bool operator>(const RowIterator& lhs, const std::size_t& rhs) {
        //     return lhs.m_current_row_idx > rhs;
        // }

        // friend bool operator>(const std::size_t& lhs, const RowIterator& rhs) {
        //     return lhs > rhs.m_current_row_idx;
        // }

      private:
        dataframe_iterator m_ptr;
        std::size_t        m_row_size;
        std::size_t        m_current_row_idx;
    };
} // namespace df

#endif // DATAFRAME_ROW_ITERATOR_H
