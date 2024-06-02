//
// xlog / api_macro.hh
// Created by brian on 2024-06-04.
//

#ifndef XLOG_API_MACRO_HH
#define XLOG_API_MACRO_HH

#define now_          std::chrono::system_clock::now()
#define instance_(id) xlog::Logger<id>::getInstance()

#define XLOG_IMPL(level, LoggerId, ...)                                                                                \
  if (!instance_(LoggerId).checkLevel(level)) {                                                                        \
  } else instance_(LoggerId) += xlog::record_t(now_, level, GET_STRING(__FILE__, __LINE__)).ref()

#ifndef XLOG
#  define XLOG(level, ...) XLOG_IMPL(xlog::Level::level, __VA_ARGS__, 0)
#endif

#define XLOGV_IMPL(level, LoggerId, fmt, ...)                                                                          \
  if (!instance_(LoggerId).checkLevel(level)) {                                                                        \
  } else do {                                                                                                          \
      instance_(LoggerId) += xlog::record_t(now_, level, GET_STRING(__FILE__, __LINE__)).sprintf(fmt, __VA_ARGS__);    \
      if constexpr (level == xlog::Level::FATAL) {                                                                     \
        xlog::flushLogs<LoggerId>();                                                                                   \
        std::exit(EXIT_FAILURE);                                                                                       \
      }                                                                                                                \
    } while (false)

#ifndef XLOGV
#  define XLOGV(level, ...) XLOGV_IMPL(xlog::Level::level, 0, __VA_ARGS__, "\n")
#endif

#ifndef MXLOGV
#  define MXLOGV(level, LoggerId, ...) XLOGV_IMPL(xlog::Level::level, LoggerId, __VA_ARGS__, "\n")
#endif

#if __has_include(<fmt/format.h>) or __has_include(<format>)
#  define XLOGFMT_IMPL0(level, LoggerId, prefix, ...)                                                                  \
    if (!instance_(LoggerId).checkLevel(level)) {                                                                      \
    } else do {                                                                                                        \
        instance_(LoggerId) +=                                                                                         \
            xlog::record_t(now_, level, GET_STRING(__FILE__, __LINE__)).format(prefix::format(__VA_ARGS__));           \
        if constexpr (level == xlog::Level::FATAL) {                                                                   \
          xlog::flushLogs<LoggerId>();                                                                                 \
          std::exit(EXIT_FAILURE);                                                                                     \
        }                                                                                                              \
      } while (false)

#  if __has_include(<fmt/format.h>)
#    define XLOGFMT_IMPL(level, LoggerId, ...) XLOGFMT_IMPL0(level, LoggerId, fmt, __VA_ARGS__)
#  else
#    define XLOGFMT_IMPL(level, LoggerId, ...) XLOGFMT_IMPL0(level, LoggerId, std, __VA_ARGS__)
#  endif

#  ifndef XLOGFMT
#    define XLOGFMT(level, ...) XLOGFMT_IMPL(xlog::Level::level, 0, __VA_ARGS__)
#  endif
#  ifndef MXLOGFMT
#    define MXLOGFMT(level, LoggerId, ...) XLOGFMT_IMPL(xlog::Level::level, LoggerId, __VA_ARGS__)
#  endif
#endif

#define XLOG_TRACE XLOG(TRACE)
#define XLOG_DEBUG XLOG(DEBUG)
#define XLOG_INFO  XLOG(INFO)
#define XLOG_WARN  XLOG(WARN)
#define XLOG_ERROR XLOG(ERROR)
#define XLOG_FATAL XLOG(FATAL)

#ifndef MXLOG_TRACE
#  define MXLOG_TRACE(id) XLOG(INFO, id)
#endif
#ifndef MXLOG_DEBUG
#  define MXLOG_DEBUG(id) XLOG(DEBUG, id)
#endif
#ifndef MXLOG_INFO
#  define MXLOG_INFO(id) XLOG(INFO, id)
#endif
#ifndef MXLOG_WARN
#  define MXLOG_WARN(id) XLOG(WARN, id)
#endif
#ifndef MXLOG_ERROR
#  define MXLOG_ERROR(id) XLOG(ERROR, id)
#endif
#ifndef MXLOG_FATAL
#  define MXLOG_FATAL(id) XLOG(FATAL, id)
#endif
#define TOGGLE_ON  true
#define TOGGLE_OFF false
#endif // XLOG_API_MACRO_HH
