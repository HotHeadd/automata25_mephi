cmake_minimum_required(VERSION 3.16)
project(myre_tests)

set(CMAKE_C_COMPILER "g++-13.3")
set(CMAKE_CXX_STANDARD 23)
set(SOURCE_EXE ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.cpp)

enable_testing()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g")
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} --coverage -g")

SET(GTEST_LIBS gtest gtest_main gmock gmock_main)

add_subdirectory(../myre ./build_source)

add_subdirectory(parser_tests)

add_custom_target(coverage
	COMMAND lcov --capture --directory . --output-file coverage.info
	COMMAND lcov --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage_filtered.info
	COMMAND genhtml coverage_filtered.info --output-directory ../coverage_report --title "Test Coverage Report"
)