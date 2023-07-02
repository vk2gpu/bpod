add_library(AppMusicMenu STATIC
    App/MusicMenu.cpp
    App/MusicMenu.hpp
)
target_include_directories(AppMusicMenu
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppMusicMenu
    App
    AppMenu
    Images
    Music
    stringdb
)
