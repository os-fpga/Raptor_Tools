# this is a cmake script. Can be execute with 3 arguments
#   1. path of encrypt exe
#   2. list of devices to encrypt, space separated
#   3. public key
if(NOT DEFINED ENCRYPT_EXE OR NOT DEFINED DEVICES_LIST OR NOT DEFINED PUBLIC_KEY)
    message(FATAL_ERROR "ENCRYPT_EXE PUBLIC_KEY and DEVICES_LIST must be defined.")
endif()

separate_arguments(DEVICES_LIST)

foreach(device IN LISTS DEVICES_LIST)
    message("Encrypting ${device}")
    execute_process(
        COMMAND ${ENCRYPT_EXE} ${PUBLIC_KEY} ${device}
    )
endforeach()
