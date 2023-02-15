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
  #include <iomanip>
  #include <iostream>
  #include <map>
  #include <ostream>
  #include <stdlib.h>
  #include <string>
  #include <vector>
#endif

namespace df {

#ifdef QT_IMPLEMENTATION
  #define List QList
  #define clog qDebug()
  using sizetype   = unsigned long;
  using String     = QString;
  using StringList = QStringList;
  using IndexHash  = QMap<QString, long>;
#else
  #define List std::vector
  #define clog std::cout
  using sizetype   = std::size_t;
  using String     = std::string;
  using StringList = std::vector<String>;
  using IndexHash  = std::map<String, long>;
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

#endif // DATA_FRAME_COMMON_H
