#include "Pulse.hpp"

extern uint8_t count;

namespace PulseNS
{
    void buttonPressed()
    {
        count++;
    }

    static void init(void)
    {
        pinMode(PA0, INPUT_PULLUP);
        attachInterrupt(PA0, buttonPressed, FALLING);
    }

    static void loop(void)
    {
        // {
        //     count++;
        // }

        vTaskDelay(2);
    }

    void TaskPulse(void *pvParameters __attribute__((unused)))
    {
        init();

        for (;;)
        {
            loop();
        }
    }
};