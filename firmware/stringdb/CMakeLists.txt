set(STRINGDB_JSON "${CMAKE_CURRENT_SOURCE_DIR}/stringdb.json")
set(STRINGDB_MACRO_H "${CMAKE_CURRENT_BINARY_DIR}/stringdb_macro.h")
set(STRINGDB_DATA_H "${CMAKE_CURRENT_BINARY_DIR}/stringdb_data.h")
set(STRINGDB_DEPEND_LIST
    "${CMAKE_CURRENT_BINARY_DIR}/../ctf/ctf/stringdb_flag.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/../apps/i2csniff/notes.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/../apps/mcp23S17/notes.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/../apps/mcp23S17/datasheet.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/../apps/mcp23017/notes.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/../apps/mcp23017/datasheet.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/../apps/uartterm/notes.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/../apps/spisniff/notes.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../server/score_submit_url.txt"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../server/update_url.txt"
)
add_custom_command(
    OUTPUT ${STRINGDB_MACRO_H} ${STRINGDB_DATA_H}
    DEPENDS ${STRINGDB_JSON} ${STRINGDB_DEPEND_LIST}
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
target_link_libraries(stringdb
    tinyaes
    ctf
)
