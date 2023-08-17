if(RAPTOR)
set(unzip_boost_dir ${CMAKE_CURRENT_BINARY_DIR}/Raptor_Tools/graph_partition/mt-kahypar/external_tools/boost)
else()
set(unzip_boost_dir ${CMAKE_CURRENT_BINARY_DIR}/mt-kahypar/external_tools/boost)
endif()
include(FetchContent)
FetchContent_Populate(
  boost-src
  URL https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/boost_1_69_0_minimum.tar.bz2
  #URL_HASH MD5=a1332494397bf48332cb152abfefcec2
  SOURCE_DIR ${unzip_boost_dir}
)
