//
// logging / level.hh
// Created by brian on 2024-06-03.
//

#ifndef LOGGING_LEVEL_HH
#define LOGGING_LEVEL_HH

namespace xlog {

enum class Level {
  NONE = 0,
  TRACE,
  DEBUG,
  INFO,
  WARN,
  ERROR,
  FATAL,
};

} // xhl

#endif //LOGGING_LEVEL_HH
