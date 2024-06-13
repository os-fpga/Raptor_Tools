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

message(STATUS "Python Virtual ENV: ${PROJECT_SOURCE_DIR}")
# set download fetch URL as per OS
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(py_fatch_url "https://github.com/os-fpga/post_build_artifacts/releases/download/v0.1/python3.8_static_zlib_8march_2023.tar.gz")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    set(py_fatch_url "https://www.python.org/ftp/python/3.8.7/python-3.8.7-embed-amd64.zip")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(py_fatch_url "https://github.com/os-fpga/post_build_artifacts/releases/download/v0.1/python3.8_static_lib.tar.gz")
else()
    message(SEND_ERROR "No python fetch URL is available for this OS")    
endif()
# set the name of file and folder to put the download
set(py_exe_zip ${PROJECT_SOURCE_DIR}/python_3.zip)
# set the expected size
set(py_exe_expected_size 123699788)
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
# detect Linux Distro
execute_process (
    COMMAND bash -c "awk -F= '/^ID=/{print $2}' /etc/os-release |tr -d '\n' | tr -d '\"'"
    OUTPUT_VARIABLE LINUX_DISTRO
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
                 COMMAND ${CMAKE_COMMAND} -E tar xzf ${py_exe_zip}
                 WORKING_DIRECTORY ${work_dir}
                 )
else ()
      message(STATUS "Extracted Python zip Content already exist and haven't download new zip so skipping it")
endif()

