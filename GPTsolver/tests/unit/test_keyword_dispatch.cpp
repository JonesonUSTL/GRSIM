#include <cassert>

#include "gptsolver/io/inp/keyword_dispatcher.hpp"

int main() {
  using namespace gptsolver::inp;
  assert(classify_keyword("STATIC") == KeywordTier::SupportedExecutable);
  assert(classify_keyword("GENERAL CONTACT") == KeywordTier::ParsedNotSolved);
  assert(classify_keyword("SOMETHING NEW") == KeywordTier::Unknown);
  return 0;
}
