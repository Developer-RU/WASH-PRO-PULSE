#include "Pulse.hpp"

extern uint8_t count;

namespace PulseNS
{
    // uint8_t speed{10};

    static void init(void)
    {
        // pinMode(PB1, OUTPUT);
        // digitalWrite(PB1, LOW);

        // pinMode(PB11, INPUT_PULLDOWN);
        // pinMode(PB10, INPUT_PULLDOWN);

        // pinMode(PA0, INPUT_PULLUP);

    }

    static void loop(void)
    {
        // if (digitalRead(PA0) == LOW)// || digitalRead(PB10) == HIGH)
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