# requirements.txt file
set(req_file ${PROJECT_SOURCE_DIR}/requirements.txt)
set(expected_req_txt_md5 "4115eaaf76c3f3de0fbd18c959b31159")
# calculated md5sum of requirements.txt
file(MD5 ${req_file} CalculatedCheckSum)
#list of packages separated by comma
set(packages_for_venv "https://github.com/enjoy-digital/litex,https://github.com/m-labs/migen" )
# fetch URL for virtual env tar
set(env_fetch_url "https://github.com/os-fpga/post_build_artifacts/releases/download/v0.2/litex_25April_2024.tar.gz")
# set the name of file and folder to put the download
set(litex_env_zip ${PROJECT_SOURCE_DIR}/litex_env.zip)
set(litex_expected_size 83783935)
set(DO_DOWNLOAD_LX 0)
set(DO_EXTRACTION_LX 0)
#------------------------------------------------------------------------------------
#                   Unzip Litex zip and create symbolic link                        |
#------------------------------------------------------------------------------------
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

    if(NOT EXISTS ${work_dir}/litex/bin/cocotb-config)
        set(DO_EXTRACTION_LX 1)
    endif()

    if(${DO_EXTRACTION_LX} EQUAL 1)
        add_custom_target(unzip_litex ALL
                 COMMENT      "      Extracting Litex zip      "
                 DEPENDS ${litex_env_zip}
                 BYPRODUCTS ${work_dir}/litex
                 COMMAND ${CMAKE_COMMAND} -E tar xzf ${litex_env_zip}
                        WORKING_DIRECTORY ${work_dir}
                 )
        add_custom_command(TARGET unzip_litex POST_BUILD
                 COMMENT "   Create Symbolic Link    "
                 COMMAND ${CMAKE_COMMAND} -E create_symlink ../../python3.8/bin/python3.8 python3.8
                                 WORKING_DIRECTORY ${work_dir}/litex/bin
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

# --------------------------------------------------------------------------------- 
#                   ChatGPT VENV                                                   |
#----------------------------------------------------------------------------------

if(ENABLE_CHATGPT)
# fetch URL for pre build ChatGPT virtual env tar
set(chatgpt_env_fetch_url "https://github.com/os-fpga/post_build_artifacts/releases/download/v0.1/chatGPT_8april_2023.tar.gz")
# set the name of file and folder to put the download
set(chatgpt_env_zip ${PROJECT_SOURCE_DIR}/chatgpt_env.zip)
set(chatgpt_expected_size 23896907)
set(DO_DOWNLOAD_CGT 0)
set(DO_EXTRACTION_CGT 0)
# path of python exe, (commenting as it is already set)
#set(PYTHON_EXE ${work_dir}/python3.8/bin/python3.8) # needed in case building from scratch
# chatgpt-integration github repo path,
if (NOT CGT_GITHUB_REPO_PATH)
  set(CGT_GITHUB_REPO_PATH "Empty")
endif()
set(fresh_cgt_clone 0) 

if(${CGT_GITHUB_REPO_PATH}  STREQUAL  "Empty") 
    message(STATUS "Used Pre-Build ChatGPT VENV")
    #check if ChatGPT tar already exist to prevent over download
    if(EXISTS ${chatgpt_env_zip})
        message(STATUS "ChatGPT downlaoded zip already exist.")
        file(SIZE ${chatgpt_env_zip} EXE_Download_Size_cgt)
        if(${EXE_Download_Size_cgt} EQUAL ${chatgpt_expected_size} )
            message(STATUS "Already present download ChatGPT zip will be used") 
        else()
            message(STATUS "Already present download ChatGPT zip is outdated so downloading again")
            set(DO_DOWNLOAD_CGT 1)
            set(DO_EXTRACTION_CGT 1)
        endif()
    else()
        message(STATUS "Fresh Downloading ChatGPT zip Binaries")
        set(DO_DOWNLOAD_CGT 1)
        set(DO_EXTRACTION_CGT 1)
    endif()

    if(${DO_DOWNLOAD_CGT} EQUAL 1)
        #message(STATUS "Downloading ChatGPT Zip Binaries")
        file(DOWNLOAD ${chatgpt_env_fetch_url} ${chatgpt_env_zip} SHOW_PROGRESS STATUS DOWNLOAD_STATUS)
        list (GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if (NOT ${STATUS_CODE} EQUAL "0")
            message(FATAL_ERROR "Exit status of ChatGPT zip download is ${STATUS_CODE}")
        else()
            message(STATUS "Successfully Donwloaded ChatGPT zip)") 
        endif()
    endif()

#-------------------Unzip ChatGPT zip------------------------
    if(NOT EXISTS ${work_dir}/chatGPT/bin/openai)
    set(DO_EXTRACTION_CGT 1)
    endif()

    if(${DO_EXTRACTION_CGT} EQUAL 1)
    add_custom_target(unzip_chatgpt ALL
        COMMENT      "      Extracting ChatGPT zip      "
        DEPENDS ${chatgpt_env_zip}
        BYPRODUCTS ${work_dir}/chatGPT
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${chatgpt_env_zip}
                    WORKING_DIRECTORY ${work_dir}
    )
    add_custom_command(TARGET unzip_chatgpt POST_BUILD
        COMMENT "   Create Symbolic Link    "
        COMMAND ${CMAKE_COMMAND} -E create_symlink ../../python3.8/bin/python3.8 python3.8
        WORKING_DIRECTORY ${work_dir}/chatGPT/bin
    )
    else ()
      message(STATUS "Extracted ChatGPT zip Content already exist and haven't download new zip so skipping it")
    endif()
# update/create the ChatGPT venv from git repo.
else()
    # pyexe needed in case building from scratch
    # On ubuntu look for system python, on CentOS , go with own compiled one.
    if(${LINUX_DISTRO} STREQUAL "ubuntu")
        find_package(
            Python3 3.8 EXACT
            REQUIRED
            COMPONENTS Interpreter
        )
        execute_process(
            COMMAND pip show pipenv
            RESULT_VARIABLE EXIT_CODE
            OUTPUT_QUIET
        )
        if (NOT ${EXIT_CODE} EQUAL 0)
            message(FATAL_ERROR "The Python3.8 package pipenv is not installed. Please install it using the following command: python3.8 -m pip install pipenv.")
        endif()
        set(PYTHON_EXE python3.8)
        set(PYTHON_EXE_VERSION 3.8)
    else()
        set(PYTHON_EXE ${work_dir}/python3.8/bin/python3.8)
        set(PYTHON_EXE_VERSION ${work_dir}/python3.8/bin/python3.8)
    endif()
    if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/share/envs/chatGPT/bin/python)
        message(FATAL_ERROR "Cannot update/create upon existing virtual environment. Kindly clean the build folder and run again")
    else()
        message(STATUS "Updating the ChatGPT package from GitHub Repo")
        if (NOT EXISTS ${CGT_GITHUB_REPO_PATH}/setup.py)
            set(CGT_GITHUB_REPO_PATH ${CMAKE_CURRENT_BINARY_DIR}/chatgpt-integration)
            message(STATUS "Doing Fresh Clone at ${CGT_GITHUB_REPO_PATH}")
            execute_process(
                COMMAND git clone git@github.com:os-fpga/chatgpt-integration.git
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                RESULT_VARIABLE GIT_RESULT
                )
              if  (NOT GIT_RESULT EQUAL "0")
                message(FATAL_ERROR "Fail to clone git@github.com:os-fpga/chatgpt-integration.git")
              endif()
        endif()

        add_custom_command(TARGET unzip_python POST_BUILD
            COMMENT "   Make Directory of ChatGPT    "
            COMMAND ${CMAKE_COMMAND} -E make_directory ${work_dir}/chatGPT
        )    

        add_custom_command(TARGET unzip_python POST_BUILD
            COMMENT "   Create VENV of ChatGPT    "
            COMMAND ${CMAKE_COMMAND} -E touch ${work_dir}/chatGPT/.venv
            COMMAND ${CMAKE_COMMAND} -E echo "${work_dir}/chatGPT" > "${work_dir}/chatGPT/.venv"
            COMMAND ${PYTHON_EXE} -m pipenv install --python ${PYTHON_EXE_VERSION}  --no-site-packages
            COMMAND ${work_dir}/chatGPT/bin/python3 -m pip install pipenv wheel
            WORKING_DIRECTORY ${work_dir}/chatGPT
        )

        add_custom_command(TARGET unzip_python POST_BUILD
            COMMENT "   Create Wheel of ChatGPT and Install it    "
            COMMAND ${work_dir}/chatGPT/bin/python3 setup.py bdist_wheel
            COMMAND ${work_dir}/chatGPT/bin/python3 -m pip install dist/*.whl 
            WORKING_DIRECTORY ${CGT_GITHUB_REPO_PATH}
        )

        add_custom_command(TARGET unzip_python POST_BUILD
            COMMENT "   Create Symbolic Link    "
            COMMAND ${CMAKE_COMMAND} -E create_symlink ../../python3.8/bin/python3.8 python3.8
            COMMAND ${CMAKE_COMMAND} -E remove -f  ${work_dir}/chatGPT/bin/python
            COMMAND ${CMAKE_COMMAND} -E create_symlink python3 python
            WORKING_DIRECTORY ${work_dir}/chatGPT/bin
        )

        add_custom_command(TARGET unzip_python POST_BUILD
            COMMENT "   Copy Python Bash to ChatGTP bin    "
            COMMAND ${CMAKE_COMMAND} -E remove -f  ${work_dir}/chatGPT/bin/python3
            COMMAND ${CMAKE_COMMAND} -E copy ../../python3.8/bin/python3 python3
            COMMAND ${CMAKE_COMMAND} -E make_directory ${work_dir}/chatGPT/lib/os_libs
            COMMAND ${CMAKE_COMMAND} -E copy ../../python3.8/lib/os_libs/libssl.so.1.0.2k ${work_dir}/chatGPT/lib/os_libs
            COMMAND ${CMAKE_COMMAND} -E copy ../../python3.8/lib/os_libs/libffi.so.6.0.1 ${work_dir}/chatGPT/lib/os_libs
            COMMAND ${CMAKE_COMMAND} -E copy ../../python3.8/lib/os_libs/libcrypto.so.1.0.2k ${work_dir}/chatGPT/lib/os_libs
            DEPENDS ${work_dir}/chatGPT/bin/python3
            WORKING_DIRECTORY ${work_dir}/chatGPT/bin
        )

        add_custom_command(TARGET unzip_python POST_BUILD
            COMMENT "   Create OS depends Symbolic Link for ChatGPT Python    "
            COMMAND ${CMAKE_COMMAND} -E create_symlink libssl.so.1.0.2k libssl.so.10
            COMMAND ${CMAKE_COMMAND} -E create_symlink libffi.so.6.0.1 libffi.so.6
            COMMAND ${CMAKE_COMMAND} -E create_symlink libcrypto.so.1.0.2k libcrypto.so.10
            DEPENDS ${work_dir}/chatGPT/bin/python3
            WORKING_DIRECTORY ${work_dir}/chatGPT/lib/os_libs
        )
    endif()
endif ()
endif()


if(NOT RAPTOR)
install(
      DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/share/envs DESTINATION  ${CMAKE_INSTALL_PREFIX}
      USE_SOURCE_PERMISSIONS
)
endif()
# ---------------------------------------------------------------------------------
#execute script on install time
#install( CODE 
#                "
#message(STATUS \"${CMAKE_INSTALL_PREFIX} Ruuning at installation time\")
#execute_process(COMMAND  bash ${PROJECT_SOURCE_DIR}/gen_venv.sh -i ${CMAKE_INSTALL_PREFIX} -w ${work_dir} -s install)
#        "
#)


