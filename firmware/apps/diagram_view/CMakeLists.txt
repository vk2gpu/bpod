add_library(AppDiagramView STATIC
    App/DiagramView.cpp
    App/DiagramView.hpp
)
target_include_directories(AppDiagramView
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppDiagramView
    App
    Images
    stringdb
)
