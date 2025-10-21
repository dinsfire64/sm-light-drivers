#!/usr/bin/env bash
set -e

cmake -S . -B build-win32 \
	-DCMAKE_TOOLCHAIN_FILE=toolchain/win32-toolchain.cmake \
	-DCMAKE_BUILD_TYPE=Release
cmake --build build-win32 --parallel

cmake -S . -B build-win64 \
	-DCMAKE_TOOLCHAIN_FILE=toolchain/win64-toolchain.cmake \
	-DCMAKE_BUILD_TYPE=Release
cmake --build build-win64 --parallel
