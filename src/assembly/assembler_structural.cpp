#include "gptsolver/assembly/assembler_structural.hpp"

#ifdef GPTSOLVER_USE_MPI
#include <mpi.h>
#endif

namespace gptsolver {
namespace {
inline void local_range(int n, int rank, int size, int& b, int& e) {
  const int base = n / size;
  const int rem = n % size;
  b = rank * base + (rank < rem ? rank : rem);
  e = b + base + (rank < rem ? 1 : 0);
}
}

SparseMatrix build_structural_stiffness_sparse(int n) {
  SparseMatrix k(n, n);
  std::vector<Triplet> tri;
  tri.reserve(static_cast<size_t>(n));
#ifdef GPTSOLVER_USE_OPENMP
#pragma omp parallel
  {
    std::vector<Triplet> local;
#pragma omp for nowait
    for (int i = 0; i < n; ++i) local.emplace_back(i, i, 10.0 + (i % 5));
#pragma omp critical
    tri.insert(tri.end(), local.begin(), local.end());
  }
#else
  for (int i = 0; i < n; ++i) tri.emplace_back(i, i, 10.0 + (i % 5));
#endif
  k.setFromTriplets(tri.begin(), tri.end());
  return k;
}

DenseVector build_structural_load(int n) {
  DenseVector f = DenseVector::Zero(n);
  if (n > 0) f(n - 1) = 1.0;
  return f;
}

SparseMatrix build_structural_stiffness_distributed(int n) {
#ifdef GPTSOLVER_USE_MPI
  int init = 0;
  MPI_Initialized(&init);
  if (!init) {
    int argc = 0;
    char** argv = nullptr;
    MPI_Init(&argc, &argv);
  }
  int rank = 0, size = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  Eigen::VectorXd dlocal = Eigen::VectorXd::Zero(n);
  int b = 0, e = n;
  local_range(n, rank, size, b, e);
  for (int i = b; i < e; ++i) dlocal(i) = 10.0 + (i % 5);

  Eigen::VectorXd dglobal = Eigen::VectorXd::Zero(n);
  MPI_Allreduce(dlocal.data(), dglobal.data(), n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

  SparseMatrix k(n, n);
  std::vector<Triplet> tri;
  tri.reserve(static_cast<size_t>(n));
  for (int i = 0; i < n; ++i) {
    if (dglobal(i) != 0.0) tri.emplace_back(i, i, dglobal(i));
  }
  k.setFromTriplets(tri.begin(), tri.end());
  return k;
#else
  return build_structural_stiffness_sparse(n);
#endif
}

DenseVector build_structural_load_distributed(int n) {
#ifdef GPTSOLVER_USE_MPI
  int init = 0;
  MPI_Initialized(&init);
  if (!init) {
    int argc = 0;
    char** argv = nullptr;
    MPI_Init(&argc, &argv);
  }
  int rank = 0, size = 1;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  DenseVector flocal = DenseVector::Zero(n);
  int b = 0, e = n;
  local_range(n, rank, size, b, e);
  if (n > 0 && n - 1 >= b && n - 1 < e) flocal(n - 1) = 1.0;

  DenseVector fglobal = DenseVector::Zero(n);
  MPI_Allreduce(flocal.data(), fglobal.data(), n, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  return fglobal;
#else
  return build_structural_load(n);
#endif
}

SparseMatrix assemble_mpc_lagrange(int ndof, int master, int slave, double ratio, double penalty) {
  SparseMatrix k(ndof, ndof);
  std::vector<Triplet> tri;
  tri.emplace_back(master, master, penalty);
  tri.emplace_back(slave, slave, penalty * ratio * ratio);
  tri.emplace_back(master, slave, -penalty * ratio);
  tri.emplace_back(slave, master, -penalty * ratio);
  k.setFromTriplets(tri.begin(), tri.end());
  return k;
}

}  // namespace gptsolver
