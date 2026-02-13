#include "gptsolver/core/logger.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>

namespace gptsolver {

namespace {
std::string now_string() {
  auto now = std::chrono::system_clock::now();
  auto tt = std::chrono::system_clock::to_time_t(now);
  std::tm tm{};
  localtime_r(&tt, &tm);
  std::ostringstream oss;
  oss << std::put_time(&tm, "%F %T");
  return oss.str();
}
}  // namespace

void Logger::open(const std::string& path) { ofs_.open(path); }
void Logger::info(const std::string& msg) { write("INFO", msg); }
void Logger::warn(const std::string& msg) { write("WARN", msg); }
void Logger::error(const std::string& msg) { write("ERROR", msg); }

void Logger::write(const std::string& level, const std::string& msg) {
  std::lock_guard<std::mutex> lock(mtx_);
  const auto line = "[" + now_string() + "][" + level + "] " + msg;
  std::cout << line << '\n';
  if (ofs_.is_open()) ofs_ << line << '\n';
}

Logger& global_logger() {
  static Logger logger;
  return logger;
}

}  // namespace gptsolver
