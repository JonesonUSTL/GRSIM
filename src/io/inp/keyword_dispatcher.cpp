#include "gptsolver/io/inp/keyword_dispatcher.hpp"

#include <set>

namespace gptsolver::inp {

namespace {
const std::set<std::string>& exec_set() {
  static const std::set<std::string> exec = {
      "HEADING",      "PREPRINT",     "PART",       "END PART",   "ASSEMBLY",      "END ASSEMBLY",
      "INSTANCE",     "NODE",         "ELEMENT",    "NSET",       "ELSET",         "SOLID SECTION",
      "MATERIAL",     "ELASTIC",      "DENSITY",    "CONDUCTIVITY","STEP",         "STATIC",
      "HEAT TRANSFER", "END STEP",    "BOUNDARY",   "CLOAD",      "TEMPERATURE",   "SURFACE",
      "CONTACT PAIR", "SURFACE INTERACTION", "FRICTION", "GENERAL CONTACT", "CONTACT CONTROLS",
      "MPC", "INCLUDE", "RESTART", "OUTPUT", "NODE OUTPUT",  "ELEMENT OUTPUT", "SHELL SECTION", "SECTION CONTROLS"};
  return exec;
}
const std::set<std::string>& parsed_set() {
  static const std::set<std::string> parsed = {
      "PLASTIC", "USER MATERIAL", "DEPVAR", "DLOAD", "DSLOAD", "COUPLING", "KINEMATIC", "AMPLITUDE",
      "CONTROLS", "SOLUTION TECHNIQUE", "SURFACE BEHAVIOR",
      "FILM", "RADIATION", "ORIENTATION", "EXPANSION", "SPECIFIC HEAT", "EL FILE", "NODE FILE", "DYNAMIC", "VISCO", "CONTACT DAMPING"};
  return parsed;
}
}  // namespace

KeywordTier classify_keyword(const std::string& keyword) {
  if (exec_set().count(keyword)) return KeywordTier::SupportedExecutable;
  if (parsed_set().count(keyword)) return KeywordTier::ParsedNotSolved;
  return KeywordTier::Unknown;
}

std::vector<std::string> all_supported_executable_keywords() { return {exec_set().begin(), exec_set().end()}; }
std::vector<std::string> all_parsed_not_solved_keywords() { return {parsed_set().begin(), parsed_set().end()}; }

}  // namespace gptsolver::inp
