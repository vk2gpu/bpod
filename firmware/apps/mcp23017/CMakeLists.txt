add_library(AppMCP23017 STATIC
    App/MCP23017.cpp
    App/MCP23017.hpp
)
target_include_directories(AppMCP23017
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppMCP23017
    AppMenu
)
