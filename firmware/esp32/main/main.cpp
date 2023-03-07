#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"

#include <Arduino.h>
#include <Bpod/Bpod.hpp>

extern "C" void app_main(void)
{
    initArduino();
    Bpod::begin();
    while ( 1 )
    {
        Bpod::loop();
        delay(1);
    }
}
