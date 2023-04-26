add_library(score STATIC
    score.c
    score.h
)
target_include_directories(score
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
