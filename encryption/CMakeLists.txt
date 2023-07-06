cmake_minimum_required(VERSION 3.10)

# set the project name
project(enc_verilog)

if (PRODUCTION_BUILD)
    message("Production Build type set to ON")
    set (PRODUCTION_BUILD_FLAG "-DPRODUCTION_BUILD=1")
    add_definitions(-DPRODUCTION_BUILD)
    if (NOT RAPTOR)
        get_filename_component(FLEX_LM_SRC_DIR "../Flex_LM"
          REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

        # Flex_LM directory already has been added by vpr/CMakeLists.txt
    endif()
    message("FLEX: "  ${FLEX_LM_SRC_DIR})
endif(PRODUCTION_BUILD)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(files_to_dirs file_list directory_list)
    # Extract the directories for each header file
    foreach(file ${${file_list}})
        get_filename_component(dir ${file} DIRECTORY)
        list(APPEND dir_list ${dir})
    endforeach()

    # Remove any duplicates
    list(LENGTH "${dir_list}" length)

    if(${length} GREATER 1) # Avoid error with zero-length lists
        list(REMOVE_DUPLICATES ${dir_list})
    endif()

    # Set the second argument in the caller's scope
    set(${directory_list} ${dir_list} PARENT_SCOPE)
endfunction(files_to_dirs)

get_filename_component(VERIFIC_HOME "../verific_rs"
    REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE)
message("VERIFIC_HOME: ${VERIFIC_HOME}")
file(GLOB_RECURSE LIB_HEADERS
    ${VERIFIC_HOME}/*/*.h)

if (APPLE)
    set(OS "mac")
elseif (UNIX)
    set(OS "linux")
endif()

list(APPEND VERIFIC_LIBS
    ${VERIFIC_HOME}/verilog/verilog-${OS}.a
    ${VERIFIC_HOME}/database/database-${OS}.a
    ${VERIFIC_HOME}/util/util-${OS}.a
    ${VERIFIC_HOME}/containers/containers-${OS}.a
    ${VERIFIC_HOME}/hier_tree/hier_tree-${OS}.a
    ${VERIFIC_HOME}/vhdl/vhdl-${OS}.a
    ${VERIFIC_HOME}/hdl_encrypt/hdl_encrypt-linux.a
    )

files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS)
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    message("SSL not found")
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

include_directories(PUBLIC ${LIB_INCLUDE_DIRS}
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/enc_verilog
                            ${CMAKE_CURRENT_BINARY_DIR}/include
)

file(GLOB_RECURSE EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)
list(APPEND EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add executable
add_executable(enc_verilog ${EXEC_SOURCE})
# for internal use
if(NOT PRODUCTION_BUILD)
    target_link_libraries(enc_verilog PRIVATE ${VERIFIC_LIBS} ${OPENSSL_LIBRARIES})
endif()

# packaging related stuff
if(PACKAGING)
    add_custom_command(TARGET enc_verilog POST_BUILD
          COMMAND echo "Copy invoker script"
          COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/bin
          COMMAND ${CMAKE_COMMAND} -E copy
              ${PROJECT_SOURCE_DIR}/src/rs_encrypt
              ${CMAKE_CURRENT_BINARY_DIR}/bin
          COMMAND ${CMAKE_COMMAND} -E copy_directory
              ${PROJECT_SOURCE_DIR}/examples
              ${CMAKE_CURRENT_BINARY_DIR}/examples
    )

    set(FETCHCONTENT_QUIET FALSE)
    set(libs_url https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.1/encryption_tool_libs.tar.gz)
    set(Destination_Dir ${CMAKE_CURRENT_BINARY_DIR}/lib.tar.gz)
    set(DO_DOWNLOAD 0)
    set(DO_EXTRACTION 0)
    if(EXISTS ${Destination_Dir})
        message(STATUS "Libs downlaoded tar already exist.")
        set(DO_EXTRACTION 1)
    else()
        message(STATUS "Fresh Downloading Libs dir")
        set(DO_DOWNLOAD 1)
        set(DO_EXTRACTION 1)
    endif()

    if(${DO_DOWNLOAD} EQUAL 1)
        file(DOWNLOAD ${libs_url} ${Destination_Dir} SHOW_PROGRESS STATUS DOWNLOAD_STATUS)
        list (GET DOWNLOAD_STATUS 0 STATUS_CODE)
        if (NOT ${STATUS_CODE} EQUAL "0")
            message(FATAL_ERROR "Exit status of download HDL Sim is ${STATUS_CODE}")
        else()
            message(STATUS "Successfully Donwloadeded Libs)") 
        endif()
    endif ()
    if(${DO_EXTRACTION} EQUAL 1)
     add_custom_command(TARGET enc_verilog POST_BUILD
                     COMMENT      "      Extracting Libs        "
                     COMMAND ${CMAKE_COMMAND} -E tar xvzf ${Destination_Dir}
                     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                     )
     else ()
          message(STATUS "Extracted Libs exist and haven't download new tar so skipping it")
    endif()
    add_custom_command(TARGET enc_verilog POST_BUILD
    COMMENT      "      Creating Final Tar        "
    COMMAND ${CMAKE_COMMAND} -E tar cvzf rs_encryption_tool.tar.gz lib bin examples
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
endif() 

