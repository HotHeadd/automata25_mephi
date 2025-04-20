add_library(RegexLib)

target_include_directories(RegexLib PUBLIC include)

target_sources(RegexLib PRIVATE src/parser.cpp
								src/dfa.cpp
								src/context.cpp
								src/syntax_node.cpp)