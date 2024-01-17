cmake_minimum_required(VERSION 3.0)

project(LicenseManager)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER g++)

add_library(rs_licenseManager_pic
       License_manager.cpp)
set_target_properties(rs_licenseManager_pic PROPERTIES PREFIX "") # Avoid extra 'lib' prefix
add_library(rs_licenseManager License_manager.cpp)
set_target_properties(rs_licenseManager PROPERTIES PREFIX "") # Avoid extra 'lib' prefix


set(CXX_OPTS "-fPIC;-fvisibility=hidden;-no-pie;-lrt;-Wl,--no-as-needed;-ldl;-pthread")
target_compile_options(rs_licenseManager PUBLIC ${CXX_OPTS})
set(CXX_OPTS "-fPIC")
target_compile_options(rs_licenseManager_pic PUBLIC ${CXX_OPTS})
