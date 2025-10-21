cmake_minimum_required(VERSION 3.15)
project(mmmagic)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE DEFS "src/*.def")

add_library(mmmagic SHARED ${SOURCES} ${DEFS})

target_include_directories(mmmagic PUBLIC include)
target_link_libraries(mmmagic PRIVATE lightsdriver)

set_target_properties(mmmagic PROPERTIES OUTPUT_NAME "mmmagic")
set_target_properties(mmmagic PROPERTIES PREFIX "")

# Generate test executable

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
add_executable(mmmagic_test ${TEST_SOURCES})

target_link_options(mmmagic_test PRIVATE
    -static
    -s
)
target_link_options(mmmagic PRIVATE
    -static
    -s
)

