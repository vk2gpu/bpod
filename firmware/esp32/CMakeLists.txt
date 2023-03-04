cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")  # because of error "undefined reference to `typeinfo for Print'"

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(bpod)

add_subdirectory("../" firmware)

target_link_libraries(__idf_main
    __idf_arduino-esp32
    Adafruit-ST7735-Library
    TouchWheel
    Images
    Animations
    Menu
)
