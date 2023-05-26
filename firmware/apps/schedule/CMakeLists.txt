add_library(AppSchedule STATIC
    App/Schedule.cpp
    App/Schedule.hpp
)
target_include_directories(AppSchedule
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppSchedule
    AppMenu
    AppTextView
    schedulec
    stringdb
)
