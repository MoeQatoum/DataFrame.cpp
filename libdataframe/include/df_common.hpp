#ifndef DATA_FRAME_COMMON_H
#define DATA_FRAME_COMMON_H

#include <concepts>
#include <optional>

#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <ostream>
#include <stdlib.h>
#include <string>
#include <vector>

#define DF_COLOR_R "\033[91m"
#define DF_COLOR_G "\033[92m"
#define DF_COLOR_C "\033[96m"
#define DF_COLOR_Y "\033[93m"
#define DF_COLOR_M "\033[35m"
#define DF_COLOR_W "\033[00m"

#ifndef NDEBUG
    #define DF_ASSERT(condition, message)                                                                                                       \
        do {                                                                                                                                    \
            if (!(condition)) {                                                                                                                 \
                std::cerr << "Assertion `" #condition "` failed in " << __FILE__ << " line " << __LINE__ << " function " << __PRETTY_FUNCTION__ \
                          << ": " << message << std::endl;                                                                                      \
                abort();                                                                                                                        \
            }                                                                                                                                   \
        } while (false)
#else
    #define DF_ASSERT(condition, message) \
        do {                              \
        } while (false)
#endif

#define FORCED_ASSERT(condition, message)                                                                                                           \
    do {                                                                                                                                            \
        if (!(condition)) {                                                                                                                         \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__ << " line " << __LINE__ << " function " << __PRETTY_FUNCTION__ << ": " \
                      << message << std::endl;                                                                                                      \
            abort();                                                                                                                                \
        }                                                                                                                                           \
    } while (false)

#endif // DATA_FRAME_COMMON_H
