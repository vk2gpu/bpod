add_library(AppUARTSniff STATIC
    App/UARTSniff.cpp
    App/UARTSniff.hpp
)
target_include_directories(AppUARTSniff
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppUARTSniff
    App
    AppMenu
    AppTextView
)
