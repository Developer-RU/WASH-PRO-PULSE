/**
 * @file Main.hpp
 * @author Masyukov Pavel
 * @brief Main header file for the project. Declares main functions and includes all modules.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"

#include "Button.hpp"
#include "Pulse.hpp"
#include "Indication.hpp"
#include "Routine.hpp"

#include "PacketLayer.hpp"
#include "CommandsLayer.hpp"

// #include "Utils/StringUtils.hpp"
// #include "Lang/Lang.hpp"
// #include "Terminal/Terminal.hpp"
// #include "Core/Core.hpp"
// #include "Acceptor/Acceptor.hpp"
// #include "Storage/NVSStore.hpp"
// #include "Utils/Debuger.hpp"

// #include "Config.hpp"
// #include "Multiport.hpp"
// #include "Http.hpp"
// #include "Firmware.hpp"
// #include "Relay.hpp"
// #include "Display.hpp"
// #include "Totals.hpp"
// #include "Remote.hpp"
// #include "Routine.hpp"
/**
 * @brief Setup function. Initializes FreeRTOS tasks.
 */
void setup(void);

/**
 * @brief Main loop. Not used in this project as all logic is handled by FreeRTOS tasks.
 */
void loop(void);
