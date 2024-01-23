cmake_minimum_required(VERSION 3.7)

project (LicenseManager CXX)
set(LICENSECC_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/licensecc/install/lib/cmake/licensecc/")
set(LCC_PROJECT_NAME "Raptor")
set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/licensecc/install")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/licensecc)
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")
add_custom_target(install_licensecc ALL
    COMMENT "Building and installing licensecc"
    COMMAND $(MAKE) install
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/licensecc
)

add_dependencies(install_licensecc licensecc_static)

add_executable(example src/example.cpp  src/License_manager.cpp)
add_library(rs_licenseManager src/License_manager.cpp)
cmake_policy(SET CMP0028 NEW)
target_link_libraries(rs_licenseManager licensecc_static)
target_link_libraries(example licensecc_static)