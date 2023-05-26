add_library(AppGameMenu STATIC
    App/GameMenu.cpp
    App/GameMenu.hpp
)
target_include_directories(AppGameMenu
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppGameMenu
    AppMenu
    AppSnake
    AppTetris
    ctf
    score
    stringdb
)
