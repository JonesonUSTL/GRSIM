#pragma once
#include <fstream>
#include <mutex>
#include <string>

namespace gptsolver {

class Logger {
 public:
  void open(const std::string& path);
  void info(const std::string& msg);
  void warn(const std::string& msg);
  void error(const std::string& msg);

 private:
  void write(const std::string& level, const std::string& msg);
  std::ofstream ofs_;
  std::mutex mtx_;
};

Logger& global_logger();

}  // namespace gptsolver
