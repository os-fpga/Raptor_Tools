cmake_minimum_required(VERSION 3.10)

# set the project name
project(enc_verilog)

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

file(GLOB_RECURSE EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
get_filename_component(VERIFIC_HOME ../verific_rs REALPATH)
file(GLOB_RECURSE LIB_HEADERS
    ${VERIFIC_HOME}/*/*.h)
list(APPEND VERIFIC_LIBS
    ${VERIFIC_HOME}/verilog/verilog-linux.a
    ${VERIFIC_HOME}/database/database-linux.a
    ${VERIFIC_HOME}/util/util-linux.a
    ${VERIFIC_HOME}/containers/containers-linux.a
    ${VERIFIC_HOME}/hier_tree/hier_tree-linux.a
    ${VERIFIC_HOME}/vhdl/vhdl-linux.a
    ${VERIFIC_HOME}/hdl_encrypt/hdl_encrypt-linux.a
)

files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.h)

find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    message("SSL not found")
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

include_directories(PUBLIC ${LIB_INCLUDE_DIRS})

# Add executable
add_executable(enc_verilog ${EXEC_SOURCE})
add_library(enc_verilog_lib ${EXEC_SOURCE})

# Link executable with source library
target_link_libraries(enc_verilog LINK_PUBLIC ${VERIFIC_LIBS} ${OPENSSL_LIBRARIES})
target_link_libraries(enc_verilog_lib LINK_PUBLIC ${VERIFIC_LIBS} ${OPENSSL_LIBRARIES})

# Generate list of enc_verilog command for each file in library
file(GLOB files "library/*.v")

foreach(filename ${files})
    list(APPEND commands
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/enc_verilog ${filename})
endforeach()

# Target to clean everything
add_custom_target(clean_all
    COMMAND rm -rf CMakeFiles/ ieee1735_encrypt_verilog.golden.new CMakeCache.txt cmake_install.cmake enc_verilog Makefile libenc_verilog_lib.a out/ *.v
)

add_custom_target(all_
    COMMAND make
    COMMAND make enc_verilog
)
