/**
 * @file Pulse.hpp
 * @author Masyukov Pavel
 * @brief Header file for the pulse handling module.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"

/**
 * @namespace PulseNS
 * @brief Namespace to encapsulate pulse handling logic.
 */
namespace PulseNS
{
    /**
     * @brief Initializes the pin for reading pulses and sets up the interrupt.
     */
    static void init(void);
    /**
     * @brief Loop function that checks for the end of a pulse burst.
     */
    static void loop(void);

    /**
     * @brief FreeRTOS task for handling pulses.
     * @param pvParameters Unused pointer to task parameters.
     */
    void TaskPulse(void *pvParameters __attribute__((unused)));
};