file(GLOB FILES *.cpp)
SET(syntax_LIBS RegexLib)
add_executable(syntax_exec ${SOURCE_EXE} ${FILES})
target_link_libraries(syntax_exec PRIVATE ${GTEST_LIBS} ${syntax_LIBS})
add_test(NAME syntax COMMAND syntax_exec)

set_tests_properties(syntax PROPERTIES LABELS "syntax Test")