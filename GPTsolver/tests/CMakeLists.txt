add_executable(test_parser unit/test_parser.cpp)
target_link_libraries(test_parser PRIVATE gptsolver_core)
add_test(NAME unit_parser COMMAND test_parser)

add_executable(test_solver unit/test_solver.cpp)
target_link_libraries(test_solver PRIVATE gptsolver_core)
add_test(NAME unit_solver COMMAND test_solver)

add_executable(test_vtu unit/test_vtu.cpp)
target_link_libraries(test_vtu PRIVATE gptsolver_core)
add_test(NAME unit_vtu COMMAND test_vtu)

add_executable(test_contact unit/test_contact.cpp)
target_link_libraries(test_contact PRIVATE gptsolver_core)
add_test(NAME unit_contact COMMAND test_contact)

add_executable(test_block unit/test_block.cpp)
target_link_libraries(test_block PRIVATE gptsolver_core)
add_test(NAME unit_block COMMAND test_block)


add_executable(test_contact_search unit/test_contact_search.cpp)
target_link_libraries(test_contact_search PRIVATE gptsolver_core)
add_test(NAME unit_contact_search COMMAND test_contact_search)

add_executable(test_shell unit/test_shell.cpp)
target_link_libraries(test_shell PRIVATE gptsolver_core)
add_test(NAME unit_shell COMMAND test_shell)
