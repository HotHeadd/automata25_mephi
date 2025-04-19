MAKEFLAGS += --no-print-directory
BUILD_DIR = build
BUILD_TEST_DIR = buildtests

TARGET = test_search

.PHONY : build run tests runtests

final: ${TARGET}

run: build
	./bin/run

build:
	mkdir -p bin
	mkdir -p ${BUILD_DIR}
	
	cmake -B${BUILD_DIR} -S.
	cmake --build ${BUILD_DIR}

test_parser: setup_test_dirs
	cmake --build ${BUILD_TEST_DIR} --target parser_exec
	ctest --test-dir ${BUILD_TEST_DIR} --target -R parser --output-on-failure

test_search: setup_test_dirs
	cmake --build ${BUILD_TEST_DIR} --target search_exec
	ctest --test-dir ${BUILD_TEST_DIR} --target -R search --output-on-failure

setup_test_dirs:
	mkdir -p ${BUILD_TEST_DIR}
	mkdir -p ${BUILD_TEST_DIR}/build_source
	cmake -B${BUILD_TEST_DIR} -Stests

benchmark:
	mkdir -p build_bm
	mkdir -p build_bm/build_source

	cmake -Bbuild_bm -Scompile_benchmark
	cmake --build build_bm
	./bin/run_bm 
	