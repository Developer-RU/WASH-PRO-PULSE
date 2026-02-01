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
     * Triggers on the falling edge of the signal from the pulse input pin.
     * Increments the `newCount` counter and updates the `timerComplete` timer.
     */
    void buttonPressed()
    {
        newCount++;
        timerComplete = millis();
    }

    /**
     * @brief Initializes the pin for receiving pulses.
     * 
     * Configures pin PA0 as an input with a pull-up resistor and attaches
     * the `buttonPressed` interrupt to it on the falling edge.
     */
    static void init(void)
    {
        pinMode(PULSE_INPUT_PIN, INPUT_PULLUP); // Configure the pulse pin as an input with a pull-up resistor.
        attachInterrupt(PULSE_INPUT_PIN, buttonPressed, FALLING);
    }

    /**
     * @brief Main pulse processing loop.
     * 
     * Checks if there have been no new pulses for 100 ms. If so, it transfers
     * the accumulated value from `newCount` to the global `count` counter.
     */
    static void loop(void)
    {
        if(millis() > timerComplete + PULSE_BURST_TIMEOUT_MS && newCount > 0)
        {
            count = newCount; // Transfer the accumulated value to the global variable.
            newCount = 0;
        }

        vTaskDelay(PULSE_TASK_INTERVAL_MS);
    }

    /**
     * @brief FreeRTOS task for pulse handling.
     * 
     * @param pvParameters Unused pointer to task parameters.
     */
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