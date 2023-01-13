cmake_minimum_required(VERSION 3.10)

# set the project name
project(compare_ports)

# finding swig for tcl interface generation
find_package(SWIG REQUIRED)
include(FindTCL)
message(STATUS "tcl.h path is : ${TCL_INCLUDE_PATH}")
message(STATUS "libtcl.so path is : ${TCL_LIBRARY}")


file(GLOB_RECURSE LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

list(APPEND LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

include_directories(PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${TCL_INCLUDE_PATH}
)

#
# Tcl Interface Generation
#
set(compare_tcl-GENERATED_SRC
    ${PROJECT_SOURCE_DIR}/src/compare_ports_wrap.cpp
)

# Create the static library
add_library(libcompare SHARED
    ${LIB_SOURCES}
    ${compare_tcl-GENERATED_SRC}
)
set_target_properties(libcompare PROPERTIES PREFIX "") # Avoid extra 'lib' prefix

foreach(gen_src ${compare_tcl-GENERATED_SRC})
    set_source_files_properties(${gen_src} PROPERTIES GENERATED TRUE)
endforeach()

add_custom_command(
    OUTPUT ${compare_tcl-GENERATED_SRC}
    DEPENDS ${PROJECT_SOURCE_DIR}/src/compare_ports.cpp
    ${PROJECT_SOURCE_DIR}/src/json.hpp
    ${PROJECT_SOURCE_DIR}/src/compare_ports.h
    ${PROJECT_SOURCE_DIR}/src/compare_ports.i
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    COMMAND echo "---------- Tcl compare API generation ----------"
    COMMAND ${SWIG_EXECUTABLE} -c++ -tcl -cppext cpp -o ${compare_tcl-GENERATED_SRC} ${PROJECT_SOURCE_DIR}/src/compare_ports.i
)

target_link_libraries(libcompare PUBLIC ${TCL_LIBRARY})
file(GLOB_RECURSE EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

list(APPEND EXEC_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

message(TRACE ${EXEC_SOURCE})

# create executable
add_executable(compare_info
    ${EXEC_SOURCE}
)
target_link_libraries(compare_info PUBLIC libcompare)
