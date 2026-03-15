/**
 * @file Indication.hpp
 * @author Masyukov Pavel
 * @brief Модуль управления светодиодной индикацией.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"
#include "Protocol.hpp"

extern volatile uint8_t device_state;       ///< Текущее состояние устройства.
extern volatile bool is_startup_complete;   ///< Флаг завершения стартовой задержки (3 сек).

namespace Indication
{
    /**
     * @brief Инициализация выходов светодиодов.
     * 
     * PB14 (LED_HEARTBEAT): индикатор обмена данными.
     * PB15 (LED_STATUS): индикатор состояния устройства.
     */
    static void init(void);
    
    /**
     * @brief Управление светодиодами.
     * 
     * PB15 (статусный):
     * - 0-3 сек: быстрое мигание (100 мс)
     * - После 3 сек: медленное мигание (1000 мс)
     * 
     * PB14 (данные):
     * - Включается при новых кредитах
     * - Выключается после отправки
     */
    static void loop(void);
    
    /**
     * @brief Задача FreeRTOS для управления индикацией.
     * @param pvParameters Не используется.
     * 
     * Приоритет: 2 (средний).
     */
    void TaskIndication(void *pvParameters __attribute__((unused)));
}
