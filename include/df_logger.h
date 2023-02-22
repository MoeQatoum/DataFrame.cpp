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
    using CellLoggingColorCond = std::function<String(Cell<T>*)>;
    using CellLoggingPrecCond  = std::function<int(Cell<T>*)>;

    DF_Logger(DataFrame* df)
        : df(df),
          floatPrecision(8),
          spacing(5),
          max_col_name_size(0),
          max_row_name_size(0),
          excluded_cols({}) {
      cell_color_condition     = [](Cell<T>*) { return String(DF_COLOR_W); };
      cell_precision_condition = [this](Cell<T>*) { return floatPrecision; };
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

    int        floatPrecision;
    sizetype   spacing;
    sizetype   max_col_name_size;
    sizetype   max_row_name_size;
    StringList excluded_cols;

    CellLoggingColorCond cell_color_condition;
    CellLoggingPrecCond  cell_precision_condition;

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
        if (!excluded_cols.contains(cell->idx.col_name)) {
          dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
        }
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
        if (!excluded_cols.contains(cell->idx.col_name)) {
          dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
        }
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

    template<typename U = T, std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
    void log_sorted_rows(const List<Row<T>>& sorted_rows, int range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      QDebug dbg       = clog.noquote().nospace();
      int    spacing   = 5;
      int    idx_space = 4;

      int row_name_space = max_row_name_size + spacing;
      int col_spacing    = max_col_name_size + spacing;

      dbg << String("%1").arg("idx", -(max_row_name_size + spacing + idx_space));
      for (const auto& cell : df->row(0)) {
        if (!excluded_cols.contains(cell->idx.col_name)) {
          dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
        }
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
        const Row<T>& current_row = sorted_rows[idx];
        dbg << String("%1").arg(current_row.idx(), -idx_space) << String("%1").arg(current_row.name(), -row_name_space);
        for (const auto& c : current_row) {
          if (!excluded_cols.contains(c->idx.col_name)) {
            dbg << cell_color_condition(c)
                << String("%1").arg(c->value, -col_spacing, 'f', cellCellLoggingPrecCond(c), '0') << DF_COLOR_W;
          }
        }
        dbg << "\n";
      }
    }

    template<typename U = T, std::enable_if_t<std::is_integral_v<U>, bool> = true>
    void log_sorted_rows(const List<Row<T>>& sorted_rows, int range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      QDebug dbg       = clog.noquote().nospace();
      int    spacing   = 5;
      int    idx_space = 4;

      int row_name_space = max_row_name_size + spacing;
      int col_spacing    = max_col_name_size + spacing;

      dbg << String("%1").arg("idx", -(max_row_name_size + spacing + idx_space));
      for (const auto& cell : df->row(0)) {
        if (!excluded_cols.contains(cell->idx.col_name)) {
          dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
        }
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
        const Row<T>& current_row = sorted_rows[idx];
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
    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    void log(int range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      sizetype spacing   = 5;
      sizetype idx_space = 4;

      sizetype row_name_space = max_row_name_size + spacing;
      int      col_spacing    = max_col_name_size + spacing;

      auto contains = [](const StringList& excluded_cols_list, const String& cur_col_name) {
        return std::find_if(excluded_cols_list.begin(),
                            excluded_cols_list.end(),
                            [cur_col_name](String col_name) { return cur_col_name == col_name; })
               != excluded_cols_list.end();
      };

      if (std::is_floating_point_v<T>) {
        clog.precision(floatPrecision + 1);
      }

      clog << std::left << std::setw(row_name_space + idx_space) << "idx";
      for (const auto& [col_name, v] : df->m_col_idx_map) {
        if (!contains(excluded_cols, col_name)) {
          clog << std::left << std::setw(col_spacing) << col_name;
        }
      }
      clog << "\n";

      int range_start;
      int range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = df->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = range;
      } else {
        range_start = df->m_row_count + range;
        range_end   = df->m_row_count;
      }

      for (int idx = range_start; idx < range_end; idx++) {
        const auto& current_row = df->row(idx);
        clog << std::left << std::setw(idx_space) << current_row.idx() << std ::left << std::setw(row_name_space)
             << current_row.name();
        for (const auto& cell : current_row) {
          if (!contains(excluded_cols, cell->idx.col_name)) {
            clog << cell_color_condition(cell) << std::left << std::setw(col_spacing) << cell->value << DF_COLOR_W;
          }
        }
        clog << "\n";
      }
    }

    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    void log_sorted_rows(const List<Row<T>>& sorted_rows, int range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      sizetype spacing   = 5;
      sizetype idx_space = 4;

      sizetype row_name_space = max_row_name_size + spacing;
      int      col_spacing    = max_col_name_size + spacing;

      auto contains = [](const StringList& excluded_cols_list, const String& cur_col_name) {
        return std::find_if(excluded_cols_list.begin(),
                            excluded_cols_list.end(),
                            [cur_col_name](String col_name) { return cur_col_name == col_name; })
               != excluded_cols_list.end();
      };

      if (std::is_floating_point_v<T>) {
        clog.precision(floatPrecision + 1);
      }

      clog << std::left << std::setw(row_name_space + idx_space) << "idx";
      for (const auto& [col_name, v] : df->m_col_idx_map) {
        if (!contains(excluded_cols, col_name)) {
          clog << std::left << std::setw(col_spacing) << col_name;
        }
      }
      clog << "\n";

      int range_start;
      int range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = df->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = range;
      } else {
        range_start = df->m_row_count + range;
        range_end   = df->m_row_count;
      }

      for (int idx = range_start; idx < range_end; idx++) {
        const Row<T>& current_row = sorted_rows[idx];
        clog << std::left << std::setw(idx_space) << current_row.idx() << std ::left << std::setw(row_name_space)
             << current_row.name();
        for (const auto& cell : current_row) {
          if (!contains(excluded_cols, cell->idx.col_name)) {
            clog << cell_color_condition(cell) << std::left << std::setw(col_spacing) << cell->value << DF_COLOR_W;
          }
        }
        clog << "\n";
      }
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

    DF_Logger with_cell_color_condition(CellLoggingColorCond condition) {
      cell_color_condition = condition;
      return *this;
    }

    DF_Logger with_cell_precision_condition(CellLoggingPrecCond condition) {
      cell_precision_condition = condition;
      return *this;
    }
  };

} // namespace df

#endif // DATA_FRAME_LOGGER_H
