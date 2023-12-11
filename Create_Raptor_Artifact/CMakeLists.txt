cmake_minimum_required(VERSION 3.15)

if (${CMAKE_VERSION} VERSION_GREATER "3.8")
    #For cmake >= 3.9 INTERPROCEDURAL_OPTIMIZATION behaviour we need to explicitly
    #set the cmake policy version number
    cmake_policy(VERSION 3.9) 

    # If we are using verison < 3.9 then setting INTERPROCEDURAL_OPTIMIZATION
    # has no effect unless an Intel compiler is used
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif(NOT CMAKE_BUILD_TYPE)

#if(NOT IN_RELEASE_DATE)
#    set(IN_RELEASE_DATE "Empty")
#endif()
## Get the current date and time as a string
#string(TIMESTAMP CURRENT_DATE "%Y-%m-%d" UTC)
#if(IN_RELEASE_DATE STREQUAL "Empty")
#    # Extract the year, month, and day components in required format
#    string(SUBSTRING "${CURRENT_DATE}" 0 4 RELEASE_YEAR)
#    string(SUBSTRING "${CURRENT_DATE}" 5 2 RELEASE_MONTH)
#else()
#    string(SUBSTRING "${IN_RELEASE_DATE}" 0 4 RELEASE_YEAR)
#    string(SUBSTRING "${IN_RELEASE_DATE}" 5 2 RELEASE_MONTH)
#endif()
#string(SUBSTRING "${CURRENT_DATE}" 8 2 RELEASE_DAY)
#set(RELEASE_VERSION ${RELEASE_YEAR}.${RELEASE_MONTH})

project(RaptorInstaller DESCRIPTION "Raptor installer")

