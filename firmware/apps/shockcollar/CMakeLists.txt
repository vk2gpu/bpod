add_library(AppShockCollar STATIC
    App/ShockCollar.cpp
    App/ShockCollar.hpp
    App/ShockCollarRF.cpp
    App/ShockCollarRF.hpp
)
target_include_directories(AppShockCollar
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppShockCollar
    AppMenu
    stringdb
)
