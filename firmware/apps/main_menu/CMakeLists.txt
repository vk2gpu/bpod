add_library(AppMainMenu STATIC
    App/MainMenu.cpp
    App/MainMenu.hpp
)
target_include_directories(AppMainMenu
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppMainMenu
    AppMenu
    AppTextView
    AppQrCodeView
    AppSchedule
    AppGameMenu
    AppToolMenu
    AppAbout
    score
)