set(FETCHCONTENT_QUIET FALSE)
#set(RAPTOR_INSTALL_PATH_V ${RAPTOR_INSTALL_PATH}/../RapidSilicon/Raptor/${RELEASE_VERSION})
set(EXT_LIBS_URL https://github.com/RapidSilicon/post_build_artifacts/releases/download/v0.2/external_libs_dec102023.tar.gz)
set(Destination_Dir ${CMAKE_CURRENT_SOURCE_DIR}/external_libs.tar.gz)
set(expected_size 83230291)

# Raptor Install Path,
if (NOT RAPTOR_INSTALL_PATH)
  set(RAPTOR_INSTALL_PATH "Empty")
endif() 

if(${RAPTOR_INSTALL_PATH} STREQUAL "Empty")
    message(FATAL_ERROR "---    RAPTOR_INSTALL_PATH does not exist. Exiting")
else()
    execute_process(COMMAND ${RAPTOR_INSTALL_PATH}/bin/raptor --version OUTPUT_VARIABLE COMMAND_OUTPUT)
    string(REGEX MATCH "Version[ ]+:[ ]+([0-9]+\\.[0-9]+)" VERSION_MATCH "${COMMAND_OUTPUT}")
    set(VERSION_NUMBER ${CMAKE_MATCH_1})
    message(STATUS "Extracted Version Number: ${VERSION_NUMBER}")
    string(SUBSTRING "${VERSION_NUMBER}" 0 4 RELEASE_YEAR)
    string(SUBSTRING "${VERSION_NUMBER}" 5 2 RELEASE_MONTH)
    set(RELEASE_DAY 01)
    message(STATUS "so release yyyy.mm.dd is ${RELEASE_YEAR}.${RELEASE_MONTH}.${RELEASE_DAY}")
endif()

# get all devices vpr.xml file
file(GLOB RS_DEVICES ${RAPTOR_INSTALL_PATH}/share/raptor/etc/devices/*/*_vpr.xml)

message(STATUS "---     Following Devices are found ${RS_DEVICES}")

# change the device name in device.xml  
if (EXISTS ${RAPTOR_INSTALL_PATH}/share/raptor/etc/device.xml)
    message(STATUS "---     Provided Raptor Install Path with is ${RAPTOR_INSTALL_PATH}")
    file(READ ${RAPTOR_INSTALL_PATH}/share/raptor/etc/device.xml FILE_CONTENTS)
    foreach(device IN LISTS RS_DEVICES)
        get_filename_component(vpr_file "${device}" NAME)
        get_filename_component(full_dir "${device}" DIRECTORY)
        get_filename_component(dir_ "${full_dir}" NAME)
        set(full_device_name ${dir_}/${vpr_file})
        message(STATUS "---      Updating ${full_device_name} ...")
        string(REPLACE "${full_device_name}" "${full_device_name}e" FILE_CONTENTS ${FILE_CONTENTS})  # TODO: this add e each time. Prevent it 
    endforeach()
    file(WRITE ${RAPTOR_INSTALL_PATH}/share/raptor/etc/device.xml ${FILE_CONTENTS})
    file(WRITE ${RAPTOR_INSTALL_PATH}/share/envs/litex/.venv "#write absolute path of directory where this file is present" )
    file(WRITE ${RAPTOR_INSTALL_PATH}/share/envs/litex/.project "#write absolute path of directory where this file is present" )
else()
    message(FATAL_ERROR "---    RAPTOR_INSTALL_PATH does not exist. Exiting")
endif()

# download external libs
if (NOT EXISTS  ${CMAKE_CURRENT_SOURCE_DIR}/external_libs.tar.gz)
    file(DOWNLOAD ${EXT_LIBS_URL} ${Destination_Dir} SHOW_PROGRESS STATUS DOWNLOAD_STATUS)
    list (GET DOWNLOAD_STATUS 0 STATUS_CODE)
    if (NOT ${STATUS_CODE} EQUAL "0")
        message(FATAL_ERROR "---    Exit status of download is ${STATUS_CODE}")
    else()
        message(STATUS "---     Successfully Donwloadeded External LIBS") 
    endif()
else()
    message(STATUS "---     Already downloaded external libs present so skipping downaload")
endif()

# Extract external libs
if(NOT EXISTS ${RAPTOR_INSTALL_PATH}/external_libs)
    message(STATUS "---     Extracting External Libs")
    execute_process (
            COMMAND ${CMAKE_COMMAND} -E tar xvzf ${Destination_Dir}
                                WORKING_DIRECTORY ${RAPTOR_INSTALL_PATH}
    )
else()
    message(STATUS "---     Extracted Content present so skipping it")
endif()

# compile encryption exe (This is now absolute and we are using the encryption exe from Backend directly) 
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Encryption_method/src/*.cpp")
find_package(OpenSSL REQUIRED)
add_executable(Encryption_method
        ${SOURCES}
)
target_include_directories(Encryption_method
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/Encryption_method/src
)
target_link_libraries(Encryption_method OpenSSL::Crypto)

# encrypt and delete
add_custom_command(TARGET Encryption_method POST_BUILD
    COMMENT "---    Encrypt XML files and copy README to top DIR"
    # execute encrypt_device.cmake script
    COMMAND ${CMAKE_COMMAND} -DENCRYPT_EXE="${CMAKE_CURRENT_SOURCE_DIR}/Encryption_method/encrypt" -DPUBLIC_KEY="${CMAKE_CURRENT_SOURCE_DIR}/Encryption_method/public_key.pem" -DDEVICES_LIST="${RS_DEVICES}"  -P ${CMAKE_CURRENT_SOURCE_DIR}/encrypt_devic.cmake
                         
    # below command will delete the files
    COMMAND ${CMAKE_COMMAND} -E remove -f  ${RS_DEVICES}
    COMMAND ${CMAKE_COMMAND} -E copy ${RAPTOR_INSTALL_PATH}/share/raptor/doc/README.md ${RAPTOR_INSTALL_PATH}                                   
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

# zip the release folder
add_custom_command(TARGET Encryption_method POST_BUILD
    COMMENT "---    Zip the Release"
    COMMAND ${CMAKE_COMMAND} -E remove -f  ${RAPTOR_INSTALL_PATH}/Raptor_release.zip
    COMMAND ${CMAKE_COMMAND} -E tar cvf Raptor_release.zip --format=zip ${RAPTOR_INSTALL_PATH}/* ${RAPTOR_INSTALL_PATH}/.raptorenv_lin64.sh                                    
    WORKING_DIRECTORY ${RAPTOR_INSTALL_PATH}
)

set(COMPONENT_NAME_MAIN "Raptor")
set(COMPONENT_NAME_DEPENDENCIES "RuntimeLibs")

install( FILES ${RAPTOR_INSTALL_PATH}/Raptor_release.zip
            DESTINATION . 
            COMPONENT ${COMPONENT_NAME_MAIN}
        )
install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/os_dep_install_scripts/unix_dep.sh
        DESTINATION ./share 
        COMPONENT ${COMPONENT_NAME_MAIN}
    )
install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/os_dep_install_scripts/changer_mod.sh
    DESTINATION ./share 
    COMPONENT ${COMPONENT_NAME_MAIN}
)

install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E remove -f  ./Raptor_release.zip)")

# Upon include(InstallRequiredSystemLibraries), install(PROGRAMS) is called.
# We can specify the install(PROGRAMS) command COMPONENT option.
# If none is specified and we use COMPONENT_NAME_MAIN for our target, CMAKE_INSTALL_DEFAULT_COMPONENT_NAME ("Unspecified") will be used
# If we specify COMPONENT_NAME_MAIN for this, cpack_ifw_configure_component(${COMPONENT_NAME_MAIN} LICENSES... doesn't work
set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT ${COMPONENT_NAME_DEPENDENCIES})
include(InstallRequiredSystemLibraries)

set(CPACK_GENERATOR "IFW")
#set(CPACK_IFW_ROOT "C:/Qt/Tools/QtInstallerFramework/4.1/") #QTIFWDIR is used by default

#set(CPACK_IFW_ROOT "C:/Qt6/Tools/QtInstallerFramework/4.1")
set(CPACK_IFW_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/QT_installer_exe/Linux/bin")

# ----- List of images to show on installation page -------
list(APPEND IMAGES_AT_INSTALL 
        ${CMAKE_CURRENT_SOURCE_DIR}/qt_installer_config/image_slide2.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/qt_installer_config/image_slide1.png 
        ${CMAKE_CURRENT_SOURCE_DIR}/qt_installer_config/image_slide3.png
  )
#------ Package Level Config --------------
set (CPACK_IFW_PACKAGE_NAME "Raptor")
set (CPACK_PACKAGE_VERSION ${RELEASE_YEAR}.${RELEASE_MONTH})
set (CPACK_IFW_PACKAGE_TITLE "Raptor Installer")
set (CPACK_IFW_PACKAGE_TITLE_COLOR "#000000")
set (CPACK_IFW_PRODUCT_URL "https://rapidsilicon.com/")
set (CPACK_IFW_PACKAGE_PUBLISHER "RapidSilicon")
set (CPACK_IFW_PACKAGE_WIZARD_SHOW_PAGE_LIST "ON")
set (CPACK_IFW_PACKAGE_LOGO ${CMAKE_CURRENT_SOURCE_DIR}/qt_installer_config/installer_logo.png)
set (CPACK_IFW_PACKAGE_START_MENU_DIRECTORY "raptor")
set (CPACK_IFW_PACKAGE_WIZARD_STYLE "Classic")
set (CPACK_IFW_PACKAGE_WIZARD_DEFAULT_HEIGHT "35em")
set (CPACK_IFW_PACKAGE_WIZARD_DEFAULT_WIDTH "70em")
set (CPACK_IFW_PACKAGE_STYLE_SHEET ${CMAKE_CURRENT_SOURCE_DIR}/qt_installer_config/style.qss)
set (CPACK_IFW_PACKAGE_MAINTENANCE_TOOL_NAME "uninstall")
set (CPACK_IFW_TARGET_DIRECTORY "@ApplicationsDir@/@Publisher@/")
set (CPACK_IFW_PACKAGE_CONTROL_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/qt_installer_config/script.qs)
set (CPACK_IFW_PACKAGE_PRODUCT_IMAGES ${IMAGES_AT_INSTALL})
# ------------ ***** ------------------------------

set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/../../licenses/rs-eula.txt)

find_program(WINDEPLOYQT windeployqt HINTS "${_qt_bin_dir}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/deploy-qt-windows.cmake.in" "${CMAKE_CURRENT_SOURCE_DIR}/deploy-qt-windows.cmake" @ONLY)

set(CPACK_PRE_BUILD_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/deploy-qt-windows.cmake)

include(CPack)
include(CPackIFW)

CPACK_ADD_COMPONENT(${COMPONENT_NAME_MAIN})

# List controlling which components are installed
#set(CPACK_COMPONENTS_ALL ${COMPONENT_NAME_MAIN})
#message(STATUS "CPACK_COMPONENTS_ALL = " ${CPACK_COMPONENTS_ALL})

# Install the runtime libs but don't show them in the installer
#cpack_ifw_configure_component(${COMPONENT_NAME_DEPENDENCIES} VIRTUAL FORCED_INSTALLATION)

# The Raptor component HAS to be installed
#cpack_ifw_configure_component(${COMPONENT_NAME_MAIN} ESSENTIAL FORCED_INSTALLATION)

# Adds license info to package.xml file
# Also copies licence.txt to 'meta' directory to display the license text in the installer.
cpack_ifw_configure_component(${COMPONENT_NAME_MAIN} LICENSES "EULA License" ${CPACK_RESOURCE_FILE_LICENSE}
                                                     VERSION        "${RELEASE_YEAR}.${RELEASE_MONTH}"
                                                     DESCRIPTION      "Install Raptor"
                                                     RELEASE_DATE   "${RELEASE_YEAR}-${RELEASE_MONTH}-${RELEASE_DAY}"
                                                     SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/packages/com.rapidsilicon.raptor/meta/compnet_script.qs
                                                     USER_INTERFACES ${CMAKE_CURRENT_SOURCE_DIR}/packages/com.rapidsilicon.raptor/meta/readmecheckboxform.ui
    )

