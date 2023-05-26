add_library(Menu STATIC
    Menu/BpodMenu.hpp
    Menu/BpodMenu.cpp
)
target_include_directories(Menu
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Menu
    Adafruit-GFX-Library
    Images
    stringdb
)
