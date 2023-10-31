add_library(AppToolMenu STATIC
    App/ToolMenu.cpp
    App/ToolMenu.hpp
)
target_include_directories(AppToolMenu
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppToolMenu
    AppMenu
    AppShockCollar
    AppI2CDetect
    AppI2CSniff
    AppSPISniff
    AppUARTTerm
    AppGPIO
    AppLeds
    AppMCP23017
    AppMCP23S17
    # AppWifiScan
    ctf
    stringdb
)
