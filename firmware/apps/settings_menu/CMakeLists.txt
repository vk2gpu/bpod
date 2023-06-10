add_library(AppSettingsMenu STATIC
    App/SettingsMenu.cpp
    App/SettingsMenu.hpp
)
target_include_directories(AppSettingsMenu
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppSettingsMenu
    AppMenu
    AppBrightness
    AppAbout
    AppQrCodeView
    stringdb
)
