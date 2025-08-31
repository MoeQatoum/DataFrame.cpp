#ifndef DATA_FRAME_BASE_ITERATOR_H
#define DATA_FRAME_BASE_ITERATOR_H

#include "df_common.hpp"

namespace df {

    template<typename Iterable, bool IsConst>
    class BaseIterator {
      public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = typename Iterable::value_type;
        using difference_type   = std::ptrdiff_t;
        using pointer           = std::conditional_t<IsConst, const value_type*, value_type*>;
        using reference         = std::iter_reference_t<pointer>;

      private:
        template<typename>
        friend class DataFrame;

        template<typename>
        friend class Row;

        template<typename>
        friend class Column;

        template<typename>
        friend class Series;

        template<typename>
        friend class RowGroup;

        template<typename, bool>
        friend class RowIterator;

        template<typename, bool>
        friend class ColumnIterator;

        template<typename, bool>
        friend class ColumnIterator;

        explicit BaseIterator(pointer ptr = nullptr) : m_ptr(ptr) {
        }

      public:
        BaseIterator() = default;

        BaseIterator(const BaseIterator<Iterable, true>& other)
            requires(IsConst)
            : m_ptr(other.m_ptr) {
        }

        BaseIterator(const BaseIterator& other) : m_ptr(other.m_ptr) {
        }

        ~BaseIterator() {
        }

        BaseIterator& operator=(const BaseIterator& other) {
            if (this != &other) { m_ptr = other.m_ptr; }
            return *this;
        }

        // clang-format off
        pointer operator&() const { return m_ptr; }
        reference operator*() const { return *m_ptr; }
        pointer operator->() const { return m_ptr; }
        reference operator[](difference_type n) const { return m_ptr[n]; }

        BaseIterator& operator++() { ++m_ptr; return *this; }
        BaseIterator operator++(int) { BaseIterator tmp=*this; ++m_ptr; return tmp; }
        BaseIterator& operator--() { --m_ptr; return *this; }
        BaseIterator operator--(int) { BaseIterator tmp=*this; --m_ptr; return tmp; }

        BaseIterator operator+(difference_type n) const { return BaseIterator(m_ptr+n); }
        BaseIterator operator-(difference_type n) const { return BaseIterator(m_ptr-n); }
        BaseIterator& operator+=(difference_type n) { m_ptr+=n; return *this; }
        BaseIterator& operator-=(difference_type n) { m_ptr-=n; return *this; }

        /*
        bool operator==(const BaseIterator& other) const { return m_ptr == other.m_ptr; }
        bool operator!=(const BaseIterator& other) const { return m_ptr != other.m_ptr; }
        bool operator<(const  BaseIterator& other) const  { return m_ptr < other.m_ptr; }
        bool operator>(const  BaseIterator& other) const  { return m_ptr > other.m_ptr; }
        bool operator<=(const BaseIterator& other) const { return m_ptr <= other.m_ptr; }
        bool operator>=(const BaseIterator& other) const { return m_ptr >= other.m_ptr; }

        // Cross-const comparisons
        template <bool B> bool operator==(const BaseIterator<Iterable,B>& other) const { return m_ptr == other.m_ptr; }
        template <bool B> bool operator!=(const BaseIterator<Iterable,B>& other) const { return m_ptr != other.m_ptr; }
        template <bool B> bool operator<(const  BaseIterator<Iterable,B>& other) const  { return m_ptr < other.m_ptr; }
        template <bool B> bool operator>(const  BaseIterator<Iterable,B>& other) const  { return m_ptr > other.m_ptr; }
        template <bool B> bool operator<=(const BaseIterator<Iterable,B>& other) const { return m_ptr <= other.m_ptr; }
        template <bool B> bool operator>=(const BaseIterator<Iterable,B>& other) const { return m_ptr >= other.m_ptr; }
        */

        auto operator<=>(const BaseIterator& other) const = default;
        template <bool B> std::strong_ordering operator<=>(const BaseIterator<Iterable,B>& other) const { return m_ptr <=> other.m_ptr; }

        // friend BaseIterator operator+(difference_type lhs, const BaseIterator& rhs) { BaseIterator tmp{rhs}; tmp += lhs; return tmp; }
        // friend BaseIterator operator+(const BaseIterator& lhs, difference_type rhs) { BaseIterator tmp{lhs}; tmp += rhs; return tmp; }
        // BaseIterator& operator++() { m_ptr++; return *this; }
        // BaseIterator operator++(int) { BaseIterator tmp{*this}; ++(*this); return tmp; }
        // BaseIterator& operator+=(std::size_t off) { m_ptr += off; return *this; }

        // friend difference_type operator-(const BaseIterator& lhs, const BaseIterator& rhs) { return lhs.m_ptr - rhs.m_ptr; }
        // friend BaseIterator operator-(const BaseIterator& lhs, difference_type rhs) { BaseIterator tmp{lhs}; tmp -= rhs; return tmp; }
        // BaseIterator& operator--() { m_ptr--; return *this; }
        // BaseIterator operator--(int) { BaseIterator tmp{*this}; --(*this); return tmp; }
        // BaseIterator& operator-=(difference_type off) { m_ptr -= off; return *this; }

        // bool operator==(const BaseIterator& other) const { return other.m_ptr == this->m_ptr; }
        // bool operator!=(const BaseIterator& other) const { return this->m_ptr != other.m_ptr; }
        // friend bool operator<(const BaseIterator& lhs, const BaseIterator& rhs) { return lhs.m_ptr < rhs.m_ptr; }
        // friend bool operator>(const BaseIterator& lhs, const BaseIterator& rhs) { return lhs.m_ptr > rhs.m_ptr; }
        // clang-format on

        friend std::ostream& operator<<(std::ostream& os, const BaseIterator& itr) {
            os << "Iterator(current addr: 0x" << std::hex << reinterpret_cast<void*>(itr.m_ptr) << std::dec << ")";
            return os;
        }

      private:
        pointer m_ptr = nullptr;
    };

}; // namespace df

#endif // DATA_FRAME_BASE_ITERATOR_H
