#ifndef DATA_FRAME_UTILS_H
#define DATA_FRAME_UTILS_H

#include "df_common.hpp"

namespace df {
    template<typename T>
    class Series;

    template<typename T>
    class DataFrame;

    template<typename T>
    void fill_df(DataFrame<T>& df, T fill_value)
        requires(std::assignable_from<T&, T>)
    {
        for (auto& c : df) {
            c.value = fill_value;
        }
    }

    template<typename T>
    void fill_series(Series<T>& series, const T& value)
        requires(std::assignable_from<T&, T>)
    {
        for (std::size_t i = 0; i < series.size(); i++) {
            series[i] = value;
        }
    }

} // namespace df

#endif // DATA_FRAME_UTILS_H
