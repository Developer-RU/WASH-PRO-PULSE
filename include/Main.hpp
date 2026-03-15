/**
 * @file Main.hpp
 * @author Masyukov Pavel
 * @brief Главный заголовочный файл проекта.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"

// Подключение модулей
#include "Button.hpp"
#include "Pulse.hpp"
#include "Indication.hpp"
#include "Routine.hpp"

// Протокол
#include "Protocol.hpp"

// Точки входа
void setup(void);
void loop(void);
