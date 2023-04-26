add_library(App STATIC
    App/App.cpp
    App/App.hpp
)
target_include_directories(App
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(App
    Adafruit-GFX-Library
    keys
    score
)
