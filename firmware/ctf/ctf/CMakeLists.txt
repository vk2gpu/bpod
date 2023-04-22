add_library(ctf STATIC
    ctf.c
    ctf.h
)
target_include_directories(ctf
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(ctf
    __idf_arduino-esp32
)
