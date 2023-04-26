add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/keys_k.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../../keys/master.hex
        ${CMAKE_CURRENT_SOURCE_DIR}/gen_key_header.py 
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/gen_key_header.py
        --in-key-hex ${CMAKE_CURRENT_SOURCE_DIR}/../../keys/master.hex
        --name KEY_K
        --out-h ${CMAKE_CURRENT_BINARY_DIR}/keys_k.h
)
add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/keys_k2.h
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../../keys/master2.hex
        ${CMAKE_CURRENT_SOURCE_DIR}/gen_key_header.py 
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/gen_key_header.py
        --in-key-hex ${CMAKE_CURRENT_SOURCE_DIR}/../../keys/master2.hex
        --name KEY_K2
        --out-h ${CMAKE_CURRENT_BINARY_DIR}/keys_k2.h
)
add_library(keys STATIC
    keys.c
    keys.h
    ${CMAKE_CURRENT_BINARY_DIR}/keys_k.h
    ${CMAKE_CURRENT_BINARY_DIR}/keys_k2.h
)
target_include_directories(keys
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)
