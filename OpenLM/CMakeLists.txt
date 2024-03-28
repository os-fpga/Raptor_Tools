cmake_minimum_required(VERSION 3.7)

project(LicenseManager CXX)

# Uncomment below if using pre-compiled licenscc
#set(LICENSECC_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/licensecc/install/lib/cmake/licensecc/")
#set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/licensecc/install")
#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Set paths and variables
set(GIT_SUBMODULE OFF)
set(LCC_PROJECT_NAME "Raptor")

# inputs for licensing_feature.py
if(RAPTOR)
    set(XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/../../etc/device.xml)
    set(HAS_FOEDAG_RS "FOEDAG_rs/")
else()
    set(XML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/dummy.xml)
    set(HAS_FOEDAG_RS "")
endif()
set(FEATURES_TXT ${CMAKE_CURRENT_SOURCE_DIR}/features.txt)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../../${HAS_FOEDAG_RS}FOEDAG/third_party/openssl_cmake/CMakeLists.txt)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lib64 DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/../../${HAS_FOEDAG_RS}FOEDAG/third_party/openssl)
    set(OPENSSL_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../${HAS_FOEDAG_RS}FOEDAG/third_party/openssl)
    find_package(OpenSSL REQUIRED COMPONENTS Crypto)
    set(OPENSSL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../${HAS_FOEDAG_RS}FOEDAG/third_party/openssl/include)
    set(OPENSSL_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../../${HAS_FOEDAG_RS}FOEDAG/third_party/openssl/libssl.so ${CMAKE_CURRENT_SOURCE_DIR}/../../${HAS_FOEDAG_RS}FOEDAG/third_party/openssl/libcrypto.so)
    message(STATUS "****************** openssl include dir is ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "****************** openssl libs are dir is ${OPENSSL_LIBRARIES}")
endif()

cmake_policy(SET CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW) 
# Add subdirectory for licensecc
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/licensecc)
target_compile_options(licensecc_static PRIVATE
    # Add any other desired compiler options here
    -Wno-missing-field-initializers
)

# Custom target for installing licensecc
#add_custom_target(install_licensecc ALL
#   COMMENT "Building and installing licensecc"
#    COMMAND $(MAKE) install
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/licensecc
#)
#add_dependencies(install_licensecc licensecc_static)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/OPENLM_DIR/licensecc/extern/license-generator/src/license_generator/lccgen)
  add_dependencies(lcc_base licensecc_static)
  add_dependencies(license_generator_lib licensecc_static)
endif()

# Custom command to run licensing_feature.py script
add_custom_command(
    OUTPUT License_devices.cpp
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/licensing_feature.py --xml_filepath ${XML_FILE} --feature_txt_path ${FEATURES_TXT} 
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/licensing_feature.py
    COMMENT "Generating License_devices.cpp from licensing_feature.py"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# Custom target to ensure License_devices.cpp is generated before compiling the library
add_custom_target(generate_license_manager_hpp DEPENDS License_devices.cpp)

# Uncomment below to build example
#add_executable(example src/example.cpp License_manager.cpp)
#add_dependencies(example generate_license_manager_hpp)

# Ensure License_devices.cpp is generated before building these targets
add_library(rs_licenseManager License_manager.cpp ${CMAKE_CURRENT_BINARY_DIR}/License_devices.cpp)
add_dependencies(rs_licenseManager generate_license_manager_hpp)
target_include_directories(rs_licenseManager PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}  ${CMAKE_CURRENT_SOURCE_DIR}/licensecc/install/include ${CMAKE_CURRENT_SOURCE_DIR}/licensecc/install/include/licensecc/Raptor)

target_compile_options(rs_licenseManager PRIVATE
    # Add any other desired compiler options here
    -Wno-missing-field-initializers
)

# Link libraries
cmake_policy(SET CMP0028 NEW)
target_link_libraries(rs_licenseManager licensecc_static)
#add_dependencies(rs_licenseManager install_licensecc)
#target_link_libraries(example licensecc_static)

# path for privte key -s= /projects/Raptor/       projects folder is built in with exctuable in build folder
# -p /projects/Raptor/include/licensecc/Raptor/
#generating FOEDAG_rs/projects/Raptor/include/licensecc/Raptor/public_key.h 
#       and /FOEDAG_rs/projects/Raptor/private_key.rsa if they don't already exist
