cmake_minimum_required(VERSION 3.10)

# Detect build type, fallback to release and throw a warning if use didn't
# specify any
if(NOT CMAKE_BUILD_TYPE)
    message(WARNING "Build type not set, falling back to Release mode.
        To specify build type use:
        -DCMAKE_BUILD_TYPE=<mode> where <mode> is Debug or Release.")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build, options are: Debug Release."
        FORCE)
endif(NOT CMAKE_BUILD_TYPE)

# set the project name
project(pin_map)

# finding swig for tcl interface generation
find_package(SWIG REQUIRED)
include(FindTCL)
message(STATUS "tcl.h path is : ${TCL_INCLUDE_PATH}")
message(STATUS "libtcl.so path is : ${TCL_LIBRARY}")

set(CMAKE_CXX_STANDARD 17)

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

file(GLOB_RECURSE LIB_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/pin_map/*.cpp)
file(GLOB_RECURSE DUMP_TCL_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/dump_tcl/dump_tcl.cpp)

if (APPLE)
    set(OS "mac")
elseif (UNIX)
    set(OS "linux")
endif()

#include_directories(PUBLIC ${LIB_INCLUDE_DIRS}
#                            ${FLEX_LM_SRC_DIR}
#                            ${FLEX_LM_SRC_DIR}/machind
#                            ${CMAKE_CURRENT_SOURCE_DIR}/src/veri_prune
#                            ${CMAKE_CURRENT_BINARY_DIR}/include
#                            ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/external/nlohmann_json/
#)

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/pin_map/
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/dump_tcl/
                        ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/external/nlohmann_json/
                        ${TCL_INCLUDE_PATH})

file(GLOB_RECURSE EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/pin_map/*.cpp)
list(APPEND EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

#
# Tcl Interface Generation
#
set(tcl-GENERATED_SRC
    ${PROJECT_SOURCE_DIR}/src/dump_tcl/dump_tcl_wrap.cpp
)

# Create the static library
add_library(libdump SHARED
    ${DUMP_TCL_SOURCE}
    ${tcl-GENERATED_SRC}
)

set_target_properties(libdump PROPERTIES PREFIX "") # Avoid extra 'lib' prefix

foreach(gen_src ${tcl-GENERATED_SRC})
    set_source_files_properties(${gen_src} PROPERTIES GENERATED TRUE)
endforeach()

add_custom_command(
    OUTPUT ${tcl-GENERATED_SRC}
    DEPENDS ${PROJECT_SOURCE_DIR}/src/dump_tcl/dump_tcl.cpp
    ${PROJECT_SOURCE_DIR}/src/dump_tcl/dump_tcl.h
    ${PROJECT_SOURCE_DIR}/src/dump_tcl/dump_tcl.i
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMAND echo "---------- Tcl DESIGN API generation ----------"
    COMMAND ${SWIG_EXECUTABLE} -c++ -tcl -cppext cpp -o ${tcl-GENERATED_SRC} ${PROJECT_SOURCE_DIR}/src/dump_tcl/dump_tcl.i
)

target_link_libraries(libdump PUBLIC ${TCL_LIBRARY})
file(GLOB_RECURSE DUMP_EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src//dump_tcl/*.cpp)

message(TRACE ${DUMP_EXEC_SOURCE})
# create executable
add_executable(dump
    ${DUMP_EXEC_SOURCE}
)
target_link_libraries(dump PUBLIC libdump)

#suppress Verific warnings
add_compile_options(-w)
# Create the library
add_library(libpinmap STATIC
    ${LIB_SOURCE}
)
SET_TARGET_PROPERTIES(libpinmap PROPERTIES PREFIX "")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

add_executable(pin_map ${EXEC_SOURCE})
#add_executable(dump_tcl ${DUMP_TCL_SOURCE})
