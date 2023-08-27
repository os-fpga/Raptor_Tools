# -*- mode:cmake -*-

# Copyright 2021-2023 The RapidSilicon team

cmake_minimum_required(VERSION 3.20)

set (BUILD_TYPE_STRING Engineering)
if (PRODUCTION_BUILD)
  message("Production Build type set to ON")
  set (PRODUCTION_BUILD_FLAG "-DPRODUCTION_BUILD=1")
  add_definitions(-DPRODUCTION_BUILD)
  set (BUILD_TYPE_STRING Production)
endif(PRODUCTION_BUILD)

# Detect build type, fallback to release and throw a warning if use didn't
# specify any
if(NOT CMAKE_BUILD_TYPE)
  message(WARNING "Build type not set, falling back to Release mode.
 To specify build type use:
 -DCMAKE_BUILD_TYPE=<mode> where <mode> is Debug or Release.")
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE STRING "Choose the type of build, options are: Debug Release."
            FORCE)
endif(NOT CMAKE_BUILD_TYPE)

option(
  WITH_LIBCXX
  "Building with clang++ and libc++(in Linux). To enable with: -DWITH_LIBCXX=On"
  On)

project(YOSYS_SV)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(dummy_lib STATIC ${PROJECT_SOURCE_DIR}/dummy.cpp)

set(YOSYS_PATH ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/)
set(YOSYS_CONFIG ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/yosys-config)

add_custom_target(surelog_build DEPENDS ${PROJECT_SOURCE_DIR}/install/bin/surelog)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/install/bin/surelog
    COMMAND echo "       Compiling Surelog"
    COMMAND make
    COMMAND make install PREFIX=${PROJECT_SOURCE_DIR}/install
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/Surelog"
    DEPENDS ${PROJECT_SOURCE_DIR}/Surelog/src/main.cpp
  )

add_custom_target(sv_plugin_build DEPENDS ${PROJECT_SOURCE_DIR}/yosys-f4pga-plugins/systemverilog-plugin/build/systemverilog.so)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/yosys-f4pga-plugins/systemverilog-plugin/build/systemverilog.so
    COMMAND echo "       Compiling SV plugin"
    COMMAND make install YOSYS_PATH=${YOSYS_PATH} YOSYS_CONFIG=${YOSYS_CONFIG} UHDM_INSTALL_DIR=${PROJECT_SOURCE_DIR}/install PLUGIN_LIST=systemverilog 
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/yosys-f4pga-plugins"
    DEPENDS ${PROJECT_SOURCE_DIR}/yosys-f4pga-plugins/systemverilog-plugin/UhdmAst.cc
  )
  
add_dependencies(dummy_lib surelog_build sv_plugin_build)
add_dependencies(sv_plugin_build surelog_build)
