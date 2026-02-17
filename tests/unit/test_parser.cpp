#include <cassert>
#include <fstream>

#include "gptsolver/io/inp/parser.hpp"
#include "gptsolver/io/inp/semantic.hpp"

int main() {
  std::ofstream("tmp_test.inp") << "*HEADING\n*STEP, NLGEOM=YES,\nINC=200\n*STATIC\n*MPC\n1,2\n*END STEP\n";
  auto ast = gptsolver::inp::parse_file("tmp_test.inp");
  auto issues = gptsolver::inp::semantic_check(ast);
  assert(ast.blocks.size() >= 4);
  assert(issues.empty());
  return 0;
}
