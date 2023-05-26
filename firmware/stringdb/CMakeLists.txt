set(STRINGDB_JSON "${CMAKE_CURRENT_SOURCE_DIR}/stringdb.json")
set(STRINGDB_MACRO_H "${CMAKE_CURRENT_BINARY_DIR}/stringdb_macro.h")
set(STRINGDB_DATA_H "${CMAKE_CURRENT_BINARY_DIR}/stringdb_data.h")
add_custom_command(
    OUTPUT ${STRINGDB_MACRO_H} ${STRINGDB_DATA_H}
    DEPENDS ${STRINGDB_JSON}
        ${CMAKE_CURRENT_SOURCE_DIR}/stringdb.py 
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/stringdb.py
        --in-json ${STRINGDB_JSON}
        --out-macro-h ${STRINGDB_MACRO_H}
        --out-data-h ${STRINGDB_DATA_H}
)
add_library(stringdb STATIC
    stringdb.c
    stringdb.h
    ${STRINGDB_MACRO_H}
    ${STRINGDB_DATA_H}
)
target_include_directories(stringdb
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
)
