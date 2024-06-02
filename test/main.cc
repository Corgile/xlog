#include <iostream>
#include "logging/api.hh"

int main() {
  xlog::Logger<>::getInstance();
  xlog::setLogLevelTo(xlog::Level::TRACE);
  xlog::toggleConsoleLogging(true);
  xlog::toggleAsyncLogging(false);
  xlog::InstantiateLogger(xlog::Level::TRACE);
  std::cout << "Hello, World!" << std::endl;
  XLOG_TRACE << "你好世界";
  XLOG_DEBUG << "你好世界";
  XLOG_INFO << "你好世界";
  XLOG_WARN << "你好世界";
  XLOG_ERROR << "你好世界";

  MXLOG_TRACE(1) << "你好世界";
  MXLOG_DEBUG(2) << "你好世界";
  MXLOG_INFO(3) << "你好世界";
  MXLOG_WARN(4) << "你好世界";
  MXLOG_ERROR(5) << "你好世界";

  MXLOG_TRACE("root") << "你好世界";
  MXLOG_DEBUG("service") << "你好世界";
  MXLOG_INFO("bussiness") << "你好世界";
  MXLOG_WARN("server") << "你好世界";
  MXLOG_ERROR("internal") << "你好世界";
  return 0;
}
