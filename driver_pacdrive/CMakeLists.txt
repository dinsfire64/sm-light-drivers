cmake_minimum_required(VERSION 3.15)
project(pacdrive)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(pacdrive SHARED ${SOURCES})

target_include_directories(pacdrive PUBLIC include)
target_link_libraries(pacdrive PRIVATE lightsdriver)

set_target_properties(pacdrive PROPERTIES OUTPUT_NAME "PacDrive32")
set_target_properties(pacdrive PROPERTIES PREFIX "")

# Generate test executable

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
add_executable(pacdrive_test ${TEST_SOURCES})

target_link_options(pacdrive_test PRIVATE
    -static
    -s
)
target_link_options(pacdrive PRIVATE
    -static
    -s
)