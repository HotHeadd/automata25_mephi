add_library(Generators)
target_include_directories(Generators PUBLIC include ../../interfaces)

target_sources(Generators PRIVATE source/CorrectGenerator.cpp
                                 source/IncorrectGenerator.cpp)