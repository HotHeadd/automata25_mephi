add_library(MyReLib)

add_subdirectory(Regex)

target_include_directories(MyReLib PUBLIC include)

target_sources(MyReLib PRIVATE src/myre.cpp)

target_link_libraries(MyReLib PUBLIC RegexLib)