cmake_minimum_required(VERSION 3.15)
project(lightDrivers VERSION 0.5)

# Link hidapi statically
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(libs/hidapi)
add_subdirectory(libs/simpleini)
add_subdirectory(libs/lightsdriver)
add_subdirectory(driver_pacdrive)
add_subdirectory(driver_parallel_lights_io)
add_subdirectory(driver_inpout32)
add_subdirectory(driver_mmmagic)
add_subdirectory(driver_ddrio)
add_subdirectory(program_mame_feeder)
add_subdirectory(program_tester)
