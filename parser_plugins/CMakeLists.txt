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

add_library(dummy_parser_plugins_lib STATIC ${PROJECT_SOURCE_DIR}/dummy.cpp)

if (RAPTOR)
  set(YOSYS_PATH ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/)
  set(YOSYS_CONFIG ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/yosys-config)
  message("YOSYS_PATH:" ${YOSYS_PATH})
  message("YOSYS_CONFIG:" ${YOSYS_CONFIG})
else()
  set(YOSYS_PATH ${PROJECT_SOURCE_DIR}/synlig_plugin/third_party/yosys/)
  set(YOSYS_CONFIG ${PROJECT_SOURCE_DIR}/install/bin/yosys-config)
endif()

if (RAPTOR)
  FILE(COPY include/simplify.cc
       DESTINATION synlig_plugin/third_party/yosys_mod/simplify.cc)
endif()

add_custom_target(surelog_build DEPENDS ${PROJECT_SOURCE_DIR}/install/bin/surelog)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/install/bin/surelog
    COMMAND echo "       Compiling Surelog"
    COMMAND make -j
    COMMAND make -j install PREFIX=${PROJECT_SOURCE_DIR}/install
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/synlig_plugin/third_party/surelog"
    DEPENDS ${PROJECT_SOURCE_DIR}/synlig_plugin/third_party/surelog/src/main.cpp
  )


add_custom_target(synlig_yosys_build DEPENDS ${PROJECT_SOURCE_DIR}/synlig_plugin/out/release/bin/yosys)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/synlig_plugin/out/release/bin/yosys
    COMMAND echo "       Compiling Yosys for Synlig plugin"
    COMMAND ${CMAKE_COMMAND} -E remove_directory synlig_plugin/third_party/yosys
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${YOSYS_PATH} ${PROJECT_SOURCE_DIR}/synlig_plugin/third_party/yosys
    COMMAND make CONFIG=gcc PREFIX=${YOSYS_PATH} install -j 
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/synlig_plugin/third_party/yosys"
    DEPENDS ${PROJECT_SOURCE_DIR}/synlig_plugin/third_party/yosys_mod/simplify.cc
  )

add_custom_target(synlig_build DEPENDS ${PROJECT_SOURCE_DIR}/synlig_plugin/build/release/systemverilog-plugin/systemverilog.so)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/synlig_plugin/build/release/systemverilog-plugin/systemverilog.so
    COMMAND echo "       Compiling SV plugin"
    COMMAND make build YOSYS_PATH=${YOSYS_PATH} YOSYS_CONFIG=${YOSYS_CONFIG} UHDM_INSTALL_DIR=${PROJECT_SOURCE_DIR}/install PLUGIN_LIST=systemverilog
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/install/share/yosys/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/synlig_plugin/build/release/systemverilog-plugin/systemverilog.so ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/install/share/yosys/plugins/
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/synlig_plugin"
    DEPENDS ${PROJECT_SOURCE_DIR}/synlig_plugin/frontends/systemverilog/uhdm_ast.cc
  )

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   set (BUILD_DIR dbuild)
else()
   set (BUILD_DIR build)
endif()

add_custom_target(ghdl_plugin_build DEPENDS ${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin/ghdl.so)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin/ghdl.so
    COMMAND echo "       Compiling GHDL plugin"
    COMMAND make install YOSYS_PATH=${YOSYS_PATH} YOSYS_CONFIG=${YOSYS_CONFIG} GHDL=${PROJECT_SOURCE_DIR}/../../${BUILD_DIR}/bin/HDL_simulator/GHDL/bin/ghdl
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/install/share/yosys/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin/ghdl.so ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/install/share/yosys/plugins/
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin"
    DEPENDS ${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin/src/ghdl.cc
  )

if (RAPTOR)
else()
  add_custom_target(yosys DEPENDS ${PROJECT_SOURCE_DIR}/install/bin/yosys)
  add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/install/bin/yosys
    COMMAND echo "       Compiling Yosys"
    COMMAND make CONFIG=gcc PREFIX=${PROJECT_SOURCE_DIR}/install install -j
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/synlig_plugin/third_party/yosys"
    DEPENDS ${PROJECT_SOURCE_DIR}/synlig_plugin/third_party/yosys/kernel/yosys.cc
  )
endif()


  
add_dependencies(dummy_parser_plugins_lib surelog_build synlig_build ghdl_plugin_build)
add_dependencies(synlig_build surelog_build)
add_dependencies(synlig_build synlig_yosys_build)
add_dependencies(ghdl_plugin_build yosys)
add_dependencies(synlig_build yosys)
