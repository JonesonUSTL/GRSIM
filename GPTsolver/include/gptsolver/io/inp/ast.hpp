#pragma once
#include <map>
#include <string>
#include <vector>

namespace gptsolver::inp {

struct SourcePos {
  std::string file;
  int line{0};
  int col{0};
};

struct KeywordBlock {
  std::string keyword;
  std::map<std::string, std::string> params;
  std::vector<std::string> data_lines;
  SourcePos pos;
};

struct ModelAst {
  std::vector<KeywordBlock> blocks;
};

}  // namespace gptsolver::inp
