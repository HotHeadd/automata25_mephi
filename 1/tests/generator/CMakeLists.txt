add_library(Generator)
target_include_directories(Generator PUBLIC include)

target_sources(Generator PRIVATE source/Generator.cpp)