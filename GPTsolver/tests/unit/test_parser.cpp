#include <cassert>
#include <fstream>

#include "gptsolver/io/inp/parser.hpp"

int main() {
  std::ofstream("tmp_test.inp") << "*HEADING\n*NODE\n1,0,0,0\n";
  auto ast = gptsolver::inp::parse_file("tmp_test.inp");
  assert(ast.blocks.size() >= 2);
  return 0;
}
