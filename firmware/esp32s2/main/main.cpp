#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"

#include <Arduino.h>
#include <Bpod/Bpod.hpp>

#if BPOD_USE_FREERTOS
static TaskHandle_t s_loopTaskHandle = 0;
#endif // #if BPOD_USE_FREERTOS

void LoopTaskEntry(void*)
{
    while ( 1 )
    {
        Bpod::loop();
        delay(1);
    }
}

extern "C" void app_main(void)
{
    initArduino();
    Bpod::begin();
#if BPOD_USE_FREERTOS
    xTaskCreate(
        LoopTaskEntry, /* Function to implement the task */
        "LoopTask", /* Name of the task */
        10000,  /* Stack size in words */
        NULL,  /* Task input parameter */
        tskIDLE_PRIORITY,  /* Priority of the task */
        &s_loopTaskHandle);  /* Task handle. */

    for(;;)
    {
        delay(1);
    }
#else // #if BPOD_USE_FREERTOS
    MainTaskEntry(nullptr);
#endif // #else // #if BPOD_USE_FREERTOS
}
