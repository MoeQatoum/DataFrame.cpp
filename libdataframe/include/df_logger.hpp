#ifndef DATA_FRAME_LOGGER_H
#define DATA_FRAME_LOGGER_H

#include "df_common.hpp"

namespace df {
    template<typename T>
    class Cell;

    template<typename T>
    class DataFrame;

    template<typename T>
    class Column;

    template<typename T>
    class Row;

    template<typename T>
    class RowGroup;

    template<typename T>
    class LoggingContext {
      public:
        using CellLoggingColorCond = std::function<std::string(const Cell<T>*)>;
        using RowNameColorCond     = std::function<std::string(const Row<T>*)>;
        using CellLoggingPrecCond  = std::function<int(const Cell<T>*)>;

        LoggingContext& with_exclude_columns(std::vector<std::string> column_names) {
            // TODO: should be appended instead of replaced
            // TODO: use columns index instead of names
            excluded_cols = column_names;
            return *this;
        }

        LoggingContext& with_exclude_column(std::string col_name) {
            excluded_cols.push_back(col_name);
            return *this;
        }

        LoggingContext& set_precision(std::size_t precision) {
            floatPrecision = precision;
            return *this;
        }

        LoggingContext& set_space_adjustment(std::size_t spaceAdjustment) {
            spacing = spaceAdjustment;
            return *this;
        }

        LoggingContext& set_max_col_name_size(std::size_t size) {
            max_col_name_size = size;
            return *this;
        }

        LoggingContext& set_max_row_name_size(std::size_t size) {
            max_row_name_size = size;
            return *this;
        }

        LoggingContext& with_cell_color_condition(CellLoggingColorCond condition) {
            cell_color_condition = condition;
            return *this;
        }

        LoggingContext& with_row_color_condition(RowNameColorCond condition) {
            row_name_color_condition = condition;
            return *this;
        }

        LoggingContext& with_cell_precision_condition(CellLoggingPrecCond condition) {
            cell_precision_condition = condition;
            return *this;
        }

        CellLoggingColorCond     cell_color_condition     = [](const Cell<T>*) { return std::string(DF_COLOR_W); };
        RowNameColorCond         row_name_color_condition = [](const Row<T>*) { return std::string(DF_COLOR_W); };
        CellLoggingPrecCond      cell_precision_condition = [](const Cell<T>*) { return 8; };
        int                      floatPrecision           = 8;
        int                      spacing                  = 5;
        std::size_t              max_col_name_size        = 0;
        std::size_t              max_row_name_size        = 0;
        std::vector<std::string> excluded_cols            = {};
    };

    template<typename T>
    class Logger {

      protected:
        Logger() {
        }

        Logger(const Logger& other) : context(other.context) {
        }

        Logger operator=(const Logger& other) {
            if (this != &other) { context = other.context; }
            return *this;
        }

      public:
        LoggingContext<T> context;

        Logger& with_context(LoggingContext<T> context) {
            this->context = context;
            return *this;
        }
    };

    template<typename T>
    class DF_Logger : public Logger<T> {
        friend class DataFrame<T>;

        DF_Logger(DataFrame<T>* df) : Logger<T>(), df(df) {
        }

        DF_Logger(const DF_Logger& other) : Logger<T>(other.Logger), df(other.df) {
        }

        DataFrame<T>* df;

