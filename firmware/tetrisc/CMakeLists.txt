add_library(tetrisc STATIC
    src/tetrisc.c
    src/tetrisc.h
)
target_include_directories(tetrisc
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_link_libraries(tetrisc
    __idf_arduino-esp32
)
