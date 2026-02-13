#include <cassert>
#include <fstream>

#include "gptsolver/io/inp/parser.hpp"
#include "gptsolver/io/inp/semantic.hpp"

int main() {
  std::ofstream("tmp_keywords.inp")
      << "*HEADING\n"
      << "*AMPLITUDE, NAME=A1\n0.,0., 1.,1.\n"
      << "*ORIENTATION, NAME=ORI1\n1,0,0,0,1,0\n"
      << "*MATERIAL, NAME=STEEL\n*ELASTIC\n210000.,0.3\n*EXPANSION\n1.2E-5\n*SPECIFIC HEAT\n500.\n"
      << "*USER MATERIAL, CONSTANTS=2\n1.,2.\n*DEPVAR\n10\n*PLASTIC\n250.,0.0\n"
      << "*STEP\n*DYNAMIC\n0.01, 1.0\n*VISCO\n"
      << "*GENERAL CONTACT\n*CONTACT CONTROLS\n*SURFACE BEHAVIOR\n*CONTACT DAMPING\n"
      << "*COUPLING\n*KINEMATIC\n"
      << "*DLOAD\nEALL,P1,1.0\n*DSLOAD\nS1,P,1.0\n"
      << "*OUTPUT\n*NODE FILE\nU\n*EL FILE\nS\n"
      << "*END STEP\n";

  auto ast = gptsolver::inp::parse_file("tmp_keywords.inp");
  auto issues = gptsolver::inp::semantic_check(ast);

  bool has_unknown = false;
  bool saw_dynamic = false;
  for (const auto& i : issues) {
    if (i.message.find("未识别关键字") != std::string::npos) has_unknown = true;
    if (i.keyword == "DYNAMIC") saw_dynamic = true;
  }

  assert(!has_unknown);
  assert(saw_dynamic);  // parsed-not-solved tier should still be reported
  return 0;
}
