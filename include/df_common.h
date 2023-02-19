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
  #include <algorithm>
  #include <functional>
  #include <iomanip>
  #include <iostream>
  #include <map>
  #include <ostream>
  #include <stdlib.h>
  #include <string>
  #include <vector>
#endif

#define DF_COLOR_R "\033[91m"
#define DF_COLOR_G "\033[92m"
#define DF_COLOR_C "\033[96m"
#define DF_COLOR_Y "\033[93m"
#define DF_COLOR_M "\033[35m"
#define DF_COLOR_W "\033[00m"

namespace df {

#ifdef QT_IMPLEMENTATION
  #define List QList
  #define clog qDebug()
  using sizetype   = std::size_t;
  using String     = QString;
  using StringList = QStringList;
  using IndexHash  = QMap<QString, sizetype>;
#else
  #define List std::vector
  #define clog std::cout
  using sizetype   = std::size_t;
  using String     = std::string;
  using StringList = std::vector<String>;
  using IndexHash  = std::map<String, sizetype>;
  using ostream    = std::ostream;
#endif

} // namespace df

#ifndef NDEBUG
  #ifdef QT_IMPLEMENTATION
    #define DF_ASSERT(condition, message)                        \
      do {                                                       \
        if (!(condition)) {                                      \
          qFatal(QString("%1%2%3%4%5%6%7%8")                     \
                   .arg("Assertion `" #condition "` failed in ") \
                   .arg(__FILE__)                                \
                   .arg(" line ")                                \
                   .arg(__LINE__)                                \
                   .arg(" function ")                            \
                   .arg(__PRETTY_FUNCTION__)                     \
                   .arg(": ")                                    \
                   .arg(message)                                 \
                   .toStdString()                                \
                   .c_str());                                    \
        }                                                        \
      } while (false)
  #else
    #define DF_ASSERT(condition, message)                                                                          \
      do {                                                                                                         \
        if (!(condition)) {                                                                                        \
          std::cerr << "Assertion `" #condition "` failed in " << __FILE__ << " line " << __LINE__ << " function " \
                    << __PRETTY_FUNCTION__ << ": " << message << std::endl;                                        \
          abort();                                                                                                 \
        }                                                                                                          \
      } while (false)
  #endif
#else
  #define DF_ASSERT(condition, message) \
    do {                                \
    } while (false)
#endif

#ifdef QT_IMPLEMENTATION
  #define FORCED_ASSERT(condition, message)                    \
    do {                                                       \
      if (!(condition)) {                                      \
        qFatal(QString("%1%2%3%4%5%6%7%8")                     \
                 .arg("Assertion `" #condition "` failed in ") \
                 .arg(__FILE__)                                \
                 .arg(" line ")                                \
                 .arg(__LINE__)                                \
                 .arg(" function ")                            \
                 .arg(__PRETTY_FUNCTION__)                     \
                 .arg(": ")                                    \
                 .arg(message)                                 \
                 .toStdString()                                \
                 .c_str());                                    \
      }                                                        \
    } while (false)
#else
  #define FORCED_ASSERT(condition, message)                                                                      \
    do {                                                                                                         \
      if (!(condition)) {                                                                                        \
        std::cerr << "Assertion `" #condition "` failed in " << __FILE__ << " line " << __LINE__ << " function " \
                  << __PRETTY_FUNCTION__ << ": " << message << std::endl;                                        \
        abort();                                                                                                 \
      }                                                                                                          \
    } while (false)
#endif

#endif // DATA_FRAME_COMMON_H
