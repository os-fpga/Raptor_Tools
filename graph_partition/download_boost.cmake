if($ENV{BUILD_TYPE} STREQUAL "Release")
    set(build_dir build)
else()
    set(build_dir dbuild)
endif()

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/Raptor_Tools)
set(unzip_boost_dir ${CMAKE_CURRENT_BINARY_DIR}/${build_dir}/Raptor_Tools/graph_partition/mt-kahypar/external_tools/boost)
elseif(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../graph_partition)
set(unzip_boost_dir ${CMAKE_CURRENT_BINARY_DIR}/${build_dir}/mt-kahypar/external_tools/boost)
else()
set(unzip_boost_dir ${CMAKE_CURRENT_BINARY_DIR}/opt)
endif()
include(FetchContent)
FetchContent_Populate(
  boost-src
  URL https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/boost_1_69_0_minimum.tar.bz2
  #URL_HASH MD5=a1332494397bf48332cb152abfefcec2
  SOURCE_DIR ${unzip_boost_dir}
  SUBBUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/${build_dir}/boost_download
)
#file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/boost-src-subbuild)
#file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/boost-src-build)
