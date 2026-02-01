/**
 * @file Global.cpp
 * @author Masyukov Pavel
 * @brief Defines global variables used across the project.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "Global.hpp"

uint8_t count{0}; ///< Global credit counter. Modified by Pulse and Button tasks, read by Routine task.
uint8_t state{0}; ///< Global state machine variable. Modified by Routine task, read by Indication task.