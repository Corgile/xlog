#include "xlog/api.hh"

int main() {
  // 不用getInstance()也可以在后续被调用的时候实例化
  xlog::Logger<>::getInstance();
  xlog::setLogLevelTo(xlog::Level::TRACE);
  xlog::toggleConsoleLogging(TOGGLE_ON);
  xlog::toggleAsyncLogging(TOGGLE_OFF);
  constexpr std::string_view systemLogger = "System";
  XLOG_TRACE << "你好世界";
  XLOG_DEBUG << "你好世界";
  XLOG_INFO << "你好世界";
  XLOG_WARN << "你好世界";
  XLOG_ERROR << "你好世界";
  xlog::toggleConsoleLogging<hashed(systemLogger)>(TOGGLE_ON);
  MXLOG_TRACE(systemLogger) << "你好世界";
  MXLOG_DEBUG(systemLogger) << "你好世界";
  MXLOG_INFO(systemLogger) << "你好世界";
  MXLOG_WARN(systemLogger) << "你好世界";
  MXLOG_ERROR(systemLogger) << "你好世界" << 12.3;
  XLOGV(INFO, "%s", "你好世界");
  MXLOGV(WARN, systemLogger, "%s", "你好世界");
#if __has_include(<fmt/format.h> ) or __has_include(<format>)
  XLOGFMT(INFO, "{}", "你好世界");
  MXLOGFMT(WARN, systemLogger, "{1}, {0}", "你好世界", "再见世界");
#endif
  // std::this_thread::sleep_for(std::chrono::seconds(1));
  return 0;
}
