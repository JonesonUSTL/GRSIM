#include "gptsolver/io/inp/keyword_dispatcher.hpp"

#include <set>

namespace gptsolver::inp {

KeywordTier classify_keyword(const std::string& keyword) {
  static const std::set<std::string> exec = {
      "HEADING",       "NODE",       "ELEMENT",     "NSET",       "ELSET",     "STEP",
      "STATIC",        "HEAT TRANSFER", "END STEP",  "BOUNDARY",   "CLOAD",     "TEMPERATURE",
      "SURFACE",       "CONTACT PAIR",  "SURFACE INTERACTION", "FRICTION", "MPC", "INCLUDE",
      "OUTPUT",        "NODE OUTPUT", "ELEMENT OUTPUT"};

  static const std::set<std::string> parsed = {
      "PLASTIC", "USER MATERIAL", "DEPVAR", "DLOAD", "DSLOAD", "COUPLING", "KINEMATIC",
      "AMPLITUDE", "CONTROLS", "SOLUTION TECHNIQUE", "CONTACT CONTROLS", "GENERAL CONTACT", "SURFACE BEHAVIOR"};

  if (exec.count(keyword)) return KeywordTier::SupportedExecutable;
  if (parsed.count(keyword)) return KeywordTier::ParsedNotSolved;
  return KeywordTier::Unknown;
}

}  // namespace gptsolver::inp
