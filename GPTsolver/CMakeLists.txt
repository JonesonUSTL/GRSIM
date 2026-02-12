cmake_minimum_required(VERSION 3.20)
project(GPTsolver VERSION 0.1.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(Options)
include(CompilerWarnings)
include(Dependencies)

add_library(gptsolver_core
  src/core/logger.cpp
  src/core/error.cpp
  src/io/inp/lexer.cpp
  src/io/inp/parser.cpp
  src/io/inp/semantic.cpp
  src/io/inp/compatibility.cpp
  src/io/vtk/vtu_writer.cpp
  src/io/vtk/pvd_writer.cpp
  src/io/restart/checkpoint.cpp
  src/assembly/assembler_structural.cpp
  src/assembly/assembler_thermal.cpp
  src/solver/linear/eigen_direct.cpp
  src/solver/nonlinear/newton_solver.cpp
  src/physics/structural/structural_problem.cpp
  src/physics/thermal/thermal_problem.cpp
  src/ui/console_dashboard.cpp
  src/subroutine/subroutine_api.cpp
  src/fem/constitutive/plastic/j2_return_mapping.cpp
)

target_include_directories(gptsolver_core PUBLIC include)
target_link_libraries(gptsolver_core PUBLIC Eigen3::Eigen)
target_compile_features(gptsolver_core PUBLIC cxx_std_20)
apply_project_warnings(gptsolver_core)
if(GPTSOLVER_ENABLE_OPENMP)
  target_link_libraries(gptsolver_core PUBLIC OpenMP::OpenMP_CXX)
  target_compile_definitions(gptsolver_core PUBLIC GPTSOLVER_USE_OPENMP=1)
endif()

add_executable(gptsolver apps/gptsolver_cli/main.cpp)
target_link_libraries(gptsolver PRIVATE gptsolver_core)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
