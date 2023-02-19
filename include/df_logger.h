#ifndef DATA_FRAME_LOGGER_H
#define DATA_FRAME_LOGGER_H

#include "df_common.h"

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
  class DF_Logger {
    friend class DataFrame<T>;

    using DataFrame            = DataFrame<T>;
    using CellLoggingColorCond = std::function<df::String(Cell<T>*)>;

    DF_Logger(DataFrame* df, sizetype max_col_name_size = 0, sizetype max_row_name_size = 0)
        : df(df),
          floatPrecision(8),
          spacing(5),
          max_col_name_size(max_col_name_size),
          max_row_name_size(max_row_name_size),
          excluded_cols({}) {
      cell_color_condition = [](Cell<T>*) { return String(DF_COLOR_W); };
    }

    DF_Logger(const DF_Logger& other)
        : df(other.df),
          floatPrecision(other.floatPrecision),
          spacing(other.spacing),
          max_col_name_size(other.max_col_name_size),
          max_row_name_size(other.max_row_name_size),
          excluded_cols(other.excluded_cols),
          cell_color_condition(other.cell_color_condition) {
    }

    DF_Logger operator=(const DF_Logger& other) {
      if (this != &other) {}
      return *this;
    }
    DataFrame* df;

    sizetype   floatPrecision;
    sizetype   spacing;
    sizetype   max_col_name_size;
    sizetype   max_row_name_size;
    StringList excluded_cols;

    CellLoggingColorCond cell_color_condition;

public:
#ifdef QT_IMPLEMENTATION

    template<typename U = T, std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
    void log(int range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      QDebug dbg       = clog.noquote().nospace();
      int    spacing   = 5;
      int    idx_space = 4;

      int row_name_space = max_row_name_size + spacing;
      int col_spacing    = max_col_name_size + spacing;

      dbg << String("%1").arg("idx", -(max_row_name_size + spacing + idx_space));
      for (const auto& cell : df->row(0)) {
        dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
      }
      dbg << "\n";

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = df->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = df->m_row_count + range;
        range_end   = df->m_row_count;
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const auto& current_row = df->row(idx);
        dbg << String("%1").arg(current_row.idx(), -idx_space) << String("%1").arg(current_row.name(), -row_name_space);
        for (const auto& c : current_row) {
          if (!excluded_cols.contains(c->idx.col_name)) {
            dbg << cell_color_condition(c) << String("%1").arg(c->value, -col_spacing, 'f', floatPrecision)
                << DF_COLOR_W;
          }
        }
        dbg << "\n";
      }
    }

    template<typename U = T, std::enable_if_t<std::is_integral_v<U>, bool> = true>
    void log(int range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      QDebug dbg       = clog.noquote().nospace();
      int    spacing   = 5;
      int    idx_space = 4;

      int row_name_space = max_row_name_size + spacing;
      int col_spacing    = max_col_name_size + spacing;

      dbg << String("%1").arg("idx", -(max_row_name_size + spacing + idx_space));
      for (const auto& cell : df->row(0)) {
        dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
      }
      dbg << "\n";

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = df->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = df->m_row_count + range;
        range_end   = df->m_row_count;
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const auto& current_row = df->row(idx);
        dbg << String("%1").arg(current_row.idx(), -idx_space) << String("%1").arg(current_row.name(), -row_name_space);
        for (const auto& c : current_row) {
          if (!excluded_cols.contains(c->idx.col_name)) {
            dbg << cell_color_condition(c) << String("%1").arg(c->value, -col_spacing) << DF_COLOR_W;
          }
        }
        dbg << "\n";
      }
    }
#else
    void log(long range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      sizetype spacing   = 5;
      sizetype idx_space = 4;

      sizetype row_name_space = m_max_row_name_size + spacing;
      int      col_spacing    = m_max_col_name_size + spacing;

      if (std::is_floating_point_v<T>) {
        clog.precision(m_floatPrecision + 1);
      }

      clog << std::left << std::setw((row_name_space + idx_space)) << "idx";
      for (const auto& [col_name, v] : df->m_col_idx_map) {
        clog << std::left << std::setw(col_spacing) << col_name;
      }
      clog << "\n";

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = df->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = df->m_row_count + range;
        range_end   = df->m_row_count;
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const auto& current_row = df->row(idx);
        clog << std::left << std::setw(idx_space) << current_row.idx() << std ::left << std::setw(row_name_space)
             << current_row.name();
        for (const auto& c : current_row) {
          clog << std::left << std::setw(col_spacing) << c->value;
        }
        clog << "\n";
      }
    }

    friend ostream& operator<<(ostream& os, DF_Logger& logger) {
      sizetype spacing   = 5;
      sizetype idx_space = 4;

      sizetype row_name_space = logger.df->m_max_row_name_size + spacing;
      sizetype col_spacing    = logger.df->m_max_col_name_size + spacing;

      if (std::is_floating_point_v<T>) {
        clog.precision(logger.df->m_floatPrecision + 1);
      }

      os << std::left << std::setw(row_name_space + idx_space) << "idx";
      for (const auto& [col_name, v] : logger.df->m_col_idx_map) {
        os << std::left << std::setw(col_spacing) << col_name;
      }
      os << "\n";

      for (sizetype i = 0; i < logger.df->m_row_count; i++) {
        const auto& row = logger.df->row(i);
        os << std::left << std::setw(idx_space) << row.idx() << std ::left << std::setw(row_name_space) << row.name();
        for (const auto& c : row) {
          clog << std::left << std::setw(col_spacing) << c->value;
        }
        os << "\n";
      }
      return os;
    }
#endif

    DF_Logger& with_exclude_columns(StringList column_names) {
      excluded_cols = column_names;
      return *this;
    }

    DF_Logger& with_exclude_column(String col_name) {
      excluded_cols.push_back(col_name);
      return *this;
    }

    DF_Logger& where(String col_name) {
    }

    DF_Logger& where(std::function<String(String, Cell<T>)> coloring_condition) {
      cell_color_condition = coloring_condition;
    }

    void set_precision(sizetype precision) {
      floatPrecision = precision;
    }

    void set_space_adjustment(sizetype spaceAdjustment) {
      spacing = spaceAdjustment;
    }

    void set_max_col_name_size(sizetype size) {
      max_col_name_size = size;
    }

    void set_max_row_name_size(sizetype size) {
      max_row_name_size = size;
    }

    DF_Logger with_cell_logging_color_condition(CellLoggingColorCond condition) {
      cell_color_condition = condition;
      return *this;
    }
  };

} // namespace df

#endif // DATA_FRAME_LOGGER_H
