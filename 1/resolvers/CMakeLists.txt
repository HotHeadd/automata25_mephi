add_library(FlexResolver)
add_library(SMCResolver)
add_library(RegexResolver)

target_include_directories(FlexResolver PUBLIC include ../interfaces ../re2c_files)
target_include_directories(SMCResolver PUBLIC include ../interfaces ../smc_files)
target_include_directories(RegexResolver PUBLIC include ../interfaces)

target_sources(FlexResolver PRIVATE source/FlexResolver.cpp ../re2c_files/generated.cpp)
target_sources(SMCResolver PRIVATE source/SMCResolver.cpp ../smc_files/SMCResolver_sm.cpp)
target_sources(RegexResolver PRIVATE source/RegexResolver.cpp)