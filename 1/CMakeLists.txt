cmake_minimum_required(VERSION 3.16)
project(resolver)

set(CMAKE_CXX_STANDARD 20)
set(SOURCE_EXE run.cpp)

#find_package()

add_subdirectory(resolvers)
add_subdirectory(tests/generators)

add_executable(run ${SOURCE_EXE})

set_target_properties(run PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_link_libraries(run RegexResolver Generator)