add_library(Animations STATIC
    Animations/BpodLoadAnimation.hpp
    Animations/BpodLoadAnimation.cpp
    Animations/BSidesLogoAnimation.hpp
    Animations/BSidesLogoAnimation.cpp
)
target_include_directories(Animations
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Animations
    Adafruit-GFX-Library
    Images
    stringdb
)
