cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(bpod)

add_subdirectory("../" firmware)

target_link_libraries(__idf_main
    st7735
)
