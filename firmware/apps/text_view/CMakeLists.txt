add_library(AppTextView STATIC
    App/TextView.cpp
    App/TextView.hpp
)
target_include_directories(AppTextView
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppTextView
    viewerc
    App
    Images
)
