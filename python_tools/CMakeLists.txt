cmake_minimum_required(VERSION 3.15)


find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

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

message(${PROJECT_SOURCE_DIR})

#list of packages
list(APPEND packages_for_venv
    https://github.com/enjoy-digital/litex 
    https://github.com/m-labs/migen
  )

if (RAPTOR)
set(work_dir ${PROJECT_SOURCE_DIR}/..)
else ()
set(work_dir ${PROJECT_SOURCE_DIR})
endif()
message(STATUS "WORK DIR from cmake is ${work_dir}")

if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/share/envs/litex/.venv)
    message(STATUS ".venv file exist, so not creating virtual environment again")

else ()
message(STATUS "Litex virtual environment installation started")
find_package(Python COMPONENTS Interpreter REQUIRED)

execute_process(COMMAND bash ${PROJECT_SOURCE_DIR}/gen_venv.sh -w ${work_dir} -s build ${packages_for_venv}) 
message(STATUS "Litex virtual environment installation completed")
endif()                

install( CODE 
                "
message(STATUS \"${CMAKE_INSTALL_PREFIX} Ruuning at installation time\")
execute_process(COMMAND  bash ${PROJECT_SOURCE_DIR}/gen_venv.sh -i ${CMAKE_INSTALL_PREFIX} -w ${work_dir} -s install)
        "
)

