#pragma once
#include <string>
#include <vector>

namespace gptsolver::inp {

/**
 * @brief 关键字能力分级。
 */
enum class KeywordTier {
  SupportedExecutable,
  ParsedNotSolved,
  Unknown,
};

/**
 * @brief 查询关键字分级。
 */
KeywordTier classify_keyword(const std::string& keyword);
std::vector<std::string> all_supported_executable_keywords();
std::vector<std::string> all_parsed_not_solved_keywords();

}  // namespace gptsolver::inp
