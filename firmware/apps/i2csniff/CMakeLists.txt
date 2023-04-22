add_library(AppI2CSniff STATIC
    App/I2CSniff.cpp
    App/I2CSniff.hpp
)
target_include_directories(AppI2CSniff
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppI2CSniff
    App
    AppMenu
    AppDiagramView
    AppTextView
)
