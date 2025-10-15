cmake_minimum_required(VERSION 3.15)
project(parallel_lights_io)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE DEFS "src/*.def")

add_library(parallel_lights_io SHARED ${SOURCES})

target_include_directories(parallel_lights_io PUBLIC include)
target_link_libraries(parallel_lights_io PRIVATE lightsdriver)

set_target_properties(parallel_lights_io PROPERTIES OUTPUT_NAME "parallel_lights_io")
set_target_properties(parallel_lights_io PROPERTIES PREFIX "")

# Generate test executable

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
add_executable(parallel_lights_io_test ${TEST_SOURCES})

target_link_options(parallel_lights_io_test PRIVATE
    -static
)
target_link_options(parallel_lights_io PRIVATE
    -static
)

