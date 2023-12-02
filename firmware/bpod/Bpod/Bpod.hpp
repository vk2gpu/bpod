#pragma once

#define BPOD_USE_FREERTOS        1

class Bpod
{
    public:
        static void begin();
        static void loop();
};
