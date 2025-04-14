file(GLOB FILES *.cpp)
SET(PARSER_LIBS RegexLib)
add_executable(parser_exec ${SOURCE_EXE} ${FILES})
target_link_libraries(parser_exec PRIVATE ${GTEST_LIBS} ${PARSER_LIBS})
add_test(NAME parser COMMAND parser_exec)

set_tests_properties(parser PROPERTIES LABELS "Parser Test")