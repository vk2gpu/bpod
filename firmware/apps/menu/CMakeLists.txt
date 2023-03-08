add_library(AppMenu STATIC
    App/Menu.cpp
    App/Menu.hpp
)
target_include_directories(AppMenu
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppMenu
    App
    Menu
)
