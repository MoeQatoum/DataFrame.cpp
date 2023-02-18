#ifndef DATA_FRAME_UTILS_H
#define DATA_FRAME_UTILS_H

#include "df_common.h"

namespace df {
  template<typename T>
  class Cell;

  template<typename T>
  class DataFrame;

  template<typename T>
  class Row;

  template<typename T>
  class Column;

  template<typename T>
  void fill_df(DataFrame<T>& df, T fill_value) {
    for (auto& c : df) {
      c.value = fill_value;
    }
  }

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  List<Row<T>> asc_sort_rows(DataFrame<T>& df, const String& col_name) {
    using ValueType = typename Column<T>::ValueType;

    Column<T> col = df.column(col_name);

    ValueType* sorted_cells = new ValueType[col.size()];

    List<Row<T>> rows;
    for (auto row_iterator = df.iter_rows(); row_iterator < df.end(); row_iterator++) {
      rows.push_back(row_iterator.current_row());
    }

    sorted_cells[0] = col[0];

    for (sizetype idx = 0; idx < col.size(); idx++) {
      bool     lower_found = false;
      sizetype insert_idx  = 0;
      for (sizetype sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
        if (col[idx]->value < sorted_cells[sorted_idx]->value) {
          lower_found = true;
          insert_idx  = sorted_idx;
          break;
        }
      }
      if (lower_found) {
        for (sizetype i = idx - 1; i >= insert_idx; i--) {
          sorted_cells[i + 1] = sorted_cells[i];

          // avoidsizetype (aka df_ui32) underflow
          if (i == 0) {
            break;
          }
        }
        sorted_cells[insert_idx] = col[idx];
      } else {
        sorted_cells[idx] = col[idx];
      }
    }

    // sort
    List<Row<T>> sorted_rows;
    for (sizetype idx = 0; idx < col.size(); idx++) {
      sorted_rows.push_back(df.row(sorted_cells[idx]->idx.row_idx));
    }

    delete[] sorted_cells;
    return sorted_rows;
  }

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  List<Row<T>> dec_sort_rows(DataFrame<T>& df, const String& col_name) {
    using ValueType = typename Column<T>::ValueType;

    Column<T> col = df.column(col_name);

    ValueType* sorted_cells = new ValueType[col.size()];

    List<Row<T>> rows;
    for (auto row_iterator = df.iter_rows(); row_iterator < df.end(); row_iterator++) {
      rows.push_back(row_iterator.current_row());
    }

    sorted_cells[0] = col[0];

    for (sizetype idx = 0; idx < col.size(); idx++) {
      bool     higher_found = false;
      sizetype insert_idx   = 0;
      for (sizetype sorted_idx = 0; sorted_idx < idx; sorted_idx++) {
        if (col[idx]->value > sorted_cells[sorted_idx]->value) {
          higher_found = true;
          insert_idx   = sorted_idx;
          break;
        }
      }
      if (higher_found) {
        for (sizetype i = idx - 1; i >= insert_idx; i--) {
          sorted_cells[i + 1] = sorted_cells[i];

          // avoidsizetype (aka df_ui32) underflow
          if (i == 0) {
            break;
          }
        }
        sorted_cells[insert_idx] = col[idx];
      } else {
        sorted_cells[idx] = col[idx];
      }
    }

    // sort
    List<Row<T>> sorted_rows;
    for (sizetype idx = 0; idx < col.size(); idx++) {
      sorted_rows.push_back(df.row(sorted_cells[idx]->idx.row_idx));
    }

    delete[] sorted_cells;
    return sorted_rows;
  }

#ifdef QT_IMPLEMENTATION
  template<typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
  void
    log_sorted_rows(const List<Row<T>>& sorted_rows, DataFrame<T>& df, int range = 0, StringList excluded_cols = {}) {

    DF_ASSERT(range <= df.row_count() || range >= -df.row_count(), "out of allowed range");

    std::function<bool(StringList&, String&)> contains
      = [](StringList& excluded_cols, String& cur_col_name) { return excluded_cols.contains(cur_col_name); };

    int spacing   = 5;
    int idx_space = 4;

    int row_name_space = df.max_row_name_size() + spacing;
    int col_spacing    = df.max_col_name_size() + spacing;

    QDebug dbg = clog.noquote().nospace();

    dbg << String("%1").arg("idx", -(row_name_space + idx_space));
    for (auto& c : sorted_rows[0]) {
      if (!contains(excluded_cols, c->idx.col_name)) {
        dbg << String("%1").arg(c->idx.col_name, -(col_spacing));
      }
    }
    dbg << "\n";

    int range_start;
    int range_end;
    if (range == 0) {
      range_start = 0;
      range_end   = sorted_rows.size();
    } else if (range > 0) {
      range_start = 0;
      range_end   = range;
    } else {
      range_start = sorted_rows.size() + range;
      range_end   = sorted_rows.size();
    }

    for (int idx = range_start; idx < range_end; idx++) {
      const Row<T>& row = sorted_rows[idx];
      dbg << String("%1").arg(row.idx(), -idx_space) << String("%1").arg(row.name(), -(row_name_space));
      for (const auto& c : row) {
        if (!contains(excluded_cols, c->idx.col_name)) {
          dbg << String("%1").arg(c->value, -(col_spacing), 'f', df.floatPrecision());
        }
      }
      dbg << "\n";
    }
  }

