include(FetchContent)

if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()
if(POLICY CMP0169)
  cmake_policy(SET CMP0169 OLD)
endif()

find_package(Eigen3 QUIET NO_MODULE)
if(NOT TARGET Eigen3::Eigen)
  FetchContent_Declare(
    eigen
    URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
  FetchContent_GetProperties(eigen)
  if(NOT eigen_POPULATED)
    FetchContent_Populate(eigen)
  endif()

  add_library(Eigen3::Eigen INTERFACE IMPORTED)
  target_include_directories(Eigen3::Eigen INTERFACE ${eigen_SOURCE_DIR})
endif()

if(GPTSOLVER_ENABLE_OPENMP)
  find_package(OpenMP QUIET)
  if(NOT OpenMP_CXX_FOUND)
    message(STATUS "OpenMP 未找到：自动降级为串行构建（GPTSOLVER_ENABLE_OPENMP=OFF）。")
    set(GPTSOLVER_ENABLE_OPENMP OFF CACHE BOOL "Enable OpenMP assembly" FORCE)
  endif()
endif()

if(GPTSOLVER_ENABLE_PETSC)
  find_package(PETSc QUIET)
endif()

if(GPTSOLVER_ENABLE_MPI)
  find_package(MPI QUIET)
  if(NOT MPI_CXX_FOUND)
    message(STATUS "MPI 未找到：自动降级为串行构建（GPTSOLVER_ENABLE_MPI=OFF）。")
    set(GPTSOLVER_ENABLE_MPI OFF CACHE BOOL "Enable MPI distributed assembly and iterative solver" FORCE)
  endif()
endif()
