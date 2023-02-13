#ifndef DATA_FRAME_COMMON_H
#define DATA_FRAME_COMMON_H

#include <concepts>
#include <optional>

#ifdef QT_IMPLEMENTATION
  #include <QDebug>
  #include <QList>
  #include <QMap>
  #include <QString>
  #include <QStringList>
#else
  #include <iostream>
  #include <map>
  #include <ostream>
  #include <stdlib.h>
  #include <string>
  #include <vector>
#endif

namespace df {
  using i8   = char;
  using ui8  = unsigned char;
  using i16  = short;
  using ui16 = unsigned short;
  using i32  = long;
  using ui32 = unsigned long;
  using i64  = long long;
  using ui64 = unsigned long long;
  using f32  = float;
  using f64  = double;
  using f128 = long double;

  using size_t = ui32;

#ifdef QT_IMPLEMENTATION
  #define List QList
  #define clog qDebug().noquote()
  using String     = QString;
  using StringList = QStringList;
  using IndexHash  = QMap<QString, ui32>;
#else
  #define List std::vector
  #define clog std::cout
  using String     = std::string;
  using StringList = std::vector<String>;
  using IndexHash  = std::map<String, ui32>;
  using ostream    = std::ostream;
#endif

} // namespace df

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

template<typename T>
concept NumericalTypes
  = std::is_same<T, df::i8>::value || std::is_same<T, df::ui8>::value || std::is_same<T, df::i16>::value
    || std::is_same<T, df::ui16>::value || std::is_same<T, df::i32>::value || std::is_same<T, df::ui32>::value
    || std::is_same<T, df::i64>::value || std::is_same<T, df::ui64>::value || std::is_same<T, df::f32>::value
    || std::is_same<T, df::f64>::value || std::is_same<T, df::f128>::value;

#endif // DATA_FRAME_COMMON_H
