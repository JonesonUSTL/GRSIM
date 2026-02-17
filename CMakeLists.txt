cmake_minimum_required(VERSION 3.20)
project(GPTsolver VERSION 0.2.0 LANGUAGES CXX)

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
  src/io/inp/keyword_dispatcher.cpp
  src/io/vtk/vtu_writer.cpp
  src/io/vtk/pvd_writer.cpp
  src/io/restart/checkpoint.cpp
  src/assembly/csr_matrix.cpp
  src/assembly/assembler_structural.cpp
  src/assembly/assembler_thermal.cpp
  src/assembly/assembler_contact_placeholder.cpp
  src/solver/linear/eigen_direct.cpp
  src/solver/linear/eigen_iterative.cpp
  src/solver/linear/schur_preconditioner.cpp
  src/solver/linear/petsc_backend.cpp
  src/solver/nonlinear/newton_solver.cpp
  src/solver/nonlinear/increment_controller.cpp
  src/solver/nonlinear/line_search.cpp
  src/physics/structural/structural_problem.cpp
  src/physics/thermal/thermal_problem.cpp
  src/ui/console_dashboard.cpp
  src/subroutine/subroutine_api.cpp
  src/fem/constitutive/plastic/j2_return_mapping.cpp
  src/fem_shell_placeholder.cpp
  src/model/contact_pair.cpp
  src/model/surface.cpp
)

target_include_directories(gptsolver_core PUBLIC include)
target_link_libraries(gptsolver_core PUBLIC Eigen3::Eigen)
target_compile_features(gptsolver_core PUBLIC cxx_std_20)
apply_project_warnings(gptsolver_core)
if(GPTSOLVER_ENABLE_OPENMP)
  target_link_libraries(gptsolver_core PUBLIC OpenMP::OpenMP_CXX)
  target_compile_definitions(gptsolver_core PUBLIC GPTSOLVER_USE_OPENMP=1)
endif()
if(GPTSOLVER_ENABLE_PETSC AND PETSC_FOUND)
  target_link_libraries(gptsolver_core PUBLIC ${PETSC_LIBRARIES})
  target_include_directories(gptsolver_core PUBLIC ${PETSC_INCLUDES})
  target_compile_definitions(gptsolver_core PUBLIC GPTSOLVER_HAS_PETSC=1)
endif()
if(GPTSOLVER_ENABLE_MPI AND MPI_CXX_FOUND)
  target_link_libraries(gptsolver_core PUBLIC MPI::MPI_CXX)
  target_compile_definitions(gptsolver_core PUBLIC GPTSOLVER_USE_MPI=1)
endif()

add_executable(gptsolver apps/gptsolver_cli/main.cpp)
target_link_libraries(gptsolver PRIVATE gptsolver_core)
add_executable(grsim apps/gptsolver_cli/main.cpp)
target_link_libraries(grsim PRIVATE gptsolver_core)

set(BUILD_TESTING ON CACHE BOOL "Enable project tests" FORCE)
include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
