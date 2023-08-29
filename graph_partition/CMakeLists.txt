cmake_minimum_required(VERSION 3.15)

project("graph_partition")

set(ShoulbeBoostMDsum 18e91cd48013ac7b5f670cb5c9d215f9)
set(RSShoulbeBoostMDsum 1b62dd4ffd4a46aa5f71ae8fa8611293)
file(MD5  ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts/download_boost.cmake presentBoostMDsum)
set(Boost_download_RS ${CMAKE_CURRENT_SOURCE_DIR}/download_boost.cmake)

set(shouldbeTBBMd5sum 44b642420d30c5cec546a8db802d19a6)
set(RSshouldbeTBBMd5sum 695cb3b93854b2e090b78c0fa312d5d4)
file(MD5  ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts/download_tbb_linux.cmake presentTBBMd5sum)
set(TBB_download_RS ${CMAKE_CURRENT_SOURCE_DIR}/download_tbb_linux.cmake)

# Set mt-kahypar options ON or OFF
set(KAHYPAR_DOWNLOAD_BOOST ON CACHE BOOL "Enable the Download of Boost" FORCE)
set(KAHYPAR_DOWNLOAD_TBB ON CACHE BOOL "Enable the Download of TBB" FORCE)


FILE(COPY ${Boost_download_RS}
          DESTINATION
          ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts)

FILE(COPY ${TBB_download_RS}
          DESTINATION
          ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts)


message(STATUS "+++++++++++++++ CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "+++++++++++++++ CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")

if(RAPTOR)
    # need to move the findtbb.cmake to OpenFPGA as well so both look for tbb using same regex
    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/cmake/modules/FindTBB.cmake
              DESTINATION
                ${CMAKE_CURRENT_SOURCE_DIR}/../../Backend/OpenFPGA/cmake/modules)

    # need to insert a line in vpr/tatum so that it can know the tbb header files location and link libs, do same for vpr cmake as well
    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libtatutum.cmake
            DESTINATION
            ${CMAKE_CURRENT_SOURCE_DIR}/../../Backend/OpenFPGA/vtr-verilog-to-routing/libs/EXTERNAL/libtatum/libtatum)
    FILE(RENAME ${CMAKE_CURRENT_SOURCE_DIR}/../../Backend/OpenFPGA/vtr-verilog-to-routing/libs/EXTERNAL/libtatum/libtatum/libtatutum.cmake
             ${CMAKE_CURRENT_SOURCE_DIR}/../../Backend/OpenFPGA/vtr-verilog-to-routing/libs/EXTERNAL/libtatum/libtatum/CMakeLists.txt)

    # turn off googletest in mt-kahypar as FOEDAG will provide it
    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/CMakeLists.txt FILE_CONTENT)
    if(NOT "${FILE_CONTENT}" MATCHES "#add_subdirectory(external_tools/googletest EXCLUDE_FROM_ALL)")
        # Comment out the line
        string(REPLACE "add_subdirectory(external_tools/googletest EXCLUDE_FROM_ALL)" "#add_subdirectory(external_tools/googletest EXCLUDE_FROM_ALL)" MODIFIED_CONTENT "${FILE_CONTENT}")
        file(WRITE ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/CMakeLists.txt "${MODIFIED_CONTENT}")
    endif()
elseif(BACKEND_RS)
    message(STATUS "==================== BACKEND_RS")
    # need to move the findtbb.cmake to OpenFPGA as well so both look for tbb using same regex
    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/cmake/modules/FindTBB.cmake
            DESTINATION
                ${CMAKE_CURRENT_SOURCE_DIR}/../../OpenFPGA/cmake/modules)

    # need to insert a line in vpr/tatum so that it can know the tbb header files location and link libs, do same for vpr cmake as well
    FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/libtatutum.cmake
            DESTINATION
            ${CMAKE_CURRENT_SOURCE_DIR}/../../OpenFPGA/vtr-verilog-to-routing/libs/EXTERNAL/libtatum/libtatum)
    FILE(RENAME ${CMAKE_CURRENT_SOURCE_DIR}/../../OpenFPGA/vtr-verilog-to-routing/libs/EXTERNAL/libtatum/libtatum/libtatutum.cmake
            ${CMAKE_CURRENT_SOURCE_DIR}/../../OpenFPGA/vtr-verilog-to-routing/libs/EXTERNAL/libtatum/libtatum/CMakeLists.txt)
endif()

set(ENV{BUILD_TYPE} ${CMAKE_BUILD_TYPE})
if(RAPTOR)
    set(ENV{RAPTOR} on)
    set(ENV{BACKEND_RS} off)
elseif(BACKEND_RS)
    set(ENV{BACKEND_RS} on)
    set(ENV{RAPTOR} off)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/../libs/external/boost)
# Sub-project
add_subdirectory(mt-kahypar ${CMAKE_CURRENT_BINARY_DIR}/mt-kahypar EXCLUDE_FROM_ALL)

# build target of interest only
add_custom_target(build_mtkahypar ALL
    COMMENT "Building and moving MtKaHyPar"
)

add_dependencies(build_mtkahypar MtKaHyPar)

install(
  TARGETS MtKaHyPar
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install (
    DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/mt-kahypar/external_tools/tbb/lib/intel64/gcc4.8/
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/tbb)


if(RAPTOR)
    # Set MtKaHyPar_BINARY_FILE_PATH
    set(MtKaHyPar_BINARY_FILE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MtKaHyPar" CACHE INTERNAL "")
    configure_file(config.h.in "${CMAKE_CURRENT_SOURCE_DIR}/../../Backend/OpenFPGA/vtr-verilog-to-routing/vpr/src/pack/config.h")
elseif(BACKEND_RS)
    set(MtKaHyPar_BINARY_FILE_PATH "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/MtKaHyPar" CACHE INTERNAL "")
    configure_file(config.h.in "${CMAKE_CURRENT_SOURCE_DIR}/../../OpenFPGA/vtr-verilog-to-routing/vpr/src/pack/config.h")
endif()
