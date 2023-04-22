add_library(AppGPIO STATIC
    App/GPIO.cpp
    App/GPIO.hpp
)
target_include_directories(AppGPIO
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppGPIO
    AppMenu
)
