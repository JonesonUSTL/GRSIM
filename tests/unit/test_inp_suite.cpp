#include <cassert>
#include <fstream>

#include "gptsolver/io/inp/keyword_dispatcher.hpp"
#include "gptsolver/io/inp/parser.hpp"
#include "gptsolver/io/inp/semantic.hpp"

int main() {
  using namespace gptsolver::inp;
  assert(classify_keyword("STATIC") == KeywordTier::SupportedExecutable);
  assert(classify_keyword("GENERAL CONTACT") == KeywordTier::ParsedNotSolved);
  assert(classify_keyword("SOMETHING NEW") == KeywordTier::Unknown);
  assert(!all_supported_executable_keywords().empty());
  assert(!all_parsed_not_solved_keywords().empty());

  {
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

    auto ast = parse_file("tmp_keywords.inp");
    auto issues = semantic_check(ast);

    bool has_unknown = false;
    bool saw_dynamic = false;
    for (const auto& i : issues) {
      if (i.message.find("未识别关键字") != std::string::npos) has_unknown = true;
      if (i.keyword == "DYNAMIC") saw_dynamic = true;
    }

    assert(!has_unknown);
    assert(saw_dynamic);
  }

  {
    auto ast = parse_file("../../../examples/inp/official_like/official_cantilever_main.inp");
    if (ast.blocks.empty()) ast = parse_file("../../examples/inp/official_like/official_cantilever_main.inp");
    assert(!ast.blocks.empty());
    bool has_part = false, has_include = false, has_static = false;
    for (const auto& b : ast.blocks) {
      if (b.keyword == "PART") has_part = true;
      if (b.keyword == "INCLUDE") has_include = true;
      if (b.keyword == "STATIC") has_static = true;
    }
    assert(has_part && has_include && has_static);
  }

  {
    auto ast = parse_file("../../../examples/inp/official_like/official_contact_main.inp");
    if (ast.blocks.empty()) ast = parse_file("../../examples/inp/official_like/official_contact_main.inp");

    bool has_general_contact = false, has_contact_controls = false, has_surface_behavior = false;
    for (const auto& b : ast.blocks) {
      if (b.keyword == "GENERAL CONTACT") has_general_contact = true;
      if (b.keyword == "CONTACT CONTROLS") has_contact_controls = true;
      if (b.keyword == "SURFACE BEHAVIOR") has_surface_behavior = true;
    }

    assert(has_general_contact && has_contact_controls && has_surface_behavior);
  }

  {
    std::ofstream("tmp_element_supported.inp")
        << "*HEADING\n"
        << "*NODE\n"
        << "1,0,0,0\n2,1,0,0\n3,0,1,0\n4,0,0,1\n5,1,1,0\n6,1,0,1\n7,0,1,1\n8,1,1,1\n9,2,0,0\n10,0,2,0\n"
        << "*ELEMENT, TYPE=C3D10, ELSET=EALL\n"
        << "1,1,2,3,4,5,6,7,8,9,10\n";
    auto ast = parse_file("tmp_element_supported.inp");
    auto issues = semantic_check(ast);
    bool has_element_issue = false;
    for (const auto& i : issues) {
      if (i.keyword == "ELEMENT") has_element_issue = true;
    }
    assert(!has_element_issue);
  }

  {
    std::ofstream("tmp_element_unsupported.inp")
        << "*HEADING\n"
        << "*NODE\n"
        << "1,0,0,0\n2,1,0,0\n"
        << "*ELEMENT, TYPE=C3D27, ELSET=EALL\n"
        << "1,1,2\n";
    auto ast = parse_file("tmp_element_unsupported.inp");
    auto issues = semantic_check(ast);
    bool has_expected_issue = false;
    for (const auto& i : issues) {
      if (i.keyword == "ELEMENT" && i.message.find("C3D27") != std::string::npos) has_expected_issue = true;
    }
    assert(has_expected_issue);
  }

  return 0;
}
