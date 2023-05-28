set(MANIFEST_LIST
    ${CMAKE_CURRENT_SOURCE_DIR}/../chals/badge/badge-blinky-bill/MANIFEST.yaml
    ${CMAKE_CURRENT_SOURCE_DIR}/../chals/badge/badge-cheesy-strings-i/MANIFEST.yaml
    ${CMAKE_CURRENT_SOURCE_DIR}/../chals/badge/badge-cheesy-strings-ii/MANIFEST.yaml
    ${CMAKE_CURRENT_SOURCE_DIR}/../chals/badge/badge-i-can-see-you/MANIFEST.yaml
    ${CMAKE_CURRENT_SOURCE_DIR}/../chals/badge/badge-i-spy-with-my-little-eye/MANIFEST.yaml
    ${CMAKE_CURRENT_SOURCE_DIR}/../chals/badge/badge-serial-hacker/MANIFEST.yaml
)
set(CTF_FLAGS_H "${CMAKE_CURRENT_BINARY_DIR}/ctf_flags.h")
add_custom_command(
    OUTPUT ${CTF_FLAGS_H}
    DEPENDS ${MANIFEST_LIST}
        ${CMAKE_CURRENT_SOURCE_DIR}/ctf_flags.py 
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/ctf_flags.py
        --in-manifest ${MANIFEST_LIST}
        --out-h ${CTF_FLAGS_H}
)
add_library(ctf STATIC
    ctf.c
    ctf.h
    ${CTF_FLAGS_H}
)
target_include_directories(ctf
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(ctf
    __idf_arduino-esp32
)
