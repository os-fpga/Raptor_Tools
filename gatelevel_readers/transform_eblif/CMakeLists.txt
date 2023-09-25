cmake_minimum_required(VERSION 3.10) # Adjust to your CMake version

project(transform_eblif)

# Specify the C++ standard you are using, e.g., C++11, C++14, C++17, C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the path where CMake can find additional CMake modules (if any)
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Add the path to the boost header files
include_directories("../../libs/external/boost")

# Collect all the source files to be compiled
add_executable(transform_eblif src/transform_blif.cpp)

# If you have any additional libraries to link, use target_link_libraries
# target_link_libraries(transform_eblif PRIVATE some_library)
