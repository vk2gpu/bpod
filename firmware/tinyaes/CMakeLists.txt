add_library(tinyaes STATIC
    src/tinyaes.c
    src/tinyaes.h
)
target_include_directories(tinyaes
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(tinyaes
    __idf_arduino-esp32
)