  template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
  void log_sorted_rows(const List<Row<T>>& sorted_rows, DataFrame<T>& df, int range = 0) {

    DF_ASSERT(range <= df.row_count() || range >= -df.row_count(), "out of allowed range");

    int spacing   = 5;
    int idx_space = 4;

    int row_name_space = df.max_row_name_size() + spacing;
    int col_spacing    = df.max_col_name_size() + spacing;

    QDebug dbg = clog.noquote().nospace();

    dbg << String("%1").arg("idx", -(row_name_space + idx_space));
    for (auto& c : sorted_rows[0]) {
      dbg << String("%1").arg(c->idx.col_name, -(col_spacing));
    }
    dbg << "\n";

    int range_start;
    int range_end;
    if (range == 0) {
      range_start = 0;
      range_end   = sorted_rows.size();
    } else if (range > 0) {
      range_start = 0;
      range_end   = range;
    } else {
      range_start = sorted_rows.size() + range;
      range_end   = sorted_rows.size();
    }

    for (int idx = range_start; idx < range_end; idx++) {
      const Row<T>& row = sorted_rows[idx];
      dbg << String("%1").arg(row.idx(), -idx_space) << String("%1").arg(row.name(), -(row_name_space));
      for (const auto& c : row) {
        dbg << String("%1").arg(c->value, -(col_spacing));
      }
      dbg << "\n";
    }
  }
#else
  template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  void
    log_sorted_rows(const List<Row<T>>& sorted_rows, DataFrame<T>& df, int range = 0, StringList excluded_cols = {}) {

    sizetype spacing   = 5;
    sizetype idx_space = 4;

    sizetype row_name_space = df.max_row_name_size() + spacing;
    sizetype col_spacing    = df.max_col_name_size() + spacing;

    if (std::is_floating_point_v<T>) {
      clog.precision(df.floatPrecision() + 1);
    }

    clog << std::left << std::setw(row_name_space + idx_space) << "idx";
    for (auto& c : sorted_rows[0]) {
      clog << std::left << std::setw(col_spacing) << c->idx.col_name;
    }
    clog << "\n";

    int range_start;
    int range_end;
    if (range == 0) {
      range_start = 0;
      range_end   = sorted_rows.size();
    } else if (range > 0) {
      range_start = 0;
      range_end   = range;
    } else {
      range_start = sorted_rows.size() + range;
      range_end   = sorted_rows.size();
    }

    std::function<bool(StringList&, String&)> contains = [](StringList& excluded_cols_list, String& cur_col_name) {
      return std::find_if(excluded_cols_list.begin(),
                          excluded_cols_list.end(),
                          [cur_col_name](String col_name) { return cur_col_name == col_name; })
             == excluded_cols_list.end();
    };

    for (int idx = range_start; idx < range_end; idx++) {
      const Row<T>& row = sorted_rows[idx];
      clog << std::left << std::setw(idx_space) << row.idx() << std ::left << std::setw(row_name_space) << row.name();
      for (const auto& c : row) {
        if (contains(excluded_cols, c->idx.col_name)) {
          clog << std::left << std::setw(col_spacing) << c->value;
        }
      }
      clog << "\n";
    }
  }
#endif

} // namespace df

#endif // DATA_FRAME_UTILS_H
