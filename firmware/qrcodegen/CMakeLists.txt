add_library(qrcodegen STATIC
    src/qrcodegen.c
    src/qrcodegen.h
)
target_include_directories(qrcodegen
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
