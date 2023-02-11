#ifndef DATA_FRAME_COMMON_H
#define DATA_FRAME_COMMON_H

#include <algorithm>
#include <concepts>
#include <iostream>
#include <map>
#include <optional>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <vector>

#ifndef NDEBUG
  #define DF_ASSERT(condition, message)                                                                             \
    do {                                                                                                            \
      if (!(condition)) {                                                                                           \
        std::cerr << "Assertion `" #condition "` failed in " << __FILE__ << " line " << __LINE__ << ": " << message \
                  << std::endl;                                                                                     \
        abort();                                                                                                    \
      }                                                                                                             \
    } while (false)
#else
  #define DF_ASSERT(condition, message) \
    do {                                \
    } while (false)
#endif

// numerical types

using df_i8   = char;
using df_ui8  = unsigned char;
using df_i16  = short;
using df_ui16 = unsigned short;
using df_i32  = long;
using df_ui32 = unsigned long;
using df_i64  = long long;
using df_ui64 = unsigned long long;
using df_f32  = float;
using df_f64  = double;
using df_f128 = long double;

template<typename T>
concept NumericalTypes
  = std::is_same<T, df_i8>::value || std::is_same<T, df_ui8>::value || std::is_same<T, df_i16>::value
    || std::is_same<T, df_ui16>::value || std::is_same<T, df_i32>::value || std::is_same<T, df_ui32>::value
    || std::is_same<T, df_i64>::value || std::is_same<T, df_ui64>::value || std::is_same<T, df_f32>::value
    || std::is_same<T, df_f64>::value || std::is_same<T, df_f128>::value;

using size_t = df_ui32;

#endif // DATA_FRAME_COMMON_H
