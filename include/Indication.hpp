/**
 * @file Indication.hpp
 * @author Masyukov Pavel
 * @brief Header file for the LED indication module.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"
#include "CommandsLayer.hpp"

/**
 * @namespace IndicationNS
 * @brief Namespace to encapsulate LED indication logic.
 */
namespace IndicationNS
{
    /**
     * @brief Initializes the LED output pins.
     */
    static void init(void);
    /**
     * @brief Loop function that controls the LED states.
     */
    static void loop(void);

    /**
     * @brief FreeRTOS task for managing LED indication.
     * @param pvParameters Unused pointer to task parameters.
     */
    void TaskIndication(void *pvParameters __attribute__((unused)));
};