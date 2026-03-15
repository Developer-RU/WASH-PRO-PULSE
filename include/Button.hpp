/**
 * @file Button.hpp
 * @author Masyukov Pavel
 * @brief Модуль обработки нажатий кнопки для ручного добавления кредитов.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"

namespace Button
{
    /**
     * @brief Инициализация входа кнопки.
     * 
     * Настраивает PB6 как INPUT_PULLDOWN.
     */
    static void init(void);
    
    /**
     * @brief Основной цикл обработки нажатий.
     * 
     * Алгоритм:
     * 1. Детектирование нажатия
     * 2. Антидребезг (50 мс)
     * 3. Добавление 50 кредитов
     * 4. Ожидание отпускания
     */
    static void loop(void);
    
    /**
     * @brief Задача FreeRTOS для обработки кнопки.
     * @param pvParameters Не используется.
     * 
     * Приоритет: 3 (наивысший для быстрой реакции).
     */
    void TaskButton(void *pvParameters __attribute__((unused)));
}
