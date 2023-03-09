add_library(snakec STATIC
    src/snakec.c
    src/snakec.h
)
target_include_directories(snakec
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
