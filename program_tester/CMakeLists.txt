cmake_minimum_required(VERSION 3.15)
project(tester)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(tester ${SOURCES})

target_include_directories(tester PUBLIC include)
target_link_libraries(tester PRIVATE lightsdriver)

target_link_options(tester PRIVATE
    -static
    -s
)
