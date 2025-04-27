cmake_minimum_required(VERSION 3.16)
project(Benchmark)

set(CMAKE_C_COMPILER "g++-13.3")
set(CMAKE_CXX_STANDARD 23)
set(SOURCE_EXE ${CMAKE_CURRENT_SOURCE_DIR}/benchmark.cpp)

add_subdirectory(../myre ./build_source)

add_executable(run_bm ${SOURCE_EXE})

set_target_properties(run_bm PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

target_link_libraries(run_bm PUBLIC MyReLib)