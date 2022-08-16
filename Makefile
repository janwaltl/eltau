.PHONY: all debug release coverage junit test format sca docs clean

all: debug test

COMPILER=-DCMAKE_CXX_COMPILER=${CXX}

debug:
	cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build/debug ${COMPILER}
	cmake --build build/debug -- -j`nproc`
	cp build/debug/compile_commands.json build/

release:
	cmake -DCMAKE_BUILD_TYPE=ReleaseWithDebInfo -S . -B build/release ${COMPILER}
	cmake --build build/release -- -j`nproc`
	cp build/release/compile_commands.json build/

# Sanitized builds
msan: COMPILER=-DCMAKE_CXX_COMPILER=clang++
msan:
	cmake -DCMAKE_BUILD_TYPE=ReleaseWithDebInfo -S . -B build/msan ${COMPILER}
	cmake --build build/msan -- -j`nproc` 
	cp build/msan/compile_commands.json build/

tsan:
	cmake -DCMAKE_BUILD_TYPE=ReleaseWithDebInfo -S . -B build/msan ${COMPILER}
	cmake --build build/tsan -- -j`nproc` 
	cp build/tsan/compile_commands.json build/

asan:
	cmake -DCMAKE_BUILD_TYPE=ReleaseWithDebInfo -S . -B build/asan ${COMPILER}
	cmake --build build/asan -- -j`nproc` 
	cp build/asan/compile_commands.json build/

test:
	cmake -DCMAKE_BUILD_TYPE=ReleaseWithDebInfo ${COMPILER} -S . -B build/release
	cmake --build build/release -- -j`nproc`
	cp build/release/compile_commands.json build/
	scripts/run_tests.sh build/release

junit:
	cmake -DCMAKE_BUILD_TYPE=Release -DELTAU_JUNIT_TEST_OUTPUT=ON  ${COMPILER} -S . -B build/junit
	cmake --build build/junit -- -j`nproc`
	scripts/run_unit.sh build/junit
	cp build/junit/compile_commands.json build/

coverage:
	cmake -DCMAKE_BUILD_TYPE=Debug -S . -B build/coverage -DELTAU_ENABLE_COVERAGE=ON  ${COMPILER}
	cmake --build build/coverage -- -j`nproc`
	scripts/gen_coverage.sh build/coverage
	cp build/coverage/compile_commands.json build/

format:
	scripts/format_codebase.sh

sca: debug
	./scripts/filter_libs.sh build/compile_commands.json
	run-clang-tidy -p build/

mtest: msan
	scripts/run_tests.sh build/msan

ttest: tsan
	scripts/run_tests.sh build/tsan

atest: asan
	scripts/run_tests.sh build/asan

docs:
	doxygen

clean:
	rm -rf build
