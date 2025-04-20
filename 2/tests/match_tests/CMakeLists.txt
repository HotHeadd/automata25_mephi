file(GLOB FILES *.cpp)
SET(MATCH_LIBS MyReLib)
add_executable(match_exec ${SOURCE_EXE} ${FILES})
target_link_libraries(match_exec PRIVATE ${GTEST_LIBS} ${MATCH_LIBS})
add_test(NAME match COMMAND match_exec)

set_tests_properties(match PROPERTIES LABELS "Match tests")