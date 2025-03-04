add_library(Generator)
target_include_directories(Generator PUBLIC include ../../interfaces)

target_sources(Generator PRIVATE source/CorrectGenerator.cpp
                                 source/IncorrectGenerator.cpp)