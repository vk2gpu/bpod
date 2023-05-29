add_library(AppBrightness STATIC
    App/Brightness.cpp
    App/Brightness.hpp
)
target_include_directories(AppBrightness
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppBrightness
    App
    Images
    stringdb
)
