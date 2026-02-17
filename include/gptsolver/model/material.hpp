#pragma once

#include <string>
#include <unordered_map>

#include "gptsolver/fem/constitutive/plastic/j2_return_mapping.hpp"

namespace gptsolver {

struct ElasticMaterial {
  double E{210e3};
  double nu{0.3};
};

struct MaterialRecord {
  std::string name;
  ElasticMaterial elastic;
  J2Material plastic_j2;
  bool enable_j2{false};
};

class MaterialLibrary {
 public:
  void upsert(const MaterialRecord& m) { mats_[m.name] = m; }
  const MaterialRecord* find(const std::string& name) const {
    auto it = mats_.find(name);
    if (it == mats_.end()) return nullptr;
    return &it->second;
  }
  std::size_t size() const { return mats_.size(); }

 private:
  std::unordered_map<std::string, MaterialRecord> mats_;
};

}  // namespace gptsolver
