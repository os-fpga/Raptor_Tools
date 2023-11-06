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
                        ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/external/nlohmann_json/ )

file(GLOB_RECURSE EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/pin_map/*.cpp)
list(APPEND EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

#suppress Verific warnings
add_compile_options(-w)
# Create the library
add_library(libpinmap STATIC
    ${LIB_SOURCE}
)
SET_TARGET_PROPERTIES(libpinmap PROPERTIES PREFIX "")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

add_executable(pin_map ${EXEC_SOURCE})
