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
    set(py_fatch_url "https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/python3.8_static_zlib_6feb2023.tar.gz")
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
set(py_exe_expected_size 106924392)
set(DO_DOWNLOAD_PY 0)
set(DO_EXTRACTION_PY 0)
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
#check if Python tar already exist to prevent over download
if(EXISTS ${py_exe_zip})
    message(STATUS "Python downlaoded zip already exist.")
    file(SIZE ${py_exe_zip} EXE_Download_Size_py)
    if(${EXE_Download_Size_py} EQUAL ${py_exe_expected_size} )
        message(STATUS "Already present download Python zip will be used") 
    else()
        message(STATUS "Already present download Python zip is outdated so downloading again")
        set(DO_DOWNLOAD_PY 1)
        set(DO_EXTRACTION_PY 1)
    endif()
else()
    message(STATUS "Freash Downloading Python zip Binaries")
    set(DO_DOWNLOAD_PY 1)
    set(DO_EXTRACTION_PY 1)
endif()

if(${DO_DOWNLOAD_PY} EQUAL 1)
#    message(STATUS "Downloading Python Zip Binaries")
    file(DOWNLOAD ${py_fatch_url} ${py_exe_zip} SHOW_PROGRESS STATUS DOWNLOAD_STATUS)
    list (GET DOWNLOAD_STATUS 0 STATUS_CODE)
    if (NOT ${STATUS_CODE} EQUAL "0")
        message(FATAL_ERROR "Exit status of Python zip download is ${STATUS_CODE}")
    else()
        message(STATUS "Successfully Donwloaded Python zip)") 
    endif()
endif ()

#-------------------Unzip Python zip------------------------
if(NOT EXISTS ${work_dir}/python3.8/bin/python3.8)
    set(DO_EXTRACTION_PY 1)
endif()

if(${DO_EXTRACTION_PY} EQUAL 1)
    add_custom_target(unzip_python ALL
                 COMMENT      "      Extracting Python zip      "
                 DEPENDS ${py_exe_zip}
                 BYPRODUCTS ${work_dir}/python3.8
                 COMMAND ${CMAKE_COMMAND} -E tar xvzf ${py_exe_zip}
                 WORKING_DIRECTORY ${work_dir}
                 )
else ()
      message(STATUS "Extracted Python zip Content already exist and haven't download new zip so skipping it")
endif()

# requirements.txt file
set(req_file ${PROJECT_SOURCE_DIR}/requirements.txt)
set(expected_req_txt_md5 "4115eaaf76c3f3de0fbd18c959b31159")
#list of packages separated by comma
set(packages_for_venv "https://github.com/enjoy-digital/litex,https://github.com/m-labs/migen" )
# fetch URL for virtual env tar
set(env_fetch_url "https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/litex_feb62023.tar.gz")
# set the name of file and folder to put the download
set(litex_env_zip ${PROJECT_SOURCE_DIR}/litex_env.zip)
set(litex_expected_size 83784561)
set(DO_DOWNLOAD_LX 0)
set(DO_EXTRACTION_LX 0)
# calculate md5sum
file(MD5 ${req_file} CalculatedCheckSum)
set(PYTHON_EXE ${work_dir}/python3.8/bin/python3.8) # needed in case building from scratch
#compare the md5sum
if("${expected_req_txt_md5}"  STREQUAL  "${CalculatedCheckSum}") #Comapre checksums
    message(STATUS "Checksum is same. Proceeding with Pre-build Litex Env")
    #check if Litex tar already exist to prevent over download
    if(EXISTS ${litex_env_zip})
        message(STATUS "Litex downlaoded zip already exist.")
        file(SIZE ${litex_env_zip} EXE_Download_Size_lx)
        if(${EXE_Download_Size_lx} EQUAL ${litex_expected_size} )
            message(STATUS "Already present download Litex zip will be used") 
        else()
            message(STATUS "Already present download Litex zip is outdated so downloading again")
            set(DO_DOWNLOAD_LX 1)
            set(DO_EXTRACTION_LX 1)
        endif()
    else()
        message(STATUS "Fresh Downloading Litex zip Binaries")
        set(DO_DOWNLOAD_LX 1)
        set(DO_EXTRACTION_LX 1)
    endif()

    if(${DO_DOWNLOAD_LX} EQUAL 1)
        #message(STATUS "Downloading Litex Zip Binaries")
        file(DOWNLOAD ${env_fetch_url} ${litex_env_zip} SHOW_PROGRESS STATUS DOWNLOAD_STATUS)
        list (GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if (NOT ${STATUS_CODE} EQUAL "0")
            message(FATAL_ERROR "Exit status of Litex zip download is ${STATUS_CODE}")
        else()
            message(STATUS "Successfully Donwloaded Litex zip)") 
        endif()
    endif()

#-------------------Unzip Litex zip------------------------
    if(NOT EXISTS ${work_dir}/litex/bin/cocotb-config)
        set(DO_EXTRACTION_LX 1)
    endif()

    if(${DO_EXTRACTION_LX} EQUAL 1)
        add_custom_target(unzip_litex ALL
                 COMMENT      "      Extracting Litex zip      "
                 DEPENDS ${litex_env_zip}
                 BYPRODUCTS ${work_dir}/litex
                 COMMAND ${CMAKE_COMMAND} -E tar xvzf ${litex_env_zip}
                        WORKING_DIRECTORY ${work_dir}
                 )
    else ()
          message(STATUS "Extracted Litex zip Content already exist and haven't download new zip so skipping it")
    endif()
else() # uncomment it and generate , upload the tar again. requirement.txt is altered.
    message(STATUS "WORK DIR from cmake is ${work_dir}")
    add_custom_target(litex_from_scratch ALL
    COMMENT      "      Build Litex ENV from Scratch      "
    DEPENDS ${PYTHON_EXE}
    BYPRODUCTS ${work_dir}/litex
    COMMAND bash ${PROJECT_SOURCE_DIR}/gen_venv.sh -x ${PYTHON_EXE} -w ${work_dir} -s build -g ${packages_for_venv} -r ${req_file}
    )
endif ()

if(NOT EXISTS ${work_dir}/litex/bin/python3.8)
    add_custom_command(OUTPUT ${work_dir}/litex/bin/python3.8
                    COMMENT "   Create Symbolic Link    "
                    COMMAND ${CMAKE_COMMAND} -E create_symlink ../../python3.8/bin/python3.8 python3.8
                                    WORKING_DIRECTORY ${work_dir}/litex/bin
                    DEPENDS [unzip_python unzip_litex]
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


