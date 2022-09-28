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
project(read_verilog)

if (PRODUCTION_BUILD)
    message("Production Build type set to ON")
    set (PRODUCTION_BUILD_FLAG "-DPRODUCTION_BUILD=1")
    add_definitions(-DPRODUCTION_BUILD)
    if (NOT RAPTOR)
        get_filename_component(FLEX_LM_SRC_DIR "../../Flex_LM"
          REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
        
        # Flex_LM directory already has been added by vpr/CMakeLists.txt
    endif()
    message("FLEX: "  ${FLEX_LM_SRC_DIR})
endif(PRODUCTION_BUILD)

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

file(GLOB_RECURSE LIB_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)

get_filename_component(VERIFIC_HOME "../../verific_rs"
    REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE)
message("VERIFIC_HOME: ${VERIFIC_HOME}")
file(GLOB_RECURSE LIB_HEADERS
    ${VERIFIC_HOME}/*/*.h)

if (NOT RAPTOR AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_subdirectory(${VERIFIC_HOME} "verific_rs")
endif()

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
    )
set (BOOST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/boost)
set (BOOST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/boost)
#set (BOOST_LIBRARYDIR ${CMAKE_CURRENT_SOURCE_DIR}/boost/stage/lib)
set (Boost_NO_BOOST_CMAKE ON)
find_package (Boost)
include_directories(${Boost_INCLUDE_DIRS})
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
                            ${FLEX_LM_SRC_DIR}
                            ${FLEX_LM_SRC_DIR}/machind
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/read_nl
                            ${CMAKE_CURRENT_BINARY_DIR}/include
)

file(GLOB_RECURSE EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*/*.cpp)
list(APPEND EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)


# Add executable for internal use
if(NOT PRODUCTION_BUILD)
    add_executable(read_verilog ${EXEC_SOURCE})
    target_link_libraries(read_verilog PUBLIC ${VERIFIC_LIBS} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})
endif()

#suppress Verific warnings
add_compile_options(-w)
# Create the library
add_library(libreadver STATIC
    ${LIB_SOURCE}
)
SET_TARGET_PROPERTIES(libreadver PROPERTIES PREFIX "")
target_link_libraries(libreadver PUBLIC ${VERIFIC_LIBS} ${Boost_LIBRARIES})

if (NOT RAPTOR AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_dependencies(libreadver verific)
endif()
