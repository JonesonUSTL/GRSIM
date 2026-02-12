add_executable(test_parser unit/test_parser.cpp)
target_link_libraries(test_parser PRIVATE gptsolver_core)
add_test(NAME unit_parser COMMAND test_parser)

add_executable(test_solver unit/test_solver.cpp)
target_link_libraries(test_solver PRIVATE gptsolver_core)
add_test(NAME unit_solver COMMAND test_solver)

add_executable(test_vtu unit/test_vtu.cpp)
target_link_libraries(test_vtu PRIVATE gptsolver_core)
add_test(NAME unit_vtu COMMAND test_vtu)
