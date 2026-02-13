#include <cassert>

#include "gptsolver/io/inp/parser.hpp"

int main() {
  auto ast = gptsolver::inp::parse_file("../../../examples/inp/official_like/official_contact_main.inp");
  if (ast.blocks.empty()) ast = gptsolver::inp::parse_file("../../examples/inp/official_like/official_contact_main.inp");

  bool has_general_contact = false, has_contact_controls = false, has_surface_behavior = false;
  for (const auto& b : ast.blocks) {
    if (b.keyword == "GENERAL CONTACT") has_general_contact = true;
    if (b.keyword == "CONTACT CONTROLS") has_contact_controls = true;
    if (b.keyword == "SURFACE BEHAVIOR") has_surface_behavior = true;
  }

  assert(has_general_contact && has_contact_controls && has_surface_behavior);
  return 0;
}
