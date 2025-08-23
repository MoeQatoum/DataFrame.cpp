#ifndef DATAFRAME_ROW_ITERATOR_H
#define DATAFRAME_ROW_ITERATOR_H

#include "df_column.hpp"
#include "df_row.hpp"

namespace df {

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
} // namespace df

#endif // DATAFRAME_ROW_ITERATOR_H
