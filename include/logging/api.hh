//
// logging / logging.hh
// Created by brian on 2024-06-02.
//

#ifndef LOGGING_LOG_API_HH
  #define LOGGING_LOG_API_HH

  #include "logging/detail/logger.hh"

namespace xlog {
using namespace literals;

/// @brief  实例化日志记录器
/// @tparam ID  可以根据ID实例化不同logger
/// @param level    日志输出的最小等级
/// @param filename 日志文件名(无则创建)
/// @param async    是否开启异步日志
/// @param console  是否同时开启console日志
/// @param fileMaxSize  单个日志文件最大字节数 默认 1MB
/// @param maxFileCount 最大日志文件个数, 不能超过1000, >1 时开启滚动日志
/// @param alwaysFlush  写入一条刷新一次
template<size_t ID = 0>
inline void InstantiateLogger(Level level, std::string const& filename = "log",
                              bool async = true, bool console = true,
                              size_t fileMaxSize = 1_MB, size_t maxFileCount = 1,
                              bool alwaysFlush = false) {
  Logger<ID>::getInstance().init(level, async, console, filename,
                                 fileMaxSize, maxFileCount, alwaysFlush);
}

/// @brief 修改日志记录级别
/// @tparam ID  logger的ID
/// @param level 日志记录级别
template<size_t ID = 0>
inline void setLogLevelTo(Level level) {
  Logger<ID>::getInstance().setMinLevel(level);
}

/// @brief 获取当前日志记录级别
/// @tparam ID 实例ID
/// @return Level
template<size_t ID = 0>
inline Level currentLogLevel() {
  return Logger<ID>::getInstance().getMinLevel();
}

/// @brief 开启或关闭console日志输出
/// @tparam ID  logger id
/// @param enable   bool
template<size_t ID = 0>
inline void toggleConsoleLogging(bool enable) {
  Logger<ID>::getInstance().setConsole(enable);
}

/// @brief 当前logger是否允许控制台打印日志
/// @tparam ID  logger id
/// @return \p true if enabled else \p false
template<size_t ID = 0>
inline bool consoleLogEnabled() {
  return Logger<ID>::getInstance().consoleEnabled();
}

/// @brief 开启或关闭将当前logger的异步模式
/// @tparam ID  logger id
/// @param asynced \p true for async
template<size_t ID = 0>
inline void toggleAsyncLogging(bool asynced) {
  Logger<ID>::getInstance().setAsync(asynced);
}

/// @brief 当前logger是否是异步模式
/// @tparam ID logger id
/// @return \p true for asynced
template<size_t ID = 0>
inline bool loggerIsAsynced() {
  return Logger<ID>::getInstance().isAsynced();
}

/// @brief 刷新当前logger的输出
/// @tparam ID logger id
template<size_t ID = 0>
inline void flushLogs() {
  Logger<ID>::getInstance().flush();
}

/// @brief 停止当前logger的异步记录线程
/// @tparam ID logger id
template<size_t ID = 0>
inline void stopAsyncLog() {
  Logger<ID>::getInstance().stopAsyncLog();
}

/// @brief 添加用户自定义日志输出sink
/// @tparam ID  logger id
/// @param fn   函数对象
template<size_t ID = 0>
inline void addCustomLoggerSink(std::function<void(std::string_view)> fn) {
  Logger<ID>::getInstance().addSink(std::move(fn));
}
}
#endif //LOGGING_LOG_API_HH

