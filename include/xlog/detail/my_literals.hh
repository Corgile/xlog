//
// xlog / my_literals.hh
// Created by brian on 2024-06-03.
//

#ifndef XLOG_MY_LITERALS_HH
#define XLOG_MY_LITERALS_HH
namespace xlog::literals {
constexpr unsigned long long operator ""_B(unsigned long long bytes) {
  return bytes;
}

constexpr unsigned long long operator ""_KB(unsigned long long kilobytes) {
  return kilobytes << 10;
}

constexpr unsigned long long operator ""_MB(unsigned long long megabytes) {
  return megabytes << 20;
}

constexpr unsigned long long operator ""_GB(unsigned long long gigabytes) {
  return gigabytes << 30;
}
}
#endif // XLOG_MY_LITERALS_HH
