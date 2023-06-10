set(GITHASH_GEN_H "${CMAKE_CURRENT_BINARY_DIR}/githash_gen.h")
add_custom_command(
    OUTPUT ${GITHASH_GEN_H} missing_file_always_build_githash
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/githash.py
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/githash.py
        --out-h ${GITHASH_GEN_H}
)
add_library(githash STATIC
    githash.c
    githash.h
    ${GITHASH_GEN_H}
)
target_include_directories(githash
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)
target_link_libraries(githash
    stringdb
)