#ifndef LOGGING_API_MACROS
  #define LOGGING_API_MACROS
  #define now_ std::chrono::system_clock::now()

  #define XLOG_IMPL(level, LoggerId, ...)                                    \
    if (!xlog::Logger<LoggerId>::getInstance().checkLevel(level)) {;}         \
    else                                                                     \
        xlog::Logger<LoggerId>::getInstance() +=                              \
          xlog::record_t(now_, level, GET_STRING(__FILE__, __LINE__)).ref()

  #ifndef XLOG
    #define XLOG(level, ...) XLOG_IMPL(xlog::Level::level, __VA_ARGS__, 0)
  #endif

  #define XLOGV_IMPL(level, LoggerId, fmt, ...)                            \
      if (!xlog::Logger<LoggerId>::getInstance().checkLevel(level)) {;}  \
      else do {                                                            \
        xlog::Logger<LoggerId>::getInstance() +=                            \
            xlog::record_t(now_, level,  GET_STRING(__FILE__, __LINE__))    \
                .sprintf(fmt, __VA_ARGS__);                                \
        if constexpr (level == xlog::Level::FATAL) {                        \
          xlog::flushLogs<LoggerId>();                                      \
          std::exit(EXIT_FAILURE);                                         \
        }                                                                  \
      } while (false)

  #ifndef XLOGV
    #define XLOGV(level, ...) XLOGV_IMPL(xlog::Level::level, 0, __VA_ARGS__, "\n")
  #endif

  #ifndef MXLOGV
    #define MXLOGV(level, LoggerId, ...) XLOGV_IMPL(xlog::Level::level, LoggerId, __VA_ARGS__, "\n")
  #endif

  #if __has_include(<fmt/format.h>) || __has_include(<format>)

    #define XLOGFMT_IMPL0(level, LoggerId, prefix, ...)                        \
      if (!xlog::Logger<LoggerId>::getInstance().checkLevel(level)) {;}         \
      else do {                                                                \
        xlog::Logger<LoggerId>::getInstance() +=                                \
            xlog::record_t(now_, level,  GET_STRING(__FILE__, __LINE__))        \
                .format(prefix::format(__VA_ARGS__));                          \
        if constexpr (level == xlog::Level::FATAL) {                            \
          xlog::flush<LoggerId>();                                              \
          std::exit(EXIT_FAILURE);                                             \
        }                                                                      \
      } while (false)

    #if __has_include(<fmt/format.h>)
      #define XLOGFMT_IMPL(level, LoggerId, ...) XLOGFMT_IMPL0(level, LoggerId, fmt, __VA_ARGS__)
    #else
      #define XLOGFMT_IMPL(level, LoggerId, ...) XLOGFMT_IMPL0(level, LoggerId, std, __VA_ARGS__)
    #endif

    #ifndef XLOGFMT
      #define XLOGFMT(level, ...)             XLOGFMT_IMPL(xlog::Level::level, 0, __VA_ARGS__)
    #endif
    #ifndef MXLOGFMT
      #define MXLOGFMT(level, LoggerId, ...)  XLOGFMT_IMPL(xlog::Level::level, LoggerId, __VA_ARGS__)
    #endif
  #endif

  #define XLOG_TRACE XLOG(TRACE)
  #define XLOG_DEBUG XLOG(DEBUG)
  #define XLOG_INFO  XLOG(INFO)
  #define XLOG_WARN  XLOG(WARN)
  #define XLOG_ERROR XLOG(ERROR)
  #define XLOG_FATAL XLOG(FATAL)

  #ifndef MXLOG_TRACE
    #define MXLOG_TRACE(id) XLOG(INFO, id)
  #endif
  #ifndef MXLOG_DEBUG
    #define MXLOG_DEBUG(id) XLOG(DEBUG, id)
  #endif
  #ifndef MXLOG_INFO
    #define MXLOG_INFO(id) XLOG(INFO, id)
  #endif
  #ifndef MXLOG_WARN
    #define MXLOG_WARN(id) XLOG(WARN, id)
  #endif
  #ifndef MXLOG_ERROR
    #define MXLOG_ERROR(id) XLOG(ERROR, id)
  #endif
  #ifndef MXLOG_FATAL
    #define MXLOG_FATAL(id) XLOG(FATAL, id)
  #endif

  #ifndef XLOGT
    #define XLOGT XLOG_TRACE
  #endif
  #ifndef XLOGD
    #define XLOGD XLOG_DEBUG
  #endif
  #ifndef XLOGI
    #define XLOGI XLOG_INFO
  #endif
  #ifndef XLOGW
    #define XLOGW XLOG_WARN
  #endif
  #ifndef XLOGE
    #define XLOGE XLOG_ERROR
  #endif
  #ifndef XLOGF
    #define XLOGF XLOG_FATAL
  #endif
#endif