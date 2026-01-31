/**
 * @file Pulse.cpp
 * @author Masyukov Pavel
 * @brief Implementation of the module for counting pulses from an external device.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "Pulse.hpp"

extern uint8_t count; ///< Global variable to store the total number of pulses.

namespace PulseNS
{
    unsigned long timerComplete = 0; ///< Timer to detect the end of a pulse burst.
    uint8_t newCount = 0;            ///< Temporary counter to accumulate pulses in the interrupt.

    /**
     * @brief Interrupt Service Routine (ISR) for pin PA0.
     * 
     * Triggers on the falling edge of the signal.
     * Increments the `newCount` counter and updates the `timerComplete` timer.
     */
    void buttonPressed()
    {
        newCount++;
        timerComplete = millis();
    }

    /**
     * @internal Initializes the pulse input pin.
     */
    static void init(void)
    {
        pinMode(PA0, INPUT_PULLUP); // Configure PA0 as an input with a pull-up resistor.
        attachInterrupt(PA0, buttonPressed, FALLING);
    }

    static void loop(void)
    {
        if(millis() > timerComplete + 100 && newCount > 0)
        {
            count = newCount; // Transfer the accumulated value to the global variable.
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