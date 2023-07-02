set(RAW_IMAGE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../images")
set(IMAGES_DIR "${CMAKE_CURRENT_BINARY_DIR}/Images")

list(APPEND RawImages
    ${RAW_IMAGE_DIR}/bpod-icon.bmp
    ${RAW_IMAGE_DIR}/bsides-2023-icon.bmp
    ${RAW_IMAGE_DIR}/bpod-title-bar-colors.bmp
    ${RAW_IMAGE_DIR}/bpod-title-bar-colors-dark.bmp
    ${RAW_IMAGE_DIR}/bpod-menu-bar-colors.bmp
    ${RAW_IMAGE_DIR}/bpod-menu-bar-arrow.bmp
    ${RAW_IMAGE_DIR}/bpod-scroll-empty.bmp
    ${RAW_IMAGE_DIR}/bpod-scroll-fill.bmp
    ${RAW_IMAGE_DIR}/snake_apple_6x6.bmp
    ${RAW_IMAGE_DIR}/u2_snippet.bmp
)
list(APPEND HppImages
    ${IMAGES_DIR}/BpodLogo.hpp
    ${IMAGES_DIR}/BSidesLogo.hpp
    ${IMAGES_DIR}/BpodTitleBarBackground.hpp
    ${IMAGES_DIR}/BpodTitleBarBackgroundDark.hpp
    ${IMAGES_DIR}/BpodMenuItemBackground.hpp
    ${IMAGES_DIR}/BpodMenuItemArrow.hpp
    ${IMAGES_DIR}/BpodScrollEmpty.hpp
    ${IMAGES_DIR}/BpodScrollFill.hpp
    ${IMAGES_DIR}/SnakeApple6x6.hpp
    ${IMAGES_DIR}/U2AlbumArt.hpp
)

foreach(RawImage HppImage IN ZIP_LISTS RawImages HppImages)
    message(${HppImage})
    message(${RawImage})
    add_custom_command(
        OUTPUT ${HppImage}
        DEPENDS ${RawImage}
            ${CMAKE_CURRENT_SOURCE_DIR}/image_encoder.py 
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/image_encoder.py
            --in-image ${RawImage}
            --out-hpp ${HppImage}
    )
endforeach()

add_library(Images STATIC
    ${HppImages}
    Images.cpp
    Images/BpodTitleBar.cpp
    Images/BpodTitleBar.hpp
    Images/BpodScrollBar.hpp
    Images/BpodMenuItem.cpp
    Images/BpodMenuItem.hpp
)
target_include_directories(Images
    PUBLIC ${IMAGES_DIR}/../
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Images
    Adafruit-GFX-Library
    tinyaes
    score
)
