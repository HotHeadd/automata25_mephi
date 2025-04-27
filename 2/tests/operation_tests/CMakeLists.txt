file(GLOB FILES *.cpp)
SET(OPER_LIBS MyReLib)
add_executable(oper_exec ${SOURCE_EXE} ${FILES})
target_link_libraries(oper_exec PRIVATE ${GTEST_LIBS} ${OPER_LIBS})
add_test(NAME oper COMMAND oper_exec)

set_tests_properties(oper PROPERTIES LABELS "Operations Test")