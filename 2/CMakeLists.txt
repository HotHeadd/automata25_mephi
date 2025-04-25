cmake_minimum_required(VERSION 3.16)
project(MyRe)

set(CMAKE_C_COMPILER "g++-13.3")
set(CMAKE_CXX_STANDARD 23)
set(SOURCE_EXE run.cpp)

add_subdirectory(myre)

add_executable(run ${SOURCE_EXE})

set_target_properties(run PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

target_link_libraries(run PUBLIC MyReLib)

target_include_directories(run PUBLIC display_functions)
target_sources(run PRIVATE display_functions/display.cpp)