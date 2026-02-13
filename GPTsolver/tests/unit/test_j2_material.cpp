#include <array>
#include <cassert>

#include "gptsolver/fem/constitutive/plastic/j2_return_mapping.hpp"
#include "gptsolver/model/material.hpp"

int main() {
  gptsolver::J2Material mat;
  mat.E = 210000.0;
  mat.nu = 0.3;
  mat.sigma_y0 = 250.0;
  mat.H = 1000.0;

  gptsolver::J2State state;
  const std::array<double, 6> trial = {400.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  auto r = gptsolver::j2_radial_return(trial, mat, state);
  assert(r.yielded);
  assert(state.eqp > 0.0);
  assert(r.sigma_eq <= r.sigma_y + 1e-6);

  gptsolver::MaterialLibrary lib;
  gptsolver::MaterialRecord steel;
  steel.name = "STEEL";
  steel.enable_j2 = true;
  steel.plastic_j2 = mat;
  lib.upsert(steel);
  auto found = lib.find("STEEL");
  assert(found != nullptr);
  assert(found->enable_j2);
  return 0;
}
