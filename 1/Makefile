MAKEFLAGS += --no-print-directory
BUILD_DIR = build
BUILD_TEST_DIR = buildtests

TARGET = run

.PHONY : build run runsec setup_test_dirs timetest

final: ${TARGET}

run: build
	./bin/run 3

build:
	make -C flex_files
	make -C smc_files
	mkdir -p ${BUILD_DIR}
	cmake -B${BUILD_DIR} -S.
	cmake --build ${BUILD_DIR}
timetest:
	mkdir -p ${BUILD_TEST_DIR}
	cmake -B${BUILD_TEST_DIR} -Stests
	cmake --build ${BUILD_TEST_DIR}
	./bin/timetest 2