/**
 * @file Pulse.hpp
 * @author Masyukov Pavel
 * @brief Модуль подсчёта импульсов от внешнего устройства.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"

namespace Pulse
{
    /**
     * @brief Инициализация входа импульсов.
     * 
     * Настраивает PA0 как INPUT_FLOATING с прерыванием по спадающему фронту.
     */
    static void init(void);
    
    /**
     * @brief Основной цикл обработки пачек импульсов.
     * 
     * Если прошло 100 мс без новых импульсов — переносит накопленные
     * импульсы в глобальный счётчик credit_count.
     */
    static void loop(void);
    
    /**
     * @brief Задача FreeRTOS для подсчёта импульсов.
     * @param pvParameters Не используется.
     * 
     * Приоритет: 1 (низкий).
     */
    void TaskPulse(void *pvParameters __attribute__((unused)));
}
