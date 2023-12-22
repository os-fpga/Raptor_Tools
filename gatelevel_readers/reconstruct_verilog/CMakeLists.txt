cmake_minimum_required(VERSION 3.10) # Adjust to your CMake version

project(reconstruct_verilog)

# Specify the C++ standard you are using, e.g., C++11, C++14, C++17, C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the path where CMake can find additional CMake modules (if any)
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Collect all the source files to be compiled
add_executable(finalize src/reconstruct_verilog.cpp)
target_include_directories(finalize PUBLIC  "${PROJECT_SOURCE_DIR}../../libs/external/boost")
set_target_properties(finalize PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# If you have any additional libraries to link, use target_link_libraries
# target_link_libraries(reconstruct_verilog PRIVATE some_library)
