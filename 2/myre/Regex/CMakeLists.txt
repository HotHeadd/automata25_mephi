add_library(RegexLib)

target_include_directories(RegexLib PUBLIC include)

target_sources(RegexLib PRIVATE src/regex.cpp
							 	src/parser.cpp
								src/dfa.cpp)