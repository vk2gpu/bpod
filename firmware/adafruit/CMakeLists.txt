add_library(Adafruit_BusIO STATIC
    Adafruit_BusIO/Adafruit_BusIO_Register.cpp
    Adafruit_BusIO/Adafruit_I2CDevice.cpp
    Adafruit_BusIO/Adafruit_SPIDevice.cpp
)
target_include_directories(Adafruit_BusIO
    PUBLIC Adafruit_BusIO
)
target_link_libraries(Adafruit_BusIO
    __idf_arduino-esp32
)

add_library(Adafruit-GFX-Library STATIC
    Adafruit-GFX-Library/Adafruit_GFX.cpp
    Adafruit-GFX-Library/Adafruit_GrayOLED.cpp
    Adafruit-GFX-Library/Adafruit_SPITFT.cpp
)
target_include_directories(Adafruit-GFX-Library
    PUBLIC Adafruit-GFX-Library
)
target_link_libraries(Adafruit-GFX-Library
    __idf_arduino-esp32
    Adafruit_BusIO
)

add_library(Adafruit-ST7735-Library STATIC
    Adafruit-ST7735-Library/Adafruit_ST77xx.cpp
    Adafruit-ST7735-Library/Adafruit_ST7735.cpp
    Adafruit-ST7735-Library/Adafruit_ST7789.cpp
)
target_include_directories(Adafruit-ST7735-Library
    PUBLIC Adafruit-ST7735-Library
)
target_link_libraries(Adafruit-ST7735-Library
    __idf_arduino-esp32
    Adafruit-GFX-Library
)
