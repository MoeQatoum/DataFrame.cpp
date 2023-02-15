#ifndef DATA_FRAME_UTILS_H
#define DATA_FRAME_UTILS_H

#include "df_common.h"

namespace df {
  template<typename T>
  class DataFrame;

  template<typename T>
  class RowSeries;

  template<typename T>
  class ColumnSeries;

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  List<RowSeries<T>> asc_sort_rows(DataFrame<T>& df, const String& col_name) {
    using ValueType = typename ColumnSeries<T>::ValueType;

    sizetype        col_idx = df.get_col_idx(col_name);
    ColumnSeries<T> col     = df.get_column(col_name);

    ValueType* sorted_cells = new ValueType[col.size()];

    List<RowSeries<T>> rows;
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
    List<RowSeries<T>> sorted_rows;
    for (sizetype idx = 0; idx < col.size(); idx++) {
      sorted_rows.push_back(df.get_row(sorted_cells[idx]->idx.row_idx));
    }

    delete[] sorted_cells;
    return sorted_rows;
  }

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  List<RowSeries<T>> dec_sort_rows(DataFrame<T>& df, const String& col_name) {
    using ValueType = typename ColumnSeries<T>::ValueType;

    sizetype        col_idx = df.get_col_idx(col_name);
    ColumnSeries<T> col     = df.get_column(col_name);

    ValueType* sorted_cells = new ValueType[col.size()];

    List<RowSeries<T>> rows;
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
    List<RowSeries<T>> sorted_rows;
    for (sizetype idx = 0; idx < col.size(); idx++) {
      sorted_rows.push_back(df.get_row(sorted_cells[idx]->idx.row_idx));
    }

    delete[] sorted_cells;
    return sorted_rows;
  }

  template<typename T, std::enable_if_t<std::is_arithmetic_v<T>, bool> = true>
  void log_sorted_rows(const List<RowSeries<T>>& sorted_rows, DataFrame<T>& df, long range = 0) {

    DF_ASSERT(tail > m_row_count && tail >= 1, "tail is grater then row count");

#ifdef QT_IMPLEMENTATION
    int spacing   = 5;
    int idx_space = 4;

    int row_name_space = df.max_row_name_size() + spacing;
    int col_spacing    = df.max_col_name_size() + spacing;

    QDebug dbg = clog.noquote().nospace();

    dbg << String("%1").arg("idx", -(df.max_row_name_size() + spacing + idx_space));
    for (auto& c : sorted_rows[0]) {
      dbg << String("%1").arg(c->idx.col_name, -(df.max_col_name_size() + spacing));
    }
    dbg << "\n";

    qsizetype range_start;
    qsizetype range_end;
    if (range == 0) {
      range_start = 0;
      range_end   = sorted_rows.size();
    } else if (range > 0) {
      range_start = 0;
      range_end   = static_cast<qsizetype>(range);
    } else {
      range_start = sorted_rows.size() + static_cast<qsizetype>(range);
      range_end   = sorted_rows.size();
    }

    for (qsizetype idx = range_start; idx < range_end; idx++) {
      const RowSeries<T>& row = sorted_rows[idx];
      dbg << String("%1").arg(row.idx(), -idx_space) << String("%1").arg(row.name(), -(row_name_space));
      for (const auto& c : row) {
        if (std::is_floating_point_v<T>) {
          dbg << String("%1").arg(String::number(c->value, 'f', df.floatPrecision()),
                                  -(df.floatPrecision() + col_spacing));
        } else {
          dbg << String("%1").arg(c->value, -(col_spacing));
        }
      }
      dbg << "\n";
    }
#else
    sizetype spacing   = 5;
    sizetype idx_space = 4;

    sizetype row_name_space = df.max_row_name_size() + spacing;
    sizetype col_spacing    = df.max_col_name_size() + spacing;

    clog << std::left << std::setw((df.max_row_name_size() + spacing + idx_space)) << "idx";
    for (auto& c : sorted_rows[0]) {
      clog << std::left << std::setw(df.max_col_name_size() + spacing) << c->idx.col_name;
    }
    clog << "\n";

    sizetype range_start;
    sizetype range_end;
    if (range == 0) {
      range_start = 0;
      range_end   = sorted_rows.size();
    } else if (range > 0) {
      range_start = 0;
      range_end   = static_cast<sizetype>(range);
    } else {
      range_start = sorted_rows.size() + static_cast<sizetype>(range);
      range_end   = sorted_rows.size();
    }

    for (sizetype idx = range_start; idx < range_end; idx++) {
      const RowSeries<T>& row = sorted_rows[idx];
      clog << std::left << std::setw(idx_space) << row.idx() << std ::left << std::setw(row_name_space) << row.name();
      for (const auto& c : row) {
        if (std::is_floating_point_v<T>) {
          clog.precision(df.floatPrecision());
          clog << std::left << std::setw(col_spacing + df.floatPrecision()) << c->value;
          clog.precision(0);
        } else {
          clog << std::left << std::setw(col_spacing) << c->value;
        }
      }
      clog << "\n";
    }
#endif
  }

} // namespace df

#endif // DATA_FRAME_UTILS_H
