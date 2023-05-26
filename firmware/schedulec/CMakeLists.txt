set(SCHEDULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../schedule")
set(SCHEDULE_MD "${SCHEDULE_DIR}/README.md")
set(SCHEDULE_H "${CMAKE_CURRENT_BINARY_DIR}/schedule_data.h")
add_library(schedulec STATIC
    src/schedulec.c
    src/schedulec.h
    ${SCHEDULE_H}
)
add_custom_command(
    OUTPUT ${SCHEDULE_H}
    DEPENDS ${RawImage}
        ${CMAKE_CURRENT_SOURCE_DIR}/schedule_encoder.py 
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/schedule_encoder.py
        --in-schedule-md ${SCHEDULE_MD}
        --out-h ${SCHEDULE_H}
)
target_include_directories(schedulec
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(schedulec
    __idf_arduino-esp32
    stringdb
)
