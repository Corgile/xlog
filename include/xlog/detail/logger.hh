//
// xlog / logger.hh
// Created by brian on 2024-06-03.
//

#ifndef XLOG_LOGGER_HH
#define XLOG_LOGGER_HH

#if __has_include(<format>)
  #include <format>
#endif

#if __has_include(<fmt/format.h>)
  #ifndef FMT_HEADER_ONLY
    #define FMT_HEADER_ONLY

    #include <fmt/format.h>

  #endif
#endif

#include "xlog/detail/my_literals.hh"
#include "xlog/detail/record.hh"
#include "xlog/detail/sink.hh"

#include <functional>
#include <string_view>
#include <utility>
#include <vector>

namespace xlog {

template<size_t ID = 0>
class Logger {
public:
  static Logger<ID>& getInstance() {
    static Logger<ID> logger;
    return logger;
  }

  ///@brief 进行日志记录
  void log(record_t& record) {
    if (async_ and pSink_) {
      appendRecord(std::move(record));
    } else {
      appendFormat(record);
    }

    if (record.getLevel() == Level::FATAL) {
      flush();
      std::exit(EXIT_FAILURE);
    }
  }

  void operator+=(record_t& record) { log(record); }

  void flush() {
    if (pSink_) pSink_->flush();
  }

  void init(Level minLevel, bool async, bool console,
            std::string const& filename, size_t fileMaxSize,
            size_t maxFileCount, bool alwaysFlush) {
    static Sink sink(filename, async, console, fileMaxSize, maxFileCount, alwaysFlush);
    async_ = async;
    pSink_ = &sink;
    minLevel_      = minLevel;
    enableConsole_ = console;
  }

  /// 只有当 level ≥ 最低level才进行日志记录
  bool checkLevel(Level level) { return level >= minLevel_; }

  /// 添加日志下游流向
  void addSink(std::function<void(std::string_view)> fn) {
    sinks_.emplace_back(std::move(fn));
  }

  /// 停止异步日志线程
  void stopAsyncLog() { pSink_->stop(); }

  void setMinLevel(Level level) { minLevel_ = level; }

  Level getMinLevel() const { return minLevel_; }

  void setConsole(bool enabled) {
    enableConsole_ = enabled;
    if (pSink_) pSink_->enableConsole(enabled);
  }

  bool consoleEnabled() const { return enableConsole_; }

  void setAsync(bool asynced) { async_ = asynced; }

  bool isAsynced() const { return async_; }

private:
  Logger() {
    static Sink sink{};
    /// TODO: 这里可以进行多线程支持
    sink.startThread();
    sink.enableConsole(true);
    async_ = true;
    pSink_ = &sink;
  }

  Logger(const Logger&) = default;

  void appendRecord(record_t record) { pSink_->write(std::move(record)); }

  void appendFormat(record_t& record) {
    if (not pSink_) return;
    if (enableConsole_) {
      pSink_->writeRecord<true, true>(record);
    } else {
      pSink_->writeRecord<true, false>(record);
    }
  }

  Level minLevel_ =
#if NDEBUG
      Level::WARN;
#else
      Level::TRACE;
#endif
  bool      async_         = false;
  bool      enableConsole_ = true;
  Sink::ptr pSink_         = nullptr;
  /// 其他下游日志消息消费者
  std::vector<std::function<void(std::string_view)>> sinks_;
};

} // xhl

#endif // XLOG_LOGGER_HH
