#ifndef DATA_FRAME_CELL_H
#define DATA_FRAME_CELL_H

#include "df_common.hpp"

namespace df {

    struct Index {

        Index() {
        }

        Index(const Index& other)
            : row_idx(other.row_idx),
              col_idx(other.col_idx),
              global_idx(other.global_idx),
              col_name(other.col_name),
              row_name(other.row_name) {
        }

        // Index(const Index&& other) = delete;

        Index& operator=(const Index& rhs) {
            if (this != &rhs) {
                row_idx    = rhs.row_idx;
                col_idx    = rhs.col_idx;
                global_idx = rhs.global_idx;
                col_name   = rhs.col_name;
                row_name   = rhs.row_name;
            }
            return *this;
        }

        std::size_t row_idx;
        std::size_t col_idx;
        std::size_t global_idx;

        std::string col_name;
        std::string row_name;

        friend std::ostream& operator<<(std::ostream& os, const Index& df_idx) {
            os << "Index(glob<" << df_idx.global_idx << ">, column<" << df_idx.col_name << ", " << df_idx.col_idx << ">, row<" << df_idx.row_name
               << ", " << df_idx.row_idx << ">)";
            return os;
        }
    };

    template<typename T>
    class Cell {
      public:
        using ValueType = T;

        Cell() {
        }

        Cell(const Cell& other) : value(other.value), idx(other.idx) {
        }

        // Cell(const Cell&& other) = delete;

        Cell<T>& operator=(const Cell<T>& other) {
            if (this != &other) {
                value = other.value;
                idx   = other.idx;
            }
            return *this;
        }

        void operator=(const T val) {
            value = val;
        }

        void operator++() {
            value++;
        }

        void operator++(int) {
            ++value;
        }

        void operator--() {
            value--;
        }

        void operator--(int) {
            --value;
        }

        void operator+=(T off) {
            value += off;
        }

        void operator-=(T off) {
            value -= off;
        }

        friend std::ostream& operator<<(std::ostream& os, const Cell<T>& cell) {
            os << "Cell(value: " << cell.value << ", idx: " << cell.idx << ")";
            return os;
        }

        T     value;
        Index idx;
    };

} // namespace df

#endif // DATA_FRAME_CELL_H
