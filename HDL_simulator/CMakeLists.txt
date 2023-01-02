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
set(Release_URL https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/hdl_simulator_v0.2.zip)
set(Destination_Dir ${PROJECT_SOURCE_DIR}/sim.tar.gz)
set(expected_size 237991184)
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

message(STATUS "Download DIR from cmake is ${Destination_Dir}")
message(STATUS "Extraction DIR from cmake is ${Extraction_dir}")

#check if file already exist to prevent over download
if(EXISTS ${Destination_Dir})
    message(STATUS "HDL Simulator downlaod zip already exist. Skipping its downlaod")
else()
    #download and print status
    message(STATUS "Downloading HDL simulator Pre-build Binaries")
    file(DOWNLOAD ${Release_URL} ${Destination_Dir} SHOW_PROGRESS)
    file(SIZE ${Destination_Dir} SIM_Download_Size)

    if(${SIM_Download_Size} EQUAL ${expected_size} )
        message(STATUS "Successfully Donwloadeded Pre-built verilator with expected size (${SIM_Download_Size})")
        #file(ARCHIVE_EXTRACT INPUT ${Destination_Dir} DESTINATION ${Extraction_dir} VERBOSE ) # this need cmake version >= 3.18. 
    else()
        message(STATUS "ERROR: Fail to Downloaded pre-built verilator. Download size (${SIM_Download_Size}) does not match to 205951449")
    endif()
endif ()

file(SIZE ${Destination_Dir} SIM_Download_Size)
if(${SIM_Download_Size} EQUAL ${expected_size} )
    message(STATUS "Pre-built verilator downloaded is present with expected size (${SIM_Download_Size})")
    #file(ARCHIVE_EXTRACT INPUT ${Destination_Dir} DESTINATION ${Extraction_dir} VERBOSE ) # this need cmake version >= 3.18. 
    if(NOT EXISTS ${Extraction_dir}/HDL_simulator)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xvzf ${Destination_Dir}
        WORKING_DIRECTORY ${Extraction_dir}
    )
    endif()
else()
        message(STATUS "ERROR: Fail to Extract pre-built verilator. Size is (${SIM_Download_Size}) that is different from expected size")
endif()



# uncomment to debug the install target in this cmake
#install(
#        DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin/HDL_simulator DESTINATION  ${CMAKE_INSTALL_PREFIX}
#USE_SOURCE_PERMISSIONS	
#)

