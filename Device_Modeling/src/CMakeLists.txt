cmake_minimum_required(VERSION 3.10)
project(Device_Modeling)

set(CMAKE_CXX_STANDARD 17)

# Add the Google Test directory to the include path
include_directories(${PROJECT_SOURCE_DIR}/../external/googletest/googletest/include ${PROJECT_SOURCE_DIR}/../external/googletest/googlemock/include)

# Add the Google Test library
add_subdirectory(${PROJECT_SOURCE_DIR}/../external/googletest ${CMAKE_CURRENT_BINARY_DIR}/googletest)

# Add the spdlog directory to the include path
include_directories(${PROJECT_SOURCE_DIR}/../external/spdlog/include)

# Add the exprtk directory to the include path
include_directories(${PROJECT_SOURCE_DIR}/../external/exprtk)

# Add the source directory to the include path
include_directories(${PROJECT_SOURCE_DIR}/device_spec ${PROJECT_SOURCE_DIR}/configuration)

# Add the source directory to the include path
include_directories(${PROJECT_SOURCE_DIR}/utilities)

# Add the test directory to the include path
include_directories(${PROJECT_SOURCE_DIR}/../test)

file(GLOB TEST_SRC "${PROJECT_SOURCE_DIR}/../test/*.cpp")

add_executable(Device_Modeling_test ${TEST_SRC})

# Link the Google Test library
target_link_libraries(Device_Modeling_test gtest gtest_main)
