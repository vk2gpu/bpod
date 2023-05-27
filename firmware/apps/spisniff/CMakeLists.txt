add_library(AppSPISniff STATIC
    App/SPISniff.cpp
    App/SPISniff.hpp
)
target_include_directories(AppSPISniff
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppSPISniff
    App
    AppMenu
    AppDiagramView
    AppTextView
    stringdb
)
