cmake_minimum_required(VERSION 3.15)
project(lightsdriver)

file(GLOB_RECURSE SOURCES "src/*.cpp") 

add_library(lightsdriver STATIC ${SOURCES})

target_include_directories(lightsdriver PUBLIC include)
target_include_directories(lightsdriver PUBLIC ../simpleini)

target_link_libraries(lightsdriver PRIVATE hidapi)
