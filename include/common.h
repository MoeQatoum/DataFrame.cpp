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

template<typename T>
concept NumericalTypes = std::is_same<T, short>::value || std::is_same<T, long>::value || std::is_same<T, int>::value
                         || std::is_same<T, long long>::value || std::is_same<T, float>::value
                         || std::is_same<T, double>::value || std::is_same<T, long double>::value;

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

#endif // DATA_FRAME_COMMON_H
