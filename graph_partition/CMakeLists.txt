cmake_minimum_required(VERSION 3.15)

project("graph_partition")

set(ShoulbeBoostMDsum 18e91cd48013ac7b5f670cb5c9d215f9)
file(MD5  ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts/download_boost.cmake presentBoostMDsum)
set(Boost_download_RS ${CMAKE_CURRENT_SOURCE_DIR}/download_boost.cmake)
#message (STATUS "========== path of boost is ${Boost_download_RS}")
set(shouldbeTBBMd5sum 1cc32d1971ec2958f7acd59c8270800e)
file(MD5  ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts/download_tbb_linux.cmake presentTBBMd5sum)
set(TBB_download_RS ${CMAKE_CURRENT_SOURCE_DIR}/download_tbb_linux.cmake)
# 
# Set mt-kahypar options ON or OFF
#
set(KAHYPAR_DOWNLOAD_BOOST ON CACHE BOOL "Enable the Download of Boost" FORCE)
set(KAHYPAR_DOWNLOAD_TBB ON CACHE BOOL "Enable the Download of TBB" FORCE)


if(${ShoulbeBoostMDsum} STREQUAL ${presentBoostMDsum})
FILE(COPY ${Boost_download_RS}
          DESTINATION
          ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts)
else()
    message(FATAL_ERROR "The CMake to download boost is changed. Please look into it" )
endif()

if(${shouldbeTBBMd5sum} STREQUAL ${presentTBBMd5sum})
FILE(COPY ${TBB_download_RS}
          DESTINATION
          ${CMAKE_CURRENT_SOURCE_DIR}/mt-kahypar/scripts)
else()
    message(FATAL_ERROR "The CMake to download TBB is changed. Please look into it" )
endif()
# Sub-project
add_subdirectory(mt-kahypar)
add_custom_target(copyMtkahypar ALL
COMMENT      "      Moving Mtkahpar to bin      "
COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/mt-kahypar/mt-kahypar/application/MtKaHyPar ${CMAKE_CURRENT_BINARY_DIR}/bin
)


