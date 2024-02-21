# -*- mode:cmake -*-

# Copyright 2021-2023 The RapidSilicon team

cmake_minimum_required(VERSION 3.20)

set (BUILD_TYPE_STRING Engineering)
if (PRODUCTION_BUILD)
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

#option(
#  WITH_LIBCXX
#  "Building with clang++ and libc++(in Linux). To enable with: -DWITH_LIBCXX=On"
#  On)

project(YOSYS_SV)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
   set (BUILD_DIR dbuild)
else()
   set (BUILD_DIR build)
endif()


add_library(dummy_parser_plugins_lib STATIC ${PROJECT_SOURCE_DIR}/dummy.cpp)

if (RAPTOR)
  set(YOSYS_PATH ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/)
  set(YOSYS_CONFIG ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/yosys-config)
  set(GHDL_YOSYS_CONFIG ${PROJECT_SOURCE_DIR}/../../yosys_verific_rs/yosys/yosys-config)
  set(GHDL_BIN_PATH ${PROJECT_SOURCE_DIR}/../../${BUILD_DIR}/bin/HDL_simulator/GHDL/bin/ghdl)
elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../yosys)
  #message(STATUS "********* using Yosys from yosys_verific_rs level")
  set(YOSYS_PATH ${PROJECT_SOURCE_DIR}/../../yosys/)
  set(YOSYS_CONFIG ${PROJECT_SOURCE_DIR}/../../yosys/yosys-config)
  set(GHDL_YOSYS_CONFIG ${PROJECT_SOURCE_DIR}/../../yosys/yosys-config)
  set(GHDL_BIN_PATH ${PROJECT_SOURCE_DIR}/../HDL_simulator/build/bin/HDL_simulator/GHDL/bin/ghdl)
else()
  set(YOSYS_PATH ${PROJECT_SOURCE_DIR}/synlig/third_party/yosys/)
  # Do not set YOSYS_CONFIG in this case
  set(GHDL_YOSYS_CONFIG ${PROJECT_SOURCE_DIR}/synlig/third_party/yosys/yosys-config)
  set(GHDL_BIN_PATH ${PROJECT_SOURCE_DIR}/../HDL_simulator/build/bin/HDL_simulator/GHDL/bin/ghdl)
endif()

message("YOSYS_PATH:" ${YOSYS_PATH})
message("YOSYS_CONFIG:" ${YOSYS_CONFIG})
message("GHDL_BIN_PATH:" ${GHDL_BIN_PATH})

if (RAPTOR)
  FILE(COPY ${PROJECT_SOURCE_DIR}/include/synlig_simplify.cc
       DESTINATION ${PROJECT_SOURCE_DIR}/synlig/third_party/yosys_mod/)

elseif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../yosys)
  FILE(COPY ${PROJECT_SOURCE_DIR}/include/synlig_simplify.cc
       DESTINATION ${PROJECT_SOURCE_DIR}/synlig/third_party/yosys_mod/)
else()
    message(STATUS "No copying synlig_simplify.cc as synling yosys will be used")
endif()

# TODO: The best solution would be to do the following:
#   add_sybsystem(synlig_plugin)
# instead of the current: 
#   add_custom_target(synlig_build ...)
# and to make the targets depend on synlig lib, but the problem is json, capnp, common libraries that gets multiply defined at Raptor level.

add_custom_target(synlig_build DEPENDS ${PROJECT_SOURCE_DIR}/synlig/out/current/share/yosys/plugins/systemverilog.so)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/synlig/out/current/share/yosys/plugins/systemverilog.so
    COMMAND echo "       Compiling SV plugin"
    COMMAND make -f cmake-makefile YOSYS_CONFIG=${YOSYS_CONFIG} YOSYS_PATH=${YOSYS_PATH} -j ${CPU_CORES}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${YOSYS_PATH}/install/share/yosys/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/synlig/out/current/share/yosys/plugins/systemverilog.so ${YOSYS_PATH}/install/share/yosys/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/synlig/out/current/share/yosys/plugins/systemverilog.so.* ${YOSYS_PATH}/install/share/yosys/plugins/
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/synlig"
    DEPENDS ${PROJECT_SOURCE_DIR}/synlig/frontends/systemverilog/uhdm_ast.cc
  )

add_custom_target(ghdl_install DEPENDS ${GHDL_BIN_PATH})
add_custom_command(OUTPUT ${GHDL_BIN_PATH}
    COMMAND echo "       Installing GHDL"
    COMMAND make install 
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/../HDL_simulator"
    DEPENDS ${PROJECT_SOURCE_DIR}/../HDL_simulator/CMakeLists.txt
  )

add_custom_target(ghdl_plugin_build DEPENDS ${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin/ghdl.so)
add_custom_command(OUTPUT ${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin/ghdl.so
    COMMAND echo "       Compiling GHDL plugin"
    COMMAND make install YOSYS_PATH=${YOSYS_PATH} YOSYS_CONFIG=${GHDL_YOSYS_CONFIG} GHDL=${GHDL_BIN_PATH}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${YOSYS_PATH}/install/share/yosys/plugins/
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin/ghdl.so ${YOSYS_PATH}/install/share/yosys/plugins/
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin"
    DEPENDS ${PROJECT_SOURCE_DIR}/ghdl-yosys-plugin/src/ghdl.cc
  )
  
add_dependencies(dummy_parser_plugins_lib synlig_build ghdl_plugin_build)
if (RAPTOR)
else()
  add_dependencies(ghdl_plugin_build ghdl_install)
endif()
add_dependencies(ghdl_plugin_build synlig_build)
