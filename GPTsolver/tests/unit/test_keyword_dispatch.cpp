#include <cassert>

#include "gptsolver/io/inp/keyword_dispatcher.hpp"

int main() {
  using namespace gptsolver::inp;
  assert(classify_keyword("STATIC") == KeywordTier::SupportedExecutable);
  assert(classify_keyword("GENERAL CONTACT") == KeywordTier::ParsedNotSolved);
  assert(classify_keyword("SOMETHING NEW") == KeywordTier::Unknown);
  assert(!all_supported_executable_keywords().empty());
  assert(!all_parsed_not_solved_keywords().empty());
  return 0;
}
