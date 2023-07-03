set(RAW_MUSIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../music")
set(MUSIC_DIR "${CMAKE_CURRENT_BINARY_DIR}/Music")

list(APPEND RawMusic
    ${RAW_MUSIC_DIR}/u2_snippet.wav
)
list(APPEND HppMusic
    ${MUSIC_DIR}/u2_snippet.hpp
)

foreach(RawMusic HppMusic IN ZIP_LISTS RawMusic HppMusic)
    message(${HppMusic})
    message(${RawMusic})
    add_custom_command(
        OUTPUT ${HppMusic}
        DEPENDS ${RawMusic}
            ${CMAKE_CURRENT_SOURCE_DIR}/music_encoder.py 
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/music_encoder.py
            --in-music ${RawMusic}
            --out-hpp ${HppMusic}
    )
endforeach()

add_library(Music STATIC
    ${HppMusic}
    Music.cpp
)
target_include_directories(Music
    PUBLIC ${MUSIC_DIR}/../
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Music
    Adafruit-GFX-Library
)
