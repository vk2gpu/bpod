add_library(AppTetris STATIC
    App/Tetris.cpp
    App/Tetris.hpp
)
target_include_directories(AppTetris
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppTetris
    App
    Images
    tetrisc
)
