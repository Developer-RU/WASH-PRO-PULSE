/**
 * @file Indication.cpp
 * @author Masyukov Pavel
 * @brief Implementation of the module for status LED indication.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
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
        // digitalToggle(PB14);
        digitalWrite(PB14, LOW);

        if (state == 0x03)
        {
            digitalWrite(PB15, LOW);
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