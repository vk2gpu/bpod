add_library(AppSnake STATIC
    App/Snake.cpp
    App/Snake.hpp
)
target_include_directories(AppSnake
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppSnake
    App
    snakec
)
