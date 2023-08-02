cmake_minimum_required(VERSION 3.10)

# set the project name
project(gen_map)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/external/ryml/)
add_executable(generate_map ${CMAKE_CURRENT_SOURCE_DIR}/src/gen_map.cpp)