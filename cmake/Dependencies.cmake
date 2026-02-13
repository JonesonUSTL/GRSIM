include(FetchContent)
if(NOT TARGET Eigen3::Eigen)
  FetchContent_Declare(
    eigen
    URL https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.tar.gz)
  FetchContent_Populate(eigen)
  add_library(Eigen3::Eigen INTERFACE IMPORTED)
  target_include_directories(Eigen3::Eigen INTERFACE ${eigen_SOURCE_DIR})
endif()
if(GPTSOLVER_ENABLE_OPENMP)
  find_package(OpenMP REQUIRED)
endif()
