#ifndef DATA_FRAME_COMMON_H
#define DATA_FRAME_COMMON_H

#include <concepts>
#include <optional>

#ifdef QT_IMPLEMENTATION
  #include <QHash>
  #include <QList>
  #include <QString>
  #include <QStringList>
#else
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
  using String     = QString;
  using StringList = QStringList;
  using i8List     = QList<i8>;
  using ui8List    = QList<ui8>;
  using i16List    = QList<i16>;
  using ui16List   = QList<ui16>;
  using i32List    = QList<i32>;
  using ui32List   = QList<ui32>;
  using i64List    = QList<i64>;
  using ui64List   = QList<ui64>;
  using f32List    = QList<f32>;
  using f64List    = QList<f64>;
  using f128List   = QList<f128>;
  using IndexHash  = QHash<QString, i32>;

#else
  using String     = std::string;
  using StringList = std::vector<String>;
  using i8List     = std::vector<i8>;
  using ui8List    = std::vector<ui8>;
  using i16List    = std::vector<i16>;
  using ui16List   = std::vector<ui16>;
  using i32List    = std::vector<i32>;
  using ui32List   = std::vector<ui32>;
  using i64List    = std::vector<i64>;
  using ui64List   = std::vector<ui64>;
  using f32List    = std::vector<f32>;
  using f64List    = std::vector<f64>;
  using f128List   = std::vector<f128>;
  using IndexHash  = std::map<String, i32>;
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
