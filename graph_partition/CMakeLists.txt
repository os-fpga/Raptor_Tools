cmake_minimum_required(VERSION 3.15)

project("graph_partition")

set(ShoulbeBoostMDsum 18e91cd48013ac7b5f670cb5c9d215f9)
set(RSShoulbeBoostMDsum f34c5c7b7467c2613629ce4f3b1f691b)
file(MD5  ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts/download_boost.cmake presentBoostMDsum)
set(Boost_download_RS ${CMAKE_CURRENT_SOURCE_DIR}/download_boost.cmake)

set(shouldbeTBBMd5sum 44b642420d30c5cec546a8db802d19a6)
set(RSshouldbeTBBMd5sum dee20b877c387f1550825107f2cc3875)
file(MD5  ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts/download_tbb_linux.cmake presentTBBMd5sum)
set(TBB_download_RS ${CMAKE_CURRENT_SOURCE_DIR}/download_tbb_linux.cmake)

# Set mt-kahypar options ON or OFF
set(KAHYPAR_DOWNLOAD_BOOST ON CACHE BOOL "Enable the Download of Boost" FORCE)
set(KAHYPAR_DOWNLOAD_TBB ON CACHE BOOL "Enable the Download of TBB" FORCE)

if(${ShoulbeBoostMDsum} STREQUAL ${presentBoostMDsum} OR ${RSShoulbeBoostMDsum} STREQUAL ${presentBoostMDsum})
FILE(COPY ${Boost_download_RS}
          DESTINATION
          ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts)
else()
    message(FATAL_ERROR "The CMake to download boost is changed. Please look into it" )
endif()

if(${shouldbeTBBMd5sum} STREQUAL ${presentTBBMd5sum} OR ${RSshouldbeTBBMd5sum}  STREQUAL ${presentTBBMd5sum})
FILE(COPY ${TBB_download_RS}
          DESTINATION
          ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts)
else()
    message(FATAL_ERROR "The CMake to download TBB is changed. Please look into it" )
endif()

set(ENV{BUILD_TYPE} ${CMAKE_BUILD_TYPE})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Sub-project
add_subdirectory(mt-kahypar ${CMAKE_CURRENT_BINARY_DIR}/mt-kahypar EXCLUDE_FROM_ALL)
add_custom_target(build_mtkahypar ALL
    COMMENT "Building and moving MtKaHyPar"
)

add_dependencies(build_mtkahypar MtKaHyPar)