      public:
        template<std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        void log(int range = 0) const {
            if (range > static_cast<int>(df->m_row_count) || range < -static_cast<int>(df->m_row_count)) {
                throw std::runtime_error{"range is grater than row count"};
            }

            std::size_t idx_space      = 4;
            std::size_t row_name_space = this->context.max_row_name_size + this->context.spacing;
            int         col_spacing    = this->context.max_col_name_size + this->context.spacing;

            auto contains = [](const std::vector<std::string>& excluded_cols_list, const std::string& cur_col_name) {
                return std::find_if(excluded_cols_list.begin(),
                                    excluded_cols_list.end(),
                                    [cur_col_name](const std::string& col_name) { return cur_col_name == col_name; })
                       != excluded_cols_list.end();
            };

            if (std::is_floating_point_v<T>) { std::cout.precision(this->context.floatPrecision + 1); }

            std::cout << std::left << std::setw(row_name_space + idx_space) << "idx";
            for (auto col_iter = df->iter_cols(); col_iter < df->end(); ++col_iter) {
                std::string col_name = col_iter.current_col().name();
                if (!contains(this->context.excluded_cols, col_name)) { std::cout << std::left << std::setw(col_spacing) << col_name; }
            }
            std::cout << std::endl;

            int range_start = 0;
            int range_end   = df->m_row_count;
            if (range > 0) {
                range_end = std::min(range, static_cast<int>(df->m_row_count));
            } else if (range < 0) {
                range_start = std::max(0, static_cast<int>(df->m_row_count) + range);
            }

            for (int idx = range_start; idx < range_end; idx++) {
                const auto& current_row = this->df->row(idx);
                std::cout << std::left << std::setw(idx_space) << current_row.index() << this->context.row_name_color_condition(&current_row)
                          << std::left << std::setw(row_name_space) << current_row.name() << DF_COLOR_W;
                for (const auto& cell : current_row) {
                    if (!contains(this->context.excluded_cols, cell->idx.col_name)) {
                        std::cout << this->context.cell_color_condition(cell) << std::left << std::setw(col_spacing) << cell->value << DF_COLOR_W;
                    }
                }
                std::cout << std::endl;
            }
        }
    };

    template<typename T>
    class RowGroup_Logger : Logger<T> {
        friend class RowGroup<T>;

        RowGroup_Logger(RowGroup<T>* rg) : Logger<T>(), rg(rg) {
        }

        RowGroup_Logger(const RowGroup_Logger& other) : Logger<T>(other.Logger), rg(other.rg) {
        }

        RowGroup<T>* rg;

      public:
        template<std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
        void log(int range = 0) const {
            if (range > static_cast<int>(rg->size()) || range < -static_cast<int>(rg->size())) {
                throw std::runtime_error{"range is grater than row count"};
            }

            std::size_t idx_space      = 4;
            std::size_t row_name_space = this->context.max_row_name_size + this->context.spacing;
            int         col_spacing    = this->context.max_col_name_size + this->context.spacing;

            auto contains = [](const std::vector<std::string>& excluded_cols_list, const std::string& cur_col_name) {
                return std::find_if(excluded_cols_list.begin(),
                                    excluded_cols_list.end(),
                                    [cur_col_name](const std::string& col_name) { return cur_col_name == col_name; })
                       != excluded_cols_list.end();
            };

            if (std::is_floating_point_v<T>) { std::cout.precision(this->context.floatPrecision + 1); }

            std::cout << std::left << std::setw(row_name_space + idx_space) << "idx";
            for (const auto& c : rg->at(0)) {
                if (!contains(this->context.excluded_cols, c->idx.col_name)) { std::cout << std::left << std::setw(col_spacing) << c->idx.col_name; }
            }
            std::cout << std::endl;

            int range_start = 0;
            int range_end   = rg->size();
            if (range > 0) {
                range_end = std::min(range, static_cast<int>(rg->size()));
            } else if (range < 0) {
                range_start = std::max(0, static_cast<int>(rg->size()) + range);
            }

            for (int idx = range_start; idx < range_end; idx++) {
                const Row<T>& current_row = rg->at(idx);
                std::cout << std::left << std::setw(idx_space) << current_row.index() << this->context.row_name_color_condition(&current_row)
                          << std ::left << std::setw(row_name_space) << current_row.name() << DF_COLOR_W;
                for (const auto& cell : current_row) {
                    if (!contains(this->context.excluded_cols, cell->idx.col_name)) {
                        std::cout << this->context.cell_color_condition(cell) << std::left << std::setw(col_spacing) << cell->value << DF_COLOR_W;
                    }
                }
                std::cout << std::endl;
            }
        }
    };

} // namespace df

#endif // DATA_FRAME_LOGGER_H
