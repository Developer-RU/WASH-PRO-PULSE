#include "Pulse.hpp"

extern uint8_t count;

namespace PulseNS
{
    unsigned long timerComplete = 0;
    uint8_t newCount = 0;

    void buttonPressed()
    {
        newCount++;
        timerComplete = millis();
    }

    static void init(void)
    {
        pinMode(PA0, INPUT_PULLUP);
        attachInterrupt(PA0, buttonPressed, FALLING);
    }

    static void loop(void)
    {
        if(millis() > timerComplete + 100 && newCount > 0)
        {
            count = newCount;
            newCount = 0;
        }

        vTaskDelay(100);
    }

    void TaskPulse(void *pvParameters __attribute__((unused)))
    {
        init();

        timerComplete = millis();

        for (;;)
        {
            loop();
        }
    }
};