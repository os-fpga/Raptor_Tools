cmake_minimum_required(VERSION 3.10)

# set the project name
project(gen_map)

if(RAPTOR)
    get_filename_component(DEVICES_DIR "../../../../etc/devices"
          REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
    set(DEVICES_PATH ${DEVICES_DIR})
    add_definitions(-DDEVICES_PATH="${DEVICES_PATH}")
    message("DP: "  ${DEVICES_PATH})
else()
    get_filename_component(DEVICES_DIR "../devices"
        REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE)
    set(DEVICES_PATH ${DEVICES_DIR})
    add_definitions(-DDEVICES_PATH="${DEVICES_PATH}")
    message("DP: "  ${DEVICES_PATH})
endif()

get_filename_component(OUT_DIR "../src/veri_prune/gb_map.h"
    REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE)
set(OUT_PATH ${OUT_DIR})
add_definitions(-DOUT_PATH="${OUT_PATH}")

include_directories(PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../../libs/external/ryml
                    ${CMAKE_CURRENT_SOURCE_DIR}/src/)

                    # Add a custom command to generate the header file

add_executable(generate_map ${CMAKE_CURRENT_SOURCE_DIR}/src/gen_map.cpp)

add_custom_command(
    OUTPUT ${OUT_PATH}
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/generate_map
    DEPENDS generate_map
    COMMENT "Generating gb_map.h"
)

# Dummy target to ensure the custom command is executed before the parent build continues
add_custom_target(generate_map_target ALL
    DEPENDS ${OUT_PATH}  # Make sure to include the generated header file as a dependency
)