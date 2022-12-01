cmake_minimum_required(VERSION 3.10)

# set the project name
project(read_edif)
#set (CMAKE_CXX_STANDARD 11)
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

file(GLOB_RECURSE LIB_SOURCES
    ./src/*.cpp)
file(GLOB_RECURSE LIB_HEADERS
    ./src/*.h)

file(GLOB_RECURSE EXEC_SOURCE ./test/main.cpp)
list(APPEND EXEC_SOURCE ${LIB_SOURCES})

files_to_dirs(LIB_HEADERS LIB_INCLUDE_DIRS ) #./src/*/*.h
include_directories(PUBLIC ${LIB_INCLUDE_DIRS})

# Create the library
add_library(libreadedif
    ${LIB_SOURCES}
)
set_target_properties(libreadedif PROPERTIES PREFIX "") # Avoid extra 'lib' prefix

# Add executable
add_executable(read_edif ${EXEC_SOURCE})

# Link executable with source library
target_link_libraries(read_edif LINK_PUBLIC libreadedif)
