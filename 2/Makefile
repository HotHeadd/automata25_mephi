MAKEFLAGS += --no-print-directory
BUILD_DIR = build
BUILD_TEST_DIR = buildtests

TARGET = run

.PHONY : build run

final: ${TARGET}

run: build
	./bin/run 3

build:
#	make -C re2c_files
	mkdir -p bin
	mkdir -p ${BUILD_DIR}
	
	cmake -B${BUILD_DIR} -S.
	cmake --build ${BUILD_DIR}