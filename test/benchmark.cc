//
// xlog / Test.cc
// Created by brian on 2024-06-03.
//

#include <exception>
#include <system_error>
#ifdef HAVE_GLOG
  #include <glog/xlog.h>
#endif

#ifdef HAVE_SPDLOG
#include <spdlog/sinks/base_sink.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/spdlog.h>
#endif

#include <filesystem>
#include <xlog/api.hh>


class ScopedTimer {
public:
  ScopedTimer(const char *name)
    : m_name(name), m_beg(std::chrono::high_resolution_clock::now()) {}
  ScopedTimer(const char *name, uint64_t &ns) : ScopedTimer(name) {
    m_ns = &ns;
  }
  ~ScopedTimer() {
    auto end = std::chrono::high_resolution_clock::now();
    auto dur =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_beg);
    if (m_ns)
      *m_ns = dur.count();

    std::cout << m_name << " : " << dur.count() << " ns\n";
  }

private:
  const char *m_name;
  std::chrono::time_point<std::chrono::high_resolution_clock> m_beg;
  uint64_t *m_ns = nullptr;
};

void test_glog() {
#ifdef HAVE_GLOG
  std::filesystem::remove("glog.txt");
  FLAGS_log_dir = ".";
  FLAGS_minloglevel = google::GLOG_INFO;
  FLAGS_timestamp_in_logfile_name = false;
  google::SetLogDestination(google::INFO, "glog.txt");
  google::InitGoogleLogging("glog");

  {
    ScopedTimer timer("glog   ");
    for (int i = 0; i < 5000; i++)
      LOG(INFO) << "Hello, it is a long string test! " << 42 << 21 << 2.5;
  }
#endif
}

void test_easylog(std::string filename, int count, bool async) {
  std::error_code ec;
  std::filesystem::remove(filename, ec);
  if (ec) {
    std::cout << ec.message() << "\n";
  }
  xlog::InstantiateFileLogger(xlog::Level::DEBUG, filename, async, false, -1);
  for (int i = 0; i < 10; i++) {
    ScopedTimer timer("xlog");
    for (int i = 0; i < count; i++)
      XLOG_INFO << "Hello logger: msg number " << i;
  }
}

#ifdef HAVE_SPDLOG
void bench(int howmany, std::shared_ptr<spdlog::logger> log) {
  spdlog::drop(log->name());

  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;

  for (int i = 0; i < 10; i++) {
    ScopedTimer timer("spdlog ");
    for (auto i = 0; i < howmany; ++i) {
      SPDLOG_LOGGER_INFO(log, "Hello logger: msg number {}", i);
    }
  }
}

void bench_mt(int howmany, std::shared_ptr<spdlog::logger> log,
              size_t thread_count) {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  spdlog::drop(log->name());

  std::vector<std::thread> threads;
  threads.reserve(thread_count);
  {
    ScopedTimer timer("spdlog ");
    for (size_t t = 0; t < thread_count; ++t) {
      threads.emplace_back([&]() {
        for (int j = 0; j < howmany / static_cast<int>(thread_count); j++) {
          SPDLOG_LOGGER_INFO(log, "Hello logger: msg number {}", j);
        }
      });
    }

    for (auto &t : threads) {
      t.join();
    };
  }
}
#endif

int main() {
  int count = 500000;
#ifdef HAVE_SPDLOG
  spdlog::set_pattern("%Y-%m-%d %H:%M:%S.%e %l [%t] [%@] %v");

  std::cout << "========test sync spdlog===========\n";
  auto basic_st = spdlog::basic_logger_st("basic_st", "basic_st.log", true);
  bench(count, std::move(basic_st));

  std::cout << "========test async spdlog===========\n";
  auto basic_mt = spdlog::basic_logger_st("basic_mt", "basic_mt.log", true);
  bench_mt(count, std::move(basic_mt), 1);
#endif

  test_glog();
  std::cout << "========test sync xlog===========\n";
  test_easylog("xlog.txt", count, /*async =*/false);
  std::cout << "========test async xlog===========\n";
  test_easylog("async_xlog.txt", count, /*async =*/true);
}
