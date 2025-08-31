#ifndef DATA_FRAME_ROW_GROUP_H
#define DATA_FRAME_ROW_GROUP_H

#include "df_base_iterator.hpp"
#include "df_common.hpp"
#include "df_logger.hpp"
#include "df_row.hpp"

namespace df {

    template<typename>
    class Dataframe;

    template<typename T>
    class RowGroup {
      public:
        using value_type     = T;
        using data_type      = typename value_type::data_type;
        using iterator       = BaseIterator<RowGroup, false>;
        using const_iterator = BaseIterator<RowGroup, true>;

        RowGroup(DataFrame<data_type>* df) : logger(this), logging_context(df->logger.context) {
            m_size     = df->row_count();
            m_d        = new value_type[m_size];
            m_row_size = df->row_size();
            for (auto row_iter = df->iter_rows(); row_iter < df->end(); row_iter++) {
                m_d[row_iter.current_row_idx()] = row_iter.current_row();
            }
            logger.with_context(logging_context);
        }

        RowGroup(const RowGroup& other)
            : logger(this),
              logging_context(other.logging_context),
              m_size(other.m_size),
              m_d(new value_type[m_size]),
              m_row_size(other.m_row_size) {
            for (std::size_t i = 0; i < m_size; i++) {
                m_d[i] = other.m_d[i];
            }
            logger.with_context(logging_context);
        }

        RowGroup(RowGroup&& other)
            : logger(this),
              logging_context(other.logging_context),
              m_size(other.m_size),
              m_d(other.m_d),
              m_row_size(other.m_row_size) {
            logger.with_context(logging_context);
            other.m_d = nullptr;
        }

        ~RowGroup() {
            delete[] m_d;
        }

        RowGroup operator=(const RowGroup& other) = delete; // why?

        RowGroup& operator=(RowGroup&& other) {
            if (this != &other) {
                logging_context = other.logging_context;
                m_size          = other.m_size;
                m_d             = other.m_d;
                m_row_size      = other.m_row_size;
                other.m_d       = nullptr;
            }
            return *this;
        }

        value_type& operator[](const std::size_t& idx) {
            return m_d[idx];
        }

        const value_type& operator[](const std::size_t& idx) const {
            return m_d[idx];
        }

        template<typename U = data_type, typename = std::enable_if_t<std::is_arithmetic_v<data_type>, bool>>
        RowGroup& sort(const std::string& column_name, const bool ascending = false) {
            std::size_t col_idx = m_d[0].column_index_of(column_name);

            std::sort(m_d, m_d + m_size, [ascending, col_idx](const value_type& a, const value_type& b) {
                const data_type& a_val = a[col_idx]->value;
                const data_type& b_val = b[col_idx]->value;
                return ascending ? (a_val < b_val) : (a_val > b_val);
            });

            return *this;
        }

        std::size_t size() const {
            return m_size;
        }

        std::size_t row_size() const {
            return m_row_size;
        }

        value_type& at(std::size_t index) {
            return m_d[index];
        }

        const value_type& at(std::size_t index) const {
            return m_d[index];
        }

        iterator begin() const {
            return iterator(m_d);
        }

        iterator end() const {
            return iterator(m_d + m_size);
        }

        void log(int range = 0) const {
            logger.log(range);
        }

        RowGroup_Logger<data_type> logger;

      private:
        LoggingContext<data_type> logging_context;
        std::size_t               m_size;
        value_type*               m_d;
        std::size_t               m_row_size;
    };

} // namespace df

#endif // DATA_FRAME_ROW_GROUP_H
