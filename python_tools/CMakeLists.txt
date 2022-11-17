cmake_minimum_required(VERSION 3.15)

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


project(Litex)

message(${PROJECT_SOURCE_DIR})
# set download fetch URL as per OS
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(py_fatch_url "https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/python3.8_static_lib.tar.gz")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(py_fatch_url "https://www.python.org/ftp/python/3.8.7/python-3.8.7-embed-amd64.zip")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(py_fatch_url "https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/python3.8_static_lib.tar.gz")
else()
    message(SEND_ERROR "No python fetch URL is available for this OS")    
endif()
# set the name of file and folder to put the download
set(py_exe_zip ${PROJECT_SOURCE_DIR}/python_3.zip)
# set the expected size
set(py_exe_expected_size 108116381)
#set build or debuild directory
if (RAPTOR)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(work_dir ${PROJECT_SOURCE_DIR}/../../dbuild/share/envs)
    else ()
    set(work_dir ${PROJECT_SOURCE_DIR}/../../build/share/envs)
    endif()
else ()
    set(work_dir ${PROJECT_SOURCE_DIR}/build/share/envs)
endif()
# create directory in advance
execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory ${work_dir}
)
#check if file already exist to prevent over download
if(EXISTS ${py_exe_zip})
    message(STATUS "Python 3.8.7 downlaoded zip already exist. Skipping its downlaod")
else()
    #download and record status
    message(STATUS "Downloading Python 3.8.7 Pre-build Binaries")
    file(DOWNLOAD ${py_fatch_url} ${py_exe_zip} SHOW_PROGRESS)
endif ()

file(SIZE ${py_exe_zip} EXE_Download_Size)
if(${EXE_Download_Size} EQUAL ${py_exe_expected_size})
    message(STATUS "Pre-Compile 3.8.7 downloaded with expected size (${py_exe_expected_size})")
    if(NOT EXISTS ${work_dir}/python3.8)
        execute_process(
            #file(ARCHIVE_EXTRACT INPUT ${Destination_Dir} DESTINATION ${Extraction_dir} VERBOSE ) # this need cmake version >= 3.18. 
            COMMAND ${CMAKE_COMMAND} -E tar xvzf ${py_exe_zip}
                    WORKING_DIRECTORY ${work_dir}
        )
    endif()
else()
    message(STATUS "ERROR: Fail to Downloaded pre-compile python exe.Expected size is (${py_exe_expected_size}) but got (${EXE_Download_Size})")
endif()

# requirements.txt file
set(req_file ${PROJECT_SOURCE_DIR}/requirements.txt)
set(expected_req_txt_md5 "dd73fe7d483fe7f1c350424da049299e")
#list of packages separated by comma
set(packages_for_venv "https://github.com/enjoy-digital/litex,https://github.com/m-labs/migen" )
# fetch URL for virtual env tar
set(env_fetch_url "https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/litex_packages.tar.gz")
# set the name of file and folder to put the download
set(litex_env_zip ${PROJECT_SOURCE_DIR}/litex_env.zip)
set(litex_expected_size 78705833)
# calculate md5sum
file(MD5 ${req_file} CalculatedCheckSum)
#compare the md5sum
if("${expected_req_txt_md5}"  STREQUAL  "${CalculatedCheckSum}") #Comapre checksums
    message(STATUS "Checksum is same")
    #check if file already exist to prevent over download
    if(EXISTS ${litex_env_zip})
        message(STATUS "Litex env downlaoded zip already exist. Skipping its downlaod")
    else()
        #download and record status
        message(STATUS "Downloading Litex")
        file(DOWNLOAD ${env_fetch_url} ${litex_env_zip} SHOW_PROGRESS)
    endif()
    # extract the downloaded tar
    file(SIZE ${litex_env_zip} ENV_Download_Size)
    # compare size
    if(${ENV_Download_Size} EQUAL ${litex_expected_size})
        message(STATUS "litex env downloaded with expected size (${litex_expected_size})")
        if(NOT EXISTS ${work_dir}/litex)
            execute_process(
                COMMAND ${CMAKE_COMMAND} -E tar xvzf ${litex_env_zip}
                        WORKING_DIRECTORY ${work_dir}
                )
        endif()
    else()
        message(STATUS "ERROR: Fail to Downloaded Litex env.Expected size is (${litex_expected_size}) but got (${ENV_Download_Size})")
    endif()
#else() # uncomment it and generate , upload the tar again. requirement.txt is altered.
#    set(PYTHON_EXE ${PROJECT_SOURCE_DIR}/python3.8/bin/python3.8)
#    message(STATUS "WORK DIR from cmake is ${work_dir}")
#    #execute the script
#    if(EXISTS ${work_dir}/share/envs/litex/.venv)
#        message(STATUS ".venv file exist, so not creating virtual environment again")
#    else ()
#    message(STATUS "Litex virtual environment installation started")
#
#    execute_process(COMMAND bash ${PROJECT_SOURCE_DIR}/gen_venv.sh -x ${PYTHON_EXE} -w ${work_dir} -s build -g ${packages_for_venv} -r ${req_file}) 
#    message(STATUS "Litex virtual environment installation completed")
#    endif()                
endif ()

if(NOT EXISTS ${work_dir}/litex/bin/python3.8)
    message(STATUS "putting python 3.8 in virtual env")
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink ../../python3.8/bin/python3.8 python3.8
             WORKING_DIRECTORY ${work_dir}/litex/bin
    )
endif()

if(NOT RAPTOR)
install(
      DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/envs DESTINATION  ${CMAKE_INSTALL_PREFIX}
      USE_SOURCE_PERMISSIONS
)
endif()

#execute script on install time
#install( CODE 
#                "
#message(STATUS \"${CMAKE_INSTALL_PREFIX} Ruuning at installation time\")
#execute_process(COMMAND  bash ${PROJECT_SOURCE_DIR}/gen_venv.sh -i ${CMAKE_INSTALL_PREFIX} -w ${work_dir} -s install)
#        "
#)


