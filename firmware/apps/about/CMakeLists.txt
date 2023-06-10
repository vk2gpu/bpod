add_library(AppAbout STATIC
    App/About.cpp
    App/About.hpp
)
target_include_directories(AppAbout
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppAbout
    App
    Images
    stringdb
    githash
)
