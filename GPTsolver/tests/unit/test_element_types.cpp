#include <cassert>
#include <fstream>

#include "gptsolver/io/inp/parser.hpp"
#include "gptsolver/io/inp/semantic.hpp"

int main() {
  {
    std::ofstream("tmp_element_supported.inp")
        << "*HEADING\n"
        << "*NODE\n"
        << "1,0,0,0\n2,1,0,0\n3,0,1,0\n4,0,0,1\n5,1,1,0\n6,1,0,1\n7,0,1,1\n8,1,1,1\n9,2,0,0\n10,0,2,0\n"
        << "*ELEMENT, TYPE=C3D10, ELSET=EALL\n"
        << "1,1,2,3,4,5,6,7,8,9,10\n";
    auto ast = gptsolver::inp::parse_file("tmp_element_supported.inp");
    auto issues = gptsolver::inp::semantic_check(ast);
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
    auto ast = gptsolver::inp::parse_file("tmp_element_unsupported.inp");
    auto issues = gptsolver::inp::semantic_check(ast);
    bool has_expected_issue = false;
    for (const auto& i : issues) {
      if (i.keyword == "ELEMENT" && i.message.find("C3D27") != std::string::npos) has_expected_issue = true;
    }
    assert(has_expected_issue);
  }

  return 0;
}
