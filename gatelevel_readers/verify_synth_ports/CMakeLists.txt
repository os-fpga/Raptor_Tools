cmake_minimum_required(VERSION 3.10)

# set the project name
project(compare_ports)


file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

include_directories(PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/external/nlohmann_json/
)

# Create the static library
add_library(libcompare SHARED
    ${LIB_SOURCES}
)
set_target_properties(libcompare PROPERTIES PREFIX "") # Avoid extra 'lib' prefix

file(GLOB_RECURSE EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/compare_ports.cpp)

list(APPEND EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

# create executable
add_executable(verify_synth_ports
    ${EXEC_SOURCE}
)
