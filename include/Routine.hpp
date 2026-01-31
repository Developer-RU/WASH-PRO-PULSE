/**
 * @file Routine.hpp
 * @author Masyukov Pavel
 * @brief Header file for the main routine and communication task.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"
#include "CommandsLayer.hpp"

/**
 * @namespace RoutineNS
 * @brief Namespace to encapsulate the main application logic and communication protocol.
 */
namespace RoutineNS
{
    /**
     * @brief Initializes the routine task, including the serial port.
     */
    static void init(void);
    /**
     * @brief Loop function that handles sending and receiving data packets.
     */
    static void loop(void);

    /**
     * @brief FreeRTOS task for the main routine.
     * @param pvParameters Unused pointer to task parameters.
     */
    void TaskRoutine(void *pvParameters __attribute__((unused)));
}
