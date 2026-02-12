#include "gptsolver/io/inp/keyword_dispatcher.hpp"

#include <set>

namespace gptsolver::inp {

namespace {
const std::set<std::string>& exec_set() {
  static const std::set<std::string> exec = {
      "HEADING", "NODE", "ELEMENT", "NSET", "ELSET", "STEP", "STATIC", "HEAT TRANSFER", "END STEP",
      "BOUNDARY", "CLOAD", "TEMPERATURE", "SURFACE", "CONTACT PAIR", "SURFACE INTERACTION", "FRICTION",
      "MPC", "INCLUDE", "OUTPUT", "NODE OUTPUT", "ELEMENT OUTPUT"};
  return exec;
}
const std::set<std::string>& parsed_set() {
  static const std::set<std::string> parsed = {
      "PLASTIC", "USER MATERIAL", "DEPVAR", "DLOAD", "DSLOAD", "COUPLING", "KINEMATIC", "AMPLITUDE",
      "CONTROLS", "SOLUTION TECHNIQUE", "CONTACT CONTROLS", "GENERAL CONTACT", "SURFACE BEHAVIOR"};
  return parsed;
}
}  // namespace

KeywordTier classify_keyword(const std::string& keyword) {
  if (exec_set().count(keyword)) return KeywordTier::SupportedExecutable;
  if (parsed_set().count(keyword)) return KeywordTier::ParsedNotSolved;
  return KeywordTier::Unknown;
}

std::vector<std::string> all_supported_executable_keywords() {
  return {exec_set().begin(), exec_set().end()};
}

std::vector<std::string> all_parsed_not_solved_keywords() {
  return {parsed_set().begin(), parsed_set().end()};
}

}  // namespace gptsolver::inp
