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
set(Release_URL https://github.com/RapidSilicon/pre_build_artifacts/archive/refs/tags/v1.2.tar.gz)

#set download dir in build or debuild directory
if (RAPTOR)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(Destination_Dir ${PROJECT_SOURCE_DIR}/../../dbuild/sim.tar.gz)
    else ()
    set(Destination_Dir ${PROJECT_SOURCE_DIR}/../../build/sim.tar.gz)
    endif()
else ()
    set(Destination_Dir ${PROJECT_SOURCE_DIR}/build/sim.tar.gz)
endif()
message(STATUS "Download DIR from cmake is ${Destination_Dir}")

set (Extraction_dir ${PROJECT_SOURCE_DIR} )
message(STATUS "Extraction DIR from cmake is ${Extraction_dir}}")


#check if file already exist to prevent over download
if(EXISTS ${Destination_Dir})
    message(STATUS "HDL Simulator downlaod tar already exist. Skipping its downlaod")
elseif(EXISTS ${Extraction_dir}/pre_build_artifacts-1.2)
    message(STATUS "HDL Simulator Extracted tar already exist. Skipping it")
else()
    #download and record status
    message(STATUS "Downloading HDL simulator Pre-build Binaries")
    file(DOWNLOAD ${Release_URL} ${Destination_Dir} SHOW_PROGRESS)
    file(SIZE ${Destination_Dir} SIM_Download_Size)


    if(${SIM_Download_Size} EQUAL 204231293 )
        message(STATUS "Pre-built verilator downloaded with expected size (${SIM_Download_Size})")
        #file(ARCHIVE_EXTRACT INPUT ${Destination_Dir} DESTINATION ${Extraction_dir} VERBOSE ) # this need cmake version >= 3.18. 
        execute_process(
        COMMAND ${CMAKE_COMMAND} -E tar xvzf ${Destination_Dir}
        WORKING_DIRECTORY ${Extraction_dir}
        )
    else()
        message(STATUS "ERROR: Fail to Downloaded pre-built verilator.Expected size is (${SIM_Download_Size})")
    endif()
endif ()

##execute the script
#if(EXISTS ${work_dir}/share/envs/litex/.venv)
#    message(STATUS ".venv file exist, so not creating virtual environment again")
#else ()
#message(STATUS "Litex virtual environment installation started")
#
#execute_process(COMMAND bash ${PROJECT_SOURCE_DIR}/gen_venv.sh -w ${work_dir} -s build -g ${packages_for_venv} -r ${req_file}) 
#message(STATUS "Litex virtual environment installation completed")
#endif()                
#
##execute script on install time
#install( CODE 
#                "
#message(STATUS \"${CMAKE_INSTALL_PREFIX} Ruuning at installation time\")
#execute_process(COMMAND  bash ${PROJECT_SOURCE_DIR}/gen_venv.sh -i ${CMAKE_INSTALL_PREFIX} -w ${work_dir} -s install)
#        "
#)

