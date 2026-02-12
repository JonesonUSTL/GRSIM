#pragma once
#include <string>

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

}  // namespace gptsolver::inp
