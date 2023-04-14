add_library(AppSerialMonitor STATIC
    App/SerialMonitor.cpp
    App/SerialMonitor.hpp
)
target_include_directories(AppSerialMonitor
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppSerialMonitor
    App
    AppMenu
    AppTextView
)
