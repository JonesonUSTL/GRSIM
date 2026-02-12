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


add_executable(test_contact_bucket unit/test_contact_bucket.cpp)
target_link_libraries(test_contact_bucket PRIVATE gptsolver_core)
add_test(NAME unit_contact_bucket COMMAND test_contact_bucket)

add_executable(test_schur unit/test_schur.cpp)
target_link_libraries(test_schur PRIVATE gptsolver_core)
add_test(NAME unit_schur COMMAND test_schur)


add_executable(test_projection unit/test_projection.cpp)
target_link_libraries(test_projection PRIVATE gptsolver_core)
add_test(NAME unit_projection COMMAND test_projection)


add_executable(test_keyword_dispatch unit/test_keyword_dispatch.cpp)
target_link_libraries(test_keyword_dispatch PRIVATE gptsolver_core)
add_test(NAME unit_keyword_dispatch COMMAND test_keyword_dispatch)


add_executable(test_frames unit/test_frames.cpp)
target_link_libraries(test_frames PRIVATE gptsolver_core)
add_test(NAME unit_frames COMMAND test_frames)


add_executable(test_official_inp unit/test_official_inp.cpp)
target_link_libraries(test_official_inp PRIVATE gptsolver_core)
add_test(NAME unit_official_inp COMMAND test_official_inp)
