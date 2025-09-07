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
        friend class RowView;

        template<typename>
        friend class ColumnView;

        template<typename>
        friend class Series;

        template<typename>
        friend class RowGroupView;

        explicit BaseIterator(pointer ptr = nullptr) : m_ptr(ptr) {
        }

      public:
        // allow conversion from non-const to const iterator
        BaseIterator(const BaseIterator<Iterable, false>& other)
            requires(IsConst)
            : m_ptr(other.m_ptr) {
        }

        BaseIterator(const BaseIterator& other) : m_ptr(other.m_ptr) {
        }

        ~BaseIterator() {
        }

        // clang-format off
        pointer operator&() const { return m_ptr; }
        reference operator*() const {return *m_ptr; }
        pointer operator->() const { return m_ptr; }

        reference operator[](difference_type n) const { return m_ptr[n]; }

        BaseIterator& operator++() { ++m_ptr; return *this; }
        BaseIterator operator++(int) { BaseIterator tmp = *this; ++m_ptr; return tmp; }
        BaseIterator& operator--() { --m_ptr; return *this; }
        BaseIterator operator--(int) { BaseIterator tmp = *this; --m_ptr; return tmp; }

        BaseIterator operator+(difference_type n) const { return BaseIterator(m_ptr + n); }
        BaseIterator operator-(difference_type n) const { return BaseIterator(m_ptr - n); }

        BaseIterator& operator+=(difference_type n) { m_ptr += n; return *this; }
        BaseIterator& operator-=(difference_type n) { m_ptr -= n; return *this; }

        auto operator<=>(const BaseIterator& other) const = default;
        template<bool B> std::strong_ordering operator<=>(const BaseIterator<Iterable, B>& other) const { return m_ptr <=> other.m_ptr; }
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
