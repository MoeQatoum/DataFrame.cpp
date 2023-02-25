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
  class RowGroup;

  template<typename T>
  class Logger {

protected:
    using CellLoggingColorCond = std::function<String(const Cell<T>*)>;
    using RowNameColorCond     = std::function<String(const Row<T>*)>;
    using CellLoggingPrecCond  = std::function<int(const Cell<T>*)>;

    Logger() : floatPrecision(8), spacing(5), max_col_name_size(0), max_row_name_size(0), excluded_cols({}) {
      cell_color_condition     = [](const Cell<T>*) { return String(DF_COLOR_W); };
      row_name_color_condition = [](const Row<T>*) { return String(DF_COLOR_W); };
      cell_precision_condition = [](const Cell<T>*) { return 8; };
    }

    Logger(const Logger& other)
        : floatPrecision(other.floatPrecision),
          spacing(other.spacing),
          max_col_name_size(other.max_col_name_size),
          max_row_name_size(other.max_row_name_size),
          excluded_cols(other.excluded_cols),
          cell_color_condition(other.cell_color_condition),
          cell_precision_condition(other.cell_precision_condition),
          row_name_color_condition(other.row_name_color_condition) {
    }

    Logger operator=(const Logger& other) {
      if (this != &other) {}
      return *this;
    }

protected:
    int        floatPrecision;
    int        spacing;
    sizetype   max_col_name_size;
    sizetype   max_row_name_size;
    StringList excluded_cols;

    CellLoggingColorCond cell_color_condition;
    RowNameColorCond     row_name_color_condition;
    CellLoggingPrecCond  cell_precision_condition;

public:
    void with_exclude_columns(StringList column_names) {
      excluded_cols = column_names;
    }

    void with_exclude_column(String col_name) {
      excluded_cols.push_back(col_name);
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

    void with_cell_color_condition(CellLoggingColorCond condition) {
      cell_color_condition = condition;
    }

    void with_row_color_condition(RowNameColorCond condition) {
      row_name_color_condition = condition;
    }

    void with_cell_precision_condition(CellLoggingPrecCond condition) {
      cell_precision_condition = condition;
    }
  };

  template<typename T>
  class DF_Logger : public Logger<T> {
    friend class DataFrame<T>;

    DF_Logger(DataFrame<T>* df) : Logger<T>(), df(df) {
    }

    DF_Logger(const DF_Logger& other) : Logger<T>(), df(other.df) {
    }

    DataFrame<T>* df;

public:
#ifdef QT_IMPLEMENTATION
    template<typename U = T, std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
    void log(int range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      QDebug dbg       = clog.noquote().nospace();
      int    idx_space = 4;

      int row_name_space = this->max_row_name_size + this->spacing;
      int col_spacing    = this->max_col_name_size + this->spacing;

      dbg << String("%1").arg("idx", -(this->max_row_name_size + this->spacing + idx_space));
      for (const auto& cell : df->row(0)) {
        if (!this->excluded_cols.contains(cell->idx.col_name)) {
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
        dbg << String("%1").arg(current_row.index(), -idx_space) << this->row_name_color_condition(&current_row)
            << String("%1").arg(current_row.name(), -row_name_space) << DF_COLOR_W;
        for (const auto& c : current_row) {
          if (!this->excluded_cols.contains(c->idx.col_name)) {
            dbg << this->cell_color_condition(c)
                << String("%1").arg(c->value, -col_spacing, 'f', this->cell_precision_condition(c)) << DF_COLOR_W;
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

      int row_name_space = this->max_row_name_size + this->spacing;
      int col_spacing    = this->max_col_name_size + this->spacing;

      dbg << String("%1").arg("idx", -(this->max_row_name_size + spacing + idx_space));
      for (const auto& cell : df->row(0)) {
        if (!this->excluded_cols.contains(cell->idx.col_name)) {
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
        dbg << String("%1").arg(current_row.index(), -idx_space) << this->row_name_color_condition(&current_row)
            << String("%1").arg(current_row.name(), -row_name_space) << DF_COLOR_W;
        for (const auto& c : current_row) {
          if (!this->excluded_cols.contains(c->idx.col_name)) {
            dbg << this->cell_color_condition(c) << String("%1").arg(c->value, -col_spacing) << DF_COLOR_W;
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

      sizetype row_name_space = this->max_row_name_size + spacing;
      int      col_spacing    = this->max_col_name_size + spacing;

      auto contains = [](const StringList& excluded_cols_list, const String& cur_col_name) {
        return std::find_if(excluded_cols_list.begin(),
                            excluded_cols_list.end(),
                            [cur_col_name](String col_name) { return cur_col_name == col_name; })
               != excluded_cols_list.end();
      };

      if (std::is_floating_point_v<T>) {
        clog.precision(this->floatPrecision + 1);
      }

      clog << std::left << std::setw(row_name_space + idx_space) << "idx";
      for (const auto& [col_name, v] : this->df->m_col_idx_map) {
        if (!contains(this->excluded_cols, col_name)) {
          clog << std::left << std::setw(col_spacing) << col_name;
        }
      }
      clog << "\n";

      int range_start;
      int range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = this->df->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = range;
      } else {
        range_start = this->df->m_row_count + range;
        range_end   = this->df->m_row_count;
      }

      for (int idx = range_start; idx < range_end; idx++) {
        const auto& current_row = this->df->row(idx);
        clog << std::left << std::setw(idx_space) << current_row.index() << std ::left << std::setw(row_name_space)
             << current_row.name();
        for (const auto& cell : current_row) {
          if (!contains(this->excluded_cols, cell->idx.col_name)) {
            clog << this->cell_color_condition(cell) << std::left << std::setw(col_spacing) << cell->value
                 << DF_COLOR_W;
          }
        }
        clog << "\n";
      }
    }
#endif
  };

  template<typename T>
  class RowGroup_Logger : Logger<T> {
    friend class RowGroup<T>;

    RowGroup_Logger(RowGroup<T>* rg) : Logger<T>(), rg(rg) {
    }

    RowGroup_Logger(const RowGroup_Logger& other) : Logger<T>(), rg(other.rg) {
    }

    RowGroup<T>* rg;

public:
#ifdef QT_IMPLEMENTATION
    template<typename U = T, std::enable_if_t<std::is_floating_point_v<U>, bool> = true>
    void log(int range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      QDebug dbg       = clog.noquote().nospace();
      int    idx_space = 4;

      int row_name_space = this->max_row_name_size + this->spacing;
      int col_spacing    = this->max_col_name_size + this->spacing;

      dbg << String("%1").arg("idx", -(this->max_row_name_size + this->spacing + idx_space));
      for (const auto& cell : rg->row(0)) {
        if (!this->excluded_cols.contains(cell->idx.col_name)) {
          dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
        }
      }
      dbg << "\n";

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = rg->m_row_count;
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = rg->size() + range;
        range_end   = rg->size();
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const Row<T>& current_row = rg->at(idx);
        dbg << String("%1").arg(current_row.index(), -idx_space) << this->row_name_color_condition(&current_row)
            << String("%1").arg(current_row.name(), -row_name_space) << DF_COLOR_W;
        for (const auto& c : current_row) {
          if (!this->excluded_cols.contains(c->idx.col_name)) {
            dbg << this->cell_color_condition(c)
                << String("%1").arg(c->value, -col_spacing, 'f', this->cell_precision_condition(c)) << DF_COLOR_W;
          }
        }
        dbg << "\n";
      }
    }

    template<typename U = T, std::enable_if_t<std::is_integral_v<U>, bool> = true>
    void log(int range = 0) {
      DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      QDebug dbg       = clog.noquote().nospace();
      int    idx_space = 4;

      int row_name_space = this->max_row_name_size + this->spacing;
      int col_spacing    = this->max_col_name_size + this->spacing;

      dbg << String("%1").arg("idx", -(this->max_row_name_size + this->spacing + idx_space));
      for (const auto& cell : rg->at(0)) {
        if (!this->excluded_cols.contains(cell->idx.col_name)) {
          dbg << String("%1").arg(cell->idx.col_name, -col_spacing);
        }
      }
      dbg << "\n";

      sizetype range_start;
      sizetype range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = rg->size();
      } else if (range > 0) {
        range_start = 0;
        range_end   = static_cast<sizetype>(range);
      } else {
        range_start = rg->size() + range;
        range_end   = rg->size();
      }

      for (sizetype idx = range_start; idx < range_end; idx++) {
        const Row<T>& current_row = rg->at(idx);
        dbg << String("%1").arg(current_row.index(), -idx_space) << this->row_name_color_condition(&current_row)
            << String("%1").arg(current_row.name(), -row_name_space) << DF_COLOR_W;
        for (const auto& c : current_row) {
          if (!this->excluded_cols.contains(c->idx.col_name)) {
            dbg << this->cell_color_condition(c) << String("%1").arg(c->value, -col_spacing) << DF_COLOR_W;
          }
        }
        dbg << "\n";
      }
    }
#else
    template<typename U = T, std::enable_if_t<std::is_arithmetic_v<U>, bool> = true>
    void log(int range = 0) {
      // DF_ASSERT(range <= df->m_row_count || range >= -df->m_row_count, "range is grater then row count");

      sizetype idx_space      = 4;
      sizetype row_name_space = this->max_row_name_size + this->spacing;
      int      col_spacing    = this->max_col_name_size + this->spacing;

      auto contains = [](const StringList& excluded_cols_list, const String& cur_col_name) {
        return std::find_if(excluded_cols_list.begin(),
                            excluded_cols_list.end(),
                            [cur_col_name](String col_name) { return cur_col_name == col_name; })
               != excluded_cols_list.end();
      };

      if (std::is_floating_point_v<T>) {
        clog.precision(this->floatPrecision + 1);
      }

      clog << std::left << std::setw(row_name_space + idx_space) << "idx";
      for (const auto& c : rg->at(0)) {
        if (!contains(this->excluded_cols, c->idx.col_name)) {
          clog << std::left << std::setw(col_spacing) << c->idx.col_name;
        }
      }
      clog << "\n";

      int range_start;
      int range_end;
      if (range == 0) {
        range_start = 0;
        range_end   = rg->size();
      } else if (range > 0) {
        range_start = 0;
        range_end   = range;
      } else {
        range_start = rg->size() + range;
        range_end   = rg->size();
      }

      for (int idx = range_start; idx < range_end; idx++) {
        const Row<T>& current_row = rg->at(idx);
        clog << std::left << std::setw(idx_space) << current_row.index() << this->row_name_color_condition(&current_row)
             << std ::left << std::setw(row_name_space) << current_row.name() << DF_COLOR_W;
        for (const auto& cell : current_row) {
          if (!contains(this->excluded_cols, cell->idx.col_name)) {
            clog << this->cell_color_condition(cell) << std::left << std::setw(col_spacing) << cell->value
                 << DF_COLOR_W;
          }
        }
        clog << "\n";
      }
    }
#endif
  };

} // namespace df

#endif // DATA_FRAME_LOGGER_H
