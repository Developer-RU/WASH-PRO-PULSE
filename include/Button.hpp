/**
 * @file Button.hpp
 * @author Masyukov Pavel
 * @brief Header file for the button handling module.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"

/**
 * @namespace ButtonNS
 * @brief Namespace to encapsulate button handling logic.
 */
namespace ButtonNS
{
    /**
     * @brief Initializes the button input pins.
     */
    static void init(void);
    /**
     * @brief Loop function that checks for button presses.
     */
    static void loop(void);

    /**
     * @brief FreeRTOS task for handling button presses.
     * @param pvParameters Unused pointer to task parameters.
     */
    void TaskButton(void *pvParameters __attribute__((unused)));
};