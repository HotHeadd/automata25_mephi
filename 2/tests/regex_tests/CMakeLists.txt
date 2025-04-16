file(GLOB FILES *.cpp)
SET(PARSER_LIBS RegexLib)
add_executable(search_exec ${SOURCE_EXE} ${FILES})
target_link_libraries(search_exec PRIVATE ${GTEST_LIBS} ${PARSER_LIBS})
add_test(NAME search COMMAND search_exec)

set_tests_properties(search PROPERTIES LABELS "Search")