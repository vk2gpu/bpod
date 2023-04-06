add_library(AppToolMenu STATIC
    App/ToolMenu.cpp
    App/ToolMenu.hpp
)
target_include_directories(AppToolMenu
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppToolMenu
    AppMenu
    AppI2CDetect
)
