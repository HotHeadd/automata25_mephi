MAKEFLAGS += --no-print-directory
BUILD_DIR = build
BUILD_TEST_DIR = buildtests

TARGET = run

.PHONY : build run runsec setup_test_dirs timetest

final: ${TARGET}

run: build
	./bin/run

build:
	mkdir -p ${BUILD_DIR}
	cmake -B${BUILD_DIR} -S.
	cmake --build ${BUILD_DIR}

timetest:
	mkdir -p ${BUILD_TEST_DIR}
	cmake -B{BUILD_TEST_DIR} -Stests
	cmake --build ${BUILD_TEST_DIR}
	./bin/timetest