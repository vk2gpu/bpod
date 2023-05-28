add_library(Bpod STATIC
    Bpod/Bpod.hpp
    Bpod/Bpod.cpp
)
target_include_directories(Bpod
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Bpod
    Adafruit-ST7735-Library
    TouchWheel
    Images
    Animations
    App
    AppMainMenu
    score
    ctf
)
