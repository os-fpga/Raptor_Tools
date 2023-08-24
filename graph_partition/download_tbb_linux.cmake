if($ENV{BUILD_TYPE} STREQUAL "Release")
    set(build_dir build)
else()
    set(build_dir dbuild)
endif()

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/Raptor_Tools)
set(unzip_tbb_dir ${CMAKE_CURRENT_BINARY_DIR}/${build_dir}/Raptor_Tools/graph_partition/mt-kahypar/external_tools/tbb)
elseif(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/../graph_partition)
set(unzip_tbb_dir ${CMAKE_CURRENT_BINARY_DIR}/${build_dir}/mt-kahypar/external_tools/tbb)
else()
set(unzip_tbb_dir ${CMAKE_CURRENT_BINARY_DIR}/opt)
endif()
include(FetchContent)
FetchContent_Populate(
  tbb
  URL https://github.com/oneapi-src/oneTBB/releases/download/v2021.7.0/oneapi-tbb-2021.7.0-lin.tgz
  URL_HASH SHA256=3c2b3287c595e2bb833c025fcd271783963b7dfae8dc681440ea6afe5d550e6a
  SOURCE_DIR ${unzip_tbb_dir}
  SUBBUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/${build_dir}/tbb_download
)
#file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/tbb-build)
#file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/tbb-subbuild)

