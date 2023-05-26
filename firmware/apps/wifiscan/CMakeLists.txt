add_library(AppWifiScan STATIC
    App/WifiScan.cpp
    App/WifiScan.hpp
)
target_include_directories(AppWifiScan
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppWifiScan
    AppMenu
    stringdb
)
