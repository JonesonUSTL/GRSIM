#include "gptsolver/solver/linear/petsc_backend.hpp"

#include <algorithm>

namespace gptsolver {
namespace {
std::string g_backend = "eigen";
}

void set_linear_solver_backend(const std::string& backend_name) {
  g_backend = backend_name;
  std::transform(g_backend.begin(), g_backend.end(), g_backend.begin(),
                 [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
}

std::string get_linear_solver_backend() { return g_backend; }

DenseVector solve_linear_petsc(const SparseMatrix& k, const DenseVector& f, bool& ok) {
  ok = false;
#ifdef GPTSOLVER_HAS_PETSC
  PetscInitialize(nullptr, nullptr, nullptr, nullptr);
  Mat A;
  Vec b, x;
  KSP ksp;

  MatCreate(PETSC_COMM_SELF, &A);
  MatSetSizes(A, PETSC_DECIDE, PETSC_DECIDE, k.rows(), k.cols());
  MatSetType(A, MATSEQAIJ);
  MatSeqAIJSetPreallocation(A, 32, nullptr);

  for (int col = 0; col < k.outerSize(); ++col) {
    for (SparseMatrix::InnerIterator it(k, col); it; ++it) {
      const PetscInt i = static_cast<PetscInt>(it.row());
      const PetscInt j = static_cast<PetscInt>(it.col());
      const PetscScalar v = static_cast<PetscScalar>(it.value());
      MatSetValue(A, i, j, v, ADD_VALUES);
    }
  }
  MatAssemblyBegin(A, MAT_FINAL_ASSEMBLY);
  MatAssemblyEnd(A, MAT_FINAL_ASSEMBLY);

  VecCreateSeq(PETSC_COMM_SELF, f.size(), &b);
  VecDuplicate(b, &x);
  for (int i = 0; i < f.size(); ++i) VecSetValue(b, i, static_cast<PetscScalar>(f(i)), INSERT_VALUES);
  VecAssemblyBegin(b);
  VecAssemblyEnd(b);

  KSPCreate(PETSC_COMM_SELF, &ksp);
  KSPSetOperators(ksp, A, A);
  KSPSetType(ksp, KSPCG);
  KSPSetFromOptions(ksp);
  KSPSolve(ksp, b, x);

  DenseVector out(f.size());
  const PetscScalar* arr = nullptr;
  VecGetArrayRead(x, &arr);
  for (int i = 0; i < f.size(); ++i) out(i) = static_cast<double>(arr[i]);
  VecRestoreArrayRead(x, &arr);

  KSPDestroy(&ksp);
  VecDestroy(&x);
  VecDestroy(&b);
  MatDestroy(&A);
  PetscFinalize();
  ok = true;
  return out;
#else
  (void)k;
  (void)f;
  return DenseVector::Zero(f.size());
#endif
}

}  // namespace gptsolver
