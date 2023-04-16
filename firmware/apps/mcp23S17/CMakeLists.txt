add_library(AppMCP23S17 STATIC
    App/MCP23S17.cpp
    App/MCP23S17.hpp
)
target_include_directories(AppMCP23S17
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppMCP23S17
    AppMenu
)
