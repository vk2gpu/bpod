add_library(viewerc STATIC
    src/viewerc.c
    src/viewerc.h
)
target_include_directories(viewerc
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(viewerc
    __idf_arduino-esp32
)
