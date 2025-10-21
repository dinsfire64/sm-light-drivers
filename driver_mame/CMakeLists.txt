cmake_minimum_required(VERSION 3.15)
project(mame)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(mame ${SOURCES})

target_include_directories(mame PUBLIC include)
target_link_libraries(mame PRIVATE lightsdriver)
target_link_libraries(mame PRIVATE ws2_32)

target_link_options(mame PRIVATE
    -static
    -s
)
