add_library(TouchWheel STATIC
    TouchWheel/TouchWheel.cpp
)
target_include_directories(TouchWheel
    PUBLIC TouchWheel
)
target_link_libraries(TouchWheel
    __idf_arduino-esp32
    ctf
    keys
    score
)
