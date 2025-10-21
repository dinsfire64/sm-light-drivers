#!/usr/bin/env bash
set -e

#clean the build
rm -rf release 2>/dev/null || true
rm -rf build-win32 2>/dev/null || true
rm -rf build-win64 2>/dev/null || true

#start the build
./build_all.sh

# Define output folders
mkdir -p release/BewaresExtreme release/OpenITG-NotITG release/ITGMania-OutFox release/StepMania5 release/StepMania3_95

# bewares needs inpout32 and custom bin
cp build-win32/driver_inpout32/*.dll release/BewaresExtreme || true
cp bins/bewares/* release/BewaresExtreme || true

#TODO: provide matching linked copy of pacdrive dll.
#cp build-win32/driver_pacdrive/*.dll release/BewaresExtreme/PacDrive.dll || true

# sm395 needs parallel_lights_io and custom bin
cp build-win32/driver_parallel_lights_io/*.dll release/StepMania3_95 || true
cp bins/sm3_95/* release/StepMania3_95 || true

# oitg/nitg needs parallel_lights_io
cp build-win32/driver_parallel_lights_io/*.dll release/OpenITG-NotITG || true
cp build-win32/driver_mmmagic/*.dll release/StepMania5 || true

# modern sm5 forks need 64bit minimaid library
cp build-win64/driver_mmmagic/*.dll release/ITGMania-OutFox/mmmagic64.dll || true

# testers/utilities
cp build-win32/driver_mame/*.exe release || true
cp build-win32/program_tester/*.exe release || true

zip -r "release_$(date +'%Y%m%d_%H-%M-%S').zip" release
