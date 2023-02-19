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
    using ColName              = String;
    using CellLoggingColorCond = std::function<df::String(ColName, Cell<T>)>;

    DF_Logger(DataFrame* df, sizetype max_col_name_size = 0, sizetype max_row_name_size = 0)
        : m_df(df),
          m_floatPrecision(8),
          m_spacing(5),
          m_max_col_name_size(max_col_name_size),
          m_max_row_name_size(max_row_name_size),
          m_excluded_cols({}) {
      m_cell_color_condition = [](ColName, Cell<T>) { return String("\033[0;00m"); };
    }

    DF_Logger(const DF_Logger& other)
        : m_df(other.m_df),
          m_floatPrecision(other.m_floatPrecision),
          m_spacing(other.m_spacing),
          m_max_col_name_size(other.m_max_col_name_size),
          m_max_row_name_size(other.m_max_row_name_size),
          m_excluded_cols(other.m_excluded_cols),
          m_cell_color_condition(other.m_cell_color_condition) {
    }

    DF_Logger operator=(const DF_Logger& other) {
      if (this != &other) {}
      return *this;
    }
#ifdef QT_IMPLEMENTATION

    template<typename U = T, std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
    void log(int range = 0) {
      DF_ASSERT(range <= m_df->m_row_count || range >= -m_df->m_row_count, "range is grater then row count");

      QDebug dbg       = clog.noquote().nospace();
      int    spacing   = 5;
      int    idx_space = 4;

      int row_name_space = m_max_row_name_size + spacing;
      int col_spacing    = m_max_col_name_size + spacing;

      dbg << String("%1").arg("idx", -(m_max_row_name_size + spacing + idx_space));
      for (const auto& cell : m_df->row(0)) {
        dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
      }
      dbg << "\n";

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = m_df->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = m_df->m_row_count + range;
        range_end   = m_df->m_row_count;
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const auto& current_row = m_df->row(idx);
        dbg << String("%1").arg(current_row.idx(), -idx_space)
            << String("%1").arg(current_row.name(), -(row_name_space));
        for (const auto& c : current_row) {
          dbg << String("%1").arg(c->value, -(col_spacing), 'f', m_floatPrecision);
        }
        dbg << "\n";
      }
    }

    template<typename U = T, std::enable_if_t<std::is_integral_v<U>, bool> = true>
    void log(int range = 0) {
      DF_ASSERT(range <= m_df->m_row_count || range >= -m_df->m_row_count, "range is grater then row count");

      QDebug dbg       = clog.noquote().nospace();
      int    spacing   = 5;
      int    idx_space = 4;

      int row_name_space = m_max_row_name_size + spacing;
      int col_spacing    = m_max_col_name_size + spacing;

      dbg << String("%1").arg("idx", -(m_max_row_name_size + spacing + idx_space));
      for (const auto& cell : m_df->row(0)) {
        dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
      }
      dbg << "\n";

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = m_df->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = m_df->m_row_count + range;
        range_end   = m_df->m_row_count;
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const auto& current_row = m_df->row(idx);
        dbg << String("%1").arg(current_row.idx(), -idx_space) << String("%1").arg(current_row.name(), -row_name_space);
        for (const auto& c : current_row) {
          dbg << String("%1").arg(c->value, -col_spacing);
        }
        dbg << "\n";
      }
    }
#else
    void log(long range = 0) {
      DF_ASSERT(range <= m_df->m_row_count || range >= -m_df->m_row_count, "range is grater then row count");

      sizetype spacing   = 5;
      sizetype idx_space = 4;

      sizetype row_name_space = m_max_row_name_size + spacing;
      int      col_spacing    = m_max_col_name_size + spacing;

      if (std::is_floating_point_v<T>) {
        clog.precision(m_floatPrecision + 1);
      }

      clog << std::left << std::setw((row_name_space + idx_space)) << "idx";
      for (const auto& [col_name, v] : m_df->m_col_idx_map) {
        clog << std::left << std::setw(col_spacing) << col_name;
      }
      clog << "\n";

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = m_df->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = m_df->m_row_count + range;
        range_end   = m_df->m_row_count;
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const auto& current_row = m_df->row(idx);
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

      sizetype row_name_space = logger.m_df->m_max_row_name_size + spacing;
      sizetype col_spacing    = logger.m_df->m_max_col_name_size + spacing;

      if (std::is_floating_point_v<T>) {
        clog.precision(logger.m_df->m_floatPrecision + 1);
      }

      os << std::left << std::setw(row_name_space + idx_space) << "idx";
      for (const auto& [col_name, v] : logger.m_df->m_col_idx_map) {
        os << std::left << std::setw(col_spacing) << col_name;
      }
      os << "\n";

      for (sizetype i = 0; i < logger.m_df->m_row_count; i++) {
        const auto& row = logger.m_df->row(i);
        os << std::left << std::setw(idx_space) << row.idx() << std ::left << std::setw(row_name_space) << row.name();
        for (const auto& c : row) {
          clog << std::left << std::setw(col_spacing) << c->value;
        }
        os << "\n";
      }
      return os;
    }
#endif

    DF_Logger& exclude_columns(StringList column_names) {
      m_excluded_cols = column_names;
    }

    DF_Logger& exclude_column(String col_name) {
      m_excluded_cols.push_back(col_name);
    }

    DF_Logger& where(String col_name) {
    }

    DF_Logger& where(std::function<String(String, Cell<T>)> coloring_condition) {
      m_cell_color_condition = coloring_condition;
    }

    void setFloatPrecision(sizetype precision) {
      m_floatPrecision = precision;
    }

    sizetype floatPrecision() {
      return m_floatPrecision;
    }

    void setSpaceAdjustment(sizetype spaceAdjustment) {
      m_spacing = spaceAdjustment;
    }

    sizetype spaceAdjustment() {
      return m_spacing;
    }

    void set_max_col_name_size(sizetype size) {
      m_max_col_name_size = size;
    }

    sizetype max_col_name_size() {
      return m_max_col_name_size;
    }

    void set_max_row_name_size(sizetype size) {
      m_max_row_name_size = size;
    }

    sizetype max_row_name_size() {
      return m_max_row_name_size;
    }

    DataFrame* m_df;

    sizetype   m_floatPrecision;
    sizetype   m_spacing;
    sizetype   m_max_col_name_size;
    sizetype   m_max_row_name_size;
    StringList m_excluded_cols;

    CellLoggingColorCond m_cell_color_condition;
  };

} // namespace df

#endif // DATA_FRAME_LOGGER_H
