#!/usr/bin/env bash
set -e

#enforce clang
git ls-files '*.c' '*.cpp' '*.h' '*.hpp' '*.cc' '*.hh' \ | xargs clang-format -i

cmake -S . -B build-win32 \
	-DCMAKE_TOOLCHAIN_FILE=toolchain/win32-toolchain.cmake \
	-DCMAKE_BUILD_TYPE=Release
cmake --build build-win32 --parallel

cmake -S . -B build-win64 \
	-DCMAKE_TOOLCHAIN_FILE=toolchain/win64-toolchain.cmake \
	-DCMAKE_BUILD_TYPE=Release
cmake --build build-win64 --parallel
