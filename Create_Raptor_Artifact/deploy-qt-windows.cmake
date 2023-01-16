set(WINDEPLOYQT "WINDEPLOYQT-NOTFOUND")
set(COMPONENT_NAME_MAIN "Raptor")
set(CMAKE_CURRENT_SOURCE_DIR "/nfs_scratch/scratch/eda/nadeem/rapotr_gui_installer/Raptor_Tools/Create_Raptor_Artifact")


execute_process(COMMAND ${WINDEPLOYQT} --qmldir ${CMAKE_CURRENT_SOURCE_DIR} ${COMPONENT_NAME_MAIN}/data/bin WORKING_DIRECTORY ${CPACK_TEMPORARY_INSTALL_DIRECTORY}/packages)

