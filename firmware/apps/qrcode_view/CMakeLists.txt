add_library(AppQrCodeView STATIC
    App/QrCodeView.cpp
    App/QrCodeView.hpp
)
target_include_directories(AppQrCodeView
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(AppQrCodeView
    qrcodegen
    App
    Images
)
