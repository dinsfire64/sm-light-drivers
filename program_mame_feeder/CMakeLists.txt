cmake_minimum_required(VERSION 3.15)
project(mame_feeder)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(mame_feeder ${SOURCES})

target_include_directories(mame_feeder PUBLIC include)
target_link_libraries(mame_feeder PRIVATE lightsdriver)
target_link_libraries(mame_feeder PRIVATE ws2_32)

target_link_options(mame_feeder PRIVATE
    -static
    -s
)
