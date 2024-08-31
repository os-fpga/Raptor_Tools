# -*- mode:cmake -*-
cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(BITBLAST VERSION 1.0)

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

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})

option(
  WITH_LIBCXX
  "If buildling with clang++ and libc++(in Linux). To enable with: -DWITH_LIBCXX=On"
  OFF)

#find_package(Surelog PATHS  ../parser_plugins/synlig/build/third_party/surelog)

# NOTE: Policy changes has to happen before adding any subprojects
cmake_policy(SET CMP0091 NEW)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(BITBLAST_BUILD_TYPE ${CMAKE_BUILD_TYPE})
message("Building BITBLAST")

if(WITH_LIBCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
endif()

set(WITH_STATIC_CRT OFF CACHE BOOL "Use Static CRT")

# NOTE: Set the global output directories after the subprojects have had their go at it
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  # Force all .lib and .dll into bin for windows
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
else()
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TCMALLOC_COMPILE_OPTIONS} ${MY_CXX_WARNING_FLAGS}")

if(MSVC)
  add_compile_definitions(_CRT_NONSTDC_NO_WARNINGS)

  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} ${TCMALLOC_COMPILE_OPTIONS} /Zc:__cplusplus /W4 /bigobj ${MY_CXX_WARNING_FLAGS}"
  )
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
      "${CMAKE_CXX_FLAGS_RELEASE} ${TCMALLOC_COMPILE_OPTIONS} /Zc:__cplusplus /W4 /bigobj ${MY_CXX_WARNING_FLAGS}"
  )
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} ${TCMALLOC_COMPILE_OPTIONS} /Zc:__cplusplus /W4 /bigobj ${MY_CXX_WARNING_FLAGS}"
  )
  set(CMAKE_EXE_LINKER_FLAGS /STACK:8388608)  # 8MB stack size
elseif(WIN32 AND (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
  # The stack size unnecessarily high here. Investigate and bring it back to something more reasonable.
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Xclang --stack-size=33554432")  # 32MB stack size
else()
  if(DEFINED ENV{MSYSTEM})
    # Under MSYS some files are too large to build without additional flags
    set(MSYS_COMPILE_OPTIONS "-m64 -Wa,-mbig-obj")
  endif()
  #set(MEM_SANITIZER_FLAGS
  #    " -fsanitize=address -fno-omit-frame-pointer"
  #)
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} ${TCMALLOC_COMPILE_OPTIONS} -Wall -O0 -g ${MSYS_COMPILE_OPTIONS} ${MY_CXX_WARNING_FLAGS} ${MEM_SANITIZER_FLAGS}"
  )
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} ${TCMALLOC_COMPILE_OPTIONS} -Wall -O3 ${MSYS_COMPILE_OPTIONS} -DNDEBUG ${MY_CXX_WARNING_FLAGS} ${MEM_SANITIZER_FLAGS}"
  )
endif()

if (WIN32)
  add_compile_definitions(WIN32_LEAN_AND_MEAN)
endif()


# Put source code here, files that are generated at build time in
# surelog_generated_SRC
set(bitblast_SRC
  ${PROJECT_SOURCE_DIR}/src/main.cpp
  ${PROJECT_SOURCE_DIR}/src/NetlistPrettyPrinter.cpp
)

add_executable(bitblast-bin ${bitblast_SRC})

set_target_properties(bitblast-bin PROPERTIES OUTPUT_NAME bitblast)

# Raptor builds synlig which builds Surelog and UHDM, but it does not "install" them on the system.
# The usual cmake package discovery does not work for Surelog if Surelog is not installed.
# Here we manually configure the header search path and the library binding which otherwize would be taken care by 
#  find_package(Surelog)

target_include_directories(bitblast-bin PUBLIC
                           ../parser_plugins/synlig/third_party/surelog/include
                           ../parser_plugins/synlig/build/third_party/surelog/generated/include)


set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
set (SURELOG_BASE ../parser_plugins/synlig/build/third_party/surelog)
find_library(SURELOG_LIB NAMES surelog PATHS ${SURELOG_BASE}/lib/)
find_library(UHDM_LIB    NAMES uhdm    PATHS ${SURELOG_BASE}/third_party/UHDM/lib/)
find_library(CAPNP_LIB   NAMES capnp   PATHS ${SURELOG_BASE}/third_party/UHDM/third_party/capnproto/c++/src/capnp/)
find_library(KJ_LIB      NAMES kj      PATHS ${SURELOG_BASE}/third_party/UHDM/third_party/capnproto/c++/src/kj/)
find_library(ANTLR_LIB   NAMES antlr4-runtime PATHS ${SURELOG_BASE}/third_party/antlr4/runtime/Cpp/runtime/)
message (${SURELOG_LIB})
message (${UHDM_LIB})
message (${CAPNP_LIB})
message (${KJ_LIB})
message (${ANTLR_LIB})

target_link_libraries(bitblast-bin ${SURELOG_LIB})
target_link_libraries(bitblast-bin ${UHDM_LIB})
target_link_libraries(bitblast-bin ${ANTLR_LIB})
target_link_libraries(bitblast-bin ${CAPNP_LIB})
target_link_libraries(bitblast-bin ${KJ_LIB})
target_link_libraries(bitblast-bin z)

include(GNUInstallDirs)
