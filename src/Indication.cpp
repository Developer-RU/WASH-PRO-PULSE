#include "Indication.hpp"

extern uint8_t state;

namespace IndicationNS
{
    static void init(void)
    {
        pinMode(PB14, OUTPUT);
        digitalWrite(PB14, HIGH);

        pinMode(PB15, OUTPUT);
        digitalWrite(PB15, HIGH);
    }

    static void loop(void)
    {
        digitalToggle(PB14);

        if (state == 0x03)
        {
            digitalToggle(PB15);
        }
        else
        {
            digitalWrite(PB15, HIGH);
        }

        vTaskDelay(100);
    }

    void TaskIndication(void *pvParameters __attribute__((unused)))
    {
        init();

        for (;;)
        {
            loop();
        }
    }
};