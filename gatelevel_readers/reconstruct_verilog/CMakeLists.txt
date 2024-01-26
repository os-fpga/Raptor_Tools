cmake_minimum_required(VERSION 3.10) # Adjust to your CMake version

project(reconstruct_verilog)

# Specify the C++ standard you are using, e.g., C++11, C++14, C++17, C++20
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (PRODUCTION_BUILD)
   # if ((NOT RAPTOR) AND (NOT YOSYS_VERIFIC))
        # OpenLM_SRC_DIR should be made as required path.
        
        get_filename_component(OpenLM_SRC_DIR "../../OpenLM"
          REALPATH BASE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
        add_subdirectory(${OpenLM_SRC_DIR} OpenLM)
        message("OpenLM: "  ${OpenLM_SRC_DIR})
    # No need for else case.
  #  endif()
   # set(CXX_DEFINES "-DPRODUCTION_BUILD=1")
endif()

# Add the path where CMake can find additional CMake modules (if any)
# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Collect all the source files to be compiled
add_executable(finalize src/reconstruct_verilog.cpp)
target_include_directories(finalize PUBLIC  "${PROJECT_SOURCE_DIR}/../../libs/external/boost")
set_target_properties(finalize PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#target_link_libraries(finalize PUBLIC rs_licenseManager)

# If you have any additional libraries to link, use target_link_libraries
# target_link_libraries(reconstruct_verilog PRIVATE some_library)
if (PRODUCTION_BUILD)
  #  add_dependencies(finalize rs_licenseManager)
    target_include_directories(finalize PUBLIC ${OpenLM_SRC_DIR})
    target_link_libraries(finalize PUBLIC rs_licenseManager)
endif()