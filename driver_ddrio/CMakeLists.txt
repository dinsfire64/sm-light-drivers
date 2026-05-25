cmake_minimum_required(VERSION 3.15)
project(ddrio)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE DEFS "src/*.def")

add_library(ddrio SHARED ${SOURCES} ${DEFS})

target_include_directories(ddrio PUBLIC include)
target_link_libraries(ddrio PRIVATE lightsdriver)

set_target_properties(ddrio PROPERTIES OUTPUT_NAME "ddrio")
set_target_properties(ddrio PROPERTIES PREFIX "")

target_link_options(ddrio PRIVATE
    -static
    -s
    "-Wl,--enable-stdcall-fixup"
)

