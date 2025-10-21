cmake_minimum_required(VERSION 3.15)
project(inpout32)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE DEFS "src/*.def")

add_library(inpout32 SHARED ${SOURCES} ${DEFS})

target_include_directories(inpout32 PUBLIC include)
target_link_libraries(inpout32 PRIVATE lightsdriver)

set_target_properties(inpout32 PROPERTIES OUTPUT_NAME "inpout32")
set_target_properties(inpout32 PROPERTIES PREFIX "")

# Generate test executable

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
add_executable(inpout32_test ${TEST_SOURCES})

target_link_options(inpout32_test PRIVATE
    -static
)
target_link_options(inpout32 PRIVATE
    -static
    "-Wl,--enable-stdcall-fixup"
)

