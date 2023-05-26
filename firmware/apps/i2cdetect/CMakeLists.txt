add_library(AppI2CDetect STATIC
    App/I2CDetect.cpp
    App/I2CDetect.hpp
)
target_include_directories(AppI2CDetect
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppI2CDetect
    App
    Images
    stringdb
)
