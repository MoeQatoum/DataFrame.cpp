#ifndef DATA_FRAME_CELL_H
#define DATA_FRAME_CELL_H

#include "df_common.hpp"

namespace df {
    /*
     TODO: is copying the index correct or even required? we should not touch the index of the cell,
     once set shout not be tampered with, otherwise
     copying cells will mix the indexing, therefore we should copy values only!
    */

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

        // Index(const Index&& other) = delete; // TODO: should we allow move semantics?

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
        using data_type = T;

        Cell() {
        }

        Cell(const Cell& other) : value(other.value), idx(other.idx) {
        }

        // Cell(const Cell&& other) = delete; //TODO: should we allow move semantics?

        Cell& operator=(const Cell& other) {
            if (this != &other) {
                value = other.value;
                idx   = other.idx; // TODO: should we copy the index?
            }
            return *this;
        }

        void operator=(const data_type val) {
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

        void operator+=(data_type off) {
            value += off;
        }

        void operator-=(data_type off) {
            value -= off;
        }

        friend std::ostream& operator<<(std::ostream& os, const Cell& cell) {
            os << "Cell(value: " << cell.value << ", idx: " << cell.idx << ")";
            return os;
        }

        data_type value;
        Index     idx;
    };

} // namespace df

#endif // DATA_FRAME_CELL_H
