/**
 * @file Indication.cpp
 * @author Masyukov Pavel
 * @brief Implementation of the module for status LED indication.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "Indication.hpp"

extern uint8_t state; ///< Global state machine variable.

namespace IndicationNS
{
    /**
     * @brief Initializes the LED pins.
     * 
     * Configures pins PB14 and PB15 as outputs and sets them to HIGH (turns off LEDs).
     */
    static void init(void)
    {
        pinMode(PB14, OUTPUT);
        digitalWrite(PB14, HIGH);

        pinMode(PB15, OUTPUT);
        digitalWrite(PB15, HIGH);
    }

    /**
     * @brief Main indication control loop.
     * 
     * Controls the LEDs based on the global `state`.
     * The LED on PB14 blinks to show that the firmware is running.
     * The LED on PB15 lights up when the machine is ready (state == 0x03).
     */
    static void loop(void)
    {
        // digitalToggle(PB14);
        digitalWrite(PB14, LOW);

        if (state == SenderNS::AutomatState_Ready)
        {
            digitalWrite(PB15, LOW);
        }
        else
        {
            digitalWrite(PB15, HIGH);
        }

        vTaskDelay(INDICATION_TASK_INTERVAL_MS);
    }

    /**
     * @brief FreeRTOS task for managing indication.
     * 
     * @param pvParameters Unused pointer to task parameters.
     */
    void TaskIndication(void *pvParameters __attribute__((unused)))
    {
        init();

        for (;;)
        {
            loop();
        }
    }
};