//
// xlog / file.cc
// Created by brian on 2024-06-04.
//
#include "xlog/api.hh"

#include <iostream>

int main() {
  // 默认只有ID==0的logger写入文件xlog.txt.
  xlog::InstantiateFileLogger(xlog::Level::TRACE, "main.log");
  xlog::toggleConsoleLogging(TOGGLE_ON);
  xlog::toggleAsyncLogging(TOGGLE_OFF);
  XLOG_TRACE << "你好世界"; // Main
  XLOG_DEBUG << "你好世界"; // Main
  XLOG_INFO << "你好世界";  // Main
  XLOG_WARN << "你好世界";  // Main
  XLOG_ERROR << "你好世界"; // Main
  xlog::setLogLevelTo(xlog::Level::INFO);
  std::cout << "Hello, World!" << std::endl;

  xlog::InstantiateFileLogger<"System"_hash>(xlog::Level::TRACE, "system.log");
  xlog::toggleAsyncLogging<"System"_hash>(TOGGLE_OFF);
  MXLOG_TRACE("System") << "你好世界";
  MXLOG_DEBUG("System") << "你好世界";
  MXLOG_INFO("System") << "你好世界";
  MXLOG_WARN("System") << "你好世界";
  MXLOG_ERROR("System") << "你好世界";
  XLOGV(INFO, "%s", "你好世界");
  MXLOGV(WARN, "System", "%s", "你好世界");
#if __has_include(<fmt/format.h> ) or __has_include(<format>)
  XLOGFMT(INFO, "{}", "你好世界");
  MXLOGFMT(WARN, "System", "{1}, {0}", "你好世界", "再见世界");
#endif
  // std::this_thread::sleep_for(std::chrono::seconds(1));
  return 0;
}
