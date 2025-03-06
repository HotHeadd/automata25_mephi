cmake_minimum_required(VERSION 3.16)
project(time_counter)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_EXE timetest.cpp time_creators/time_creators.cpp)

add_subdirectory(../resolvers build_source)
add_subdirectory(generators)

add_executable(timetest ${SOURCE_EXE})
target_link_libraries(timetest Generators)
set_target_properties(timetest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)