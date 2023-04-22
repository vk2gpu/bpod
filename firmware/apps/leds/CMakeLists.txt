add_library(AppLeds STATIC
    App/Leds.cpp
    App/Leds.hpp
)
target_include_directories(AppLeds
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppLeds
    AppMenu
)
