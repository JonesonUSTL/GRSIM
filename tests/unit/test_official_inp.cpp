#include <cassert>

#include "gptsolver/io/inp/parser.hpp"

int main() {
  auto ast = gptsolver::inp::parse_file("../../../examples/inp/official_like/official_cantilever_main.inp");
  if (ast.blocks.empty()) ast = gptsolver::inp::parse_file("../../examples/inp/official_like/official_cantilever_main.inp");
  assert(!ast.blocks.empty());
  bool has_part = false, has_include = false, has_static = false;
  for (const auto& b : ast.blocks) {
    if (b.keyword == "PART") has_part = true;
    if (b.keyword == "INCLUDE") has_include = true;
    if (b.keyword == "STATIC") has_static = true;
  }
  assert(has_part && has_include && has_static);
  return 0;
}
