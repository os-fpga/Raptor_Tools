cmake_minimum_required(VERSION 3.15)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if (${CMAKE_VERSION} VERSION_GREATER "3.8")
    #For cmake >= 3.9 INTERPROCEDURAL_OPTIMIZATION behaviour we need to explicitly
    #set the cmake policy version number
    cmake_policy(VERSION 3.9) 

    # If we are using verison < 3.9 then setting INTERPROCEDURAL_OPTIMIZATION
    # has no effect unless an Intel compiler is used
endif()

if(NOT CMAKE_BUILD_TYPE)
    message(WARNING "Build type not set, falling back to Release mode.
        To specify build type use:
        -DCMAKE_BUILD_TYPE=<mode> where <mode> is Debug or Release.")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build, options are: Debug Release."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)


project(HDL_Simulator)

message(${PROJECT_SOURCE_DIR})

set(FETCHCONTENT_QUIET FALSE)
set(Release_URL https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/hdl_simulator_v0.5.1.zip)
set(Destination_Dir ${PROJECT_SOURCE_DIR}/sim.tar.gz)
set(expected_size 244453774)
set(DO_DOWNLOAD 0)
set(DO_EXTRACTION 0)
#set(expected_content_size )
#set download dir in build or debuild directory
if (RAPTOR)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(Extraction_dir ${PROJECT_SOURCE_DIR}/../../dbuild/bin)
    else ()
    set(Extraction_dir ${PROJECT_SOURCE_DIR}/../../build/bin)
    endif()
else ()
    set(Extraction_dir ${PROJECT_SOURCE_DIR}/build/bin)
endif()
execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory ${Extraction_dir}
)

message(STATUS "Download DIR from cmake for HDL Sim is ${Destination_Dir}")
message(STATUS "Extraction DIR from cmake for HDL Sim is ${Extraction_dir}")

#check if file already exist to prevent over download
if(EXISTS ${Destination_Dir})
    message(STATUS "HDL Simulator downlaod zip already exist.")
    file(SIZE ${Destination_Dir} SIM_Download_Size)
    if(${SIM_Download_Size} EQUAL ${expected_size} )
        message(STATUS "Already present download HDL Sim zip will be used") 
    else()
        message(STATUS "Already present download HDL Sim zip is outdated so downloading again")
        set(DO_DOWNLOAD 1)
        set(DO_EXTRACTION 1)
    endif()
else()
    message(STATUS "Fresh Downloading HDL simulator Pre-build Binaries")
    set(DO_DOWNLOAD 1)
    set(DO_EXTRACTION 1)
endif()

if(${DO_DOWNLOAD} EQUAL 1)
#    message(STATUS "Downloading HDL simulator Pre-build Binaries")
    file(DOWNLOAD ${Release_URL} ${Destination_Dir} SHOW_PROGRESS STATUS DOWNLOAD_STATUS)
    list (GET DOWNLOAD_STATUS 0 STATUS_CODE)
    if (NOT ${STATUS_CODE} EQUAL "0")
        message(FATAL_ERROR "Exit status of download HDL Sim is ${STATUS_CODE}")
    else()
        message(STATUS "Successfully Donwloadeded Pre-built HDL Sim)") 
    endif()
endif ()

#-------------------Unzip------------------------
if((NOT EXISTS ${Extraction_dir}/HDL_simulator/setup_sim) OR (NOT EXISTS ${Extraction_dir}/HDL_simulator/GHDL) OR (NOT EXISTS ${Extraction_dir}/HDL_simulator/iverilog) OR (NOT EXISTS ${Extraction_dir}/HDL_simulator/systemc_2.3.3) OR (NOT EXISTS ${Extraction_dir}/HDL_simulator/verilator))
    set(DO_EXTRACTION 1)
endif()

if(${DO_EXTRACTION} EQUAL 1)
 add_custom_target(unzip_sim ALL
                 COMMENT      "      Extracting HDL Simulator        "
                 DEPENDS ${Destination_Dir}
                 BYPRODUCTS ${Extraction_dir}/HDL_simulator
                 COMMAND ${CMAKE_COMMAND} -E tar xvzf ${Destination_Dir}
                 WORKING_DIRECTORY ${Extraction_dir}
                 )
 else ()
      message(STATUS "Extracted HDL Sim Content exist and haven't download new zip so skipping it")
 endif()

#install(
#        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/HDL_simulator DESTINATION  ${CMAKE_INSTALL_PREFIX}
#USE_SOURCE_PERMISSIONS	
#)
