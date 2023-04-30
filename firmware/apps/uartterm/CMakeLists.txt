add_library(AppUARTTerm STATIC
    App/UARTTerm.cpp
    App/UARTTerm.hpp
)
target_include_directories(AppUARTTerm
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppUARTTerm
    App
    AppMenu
    AppTextView
    AppDiagramView
)
