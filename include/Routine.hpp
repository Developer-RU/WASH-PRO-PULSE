/**
 * @file Routine.hpp
 * @author Masyukov Pavel
 * @brief Модуль связи с мастер-контроллером.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"
#include "Protocol.hpp"

extern volatile uint8_t device_state;       ///< Текущее состояние устройства.
extern volatile bool is_startup_complete;   ///< Флаг завершения стартовой задержки (3 сек).

namespace Routine
{
    /**
     * @brief Инициализация модуля связи.
     * 
     * - Настройка UART1 (PA9/PA10, 115200)
     * - Установка состояния Ready
     * - Запуск таймера стартовой задержки (3 сек)
     */
    static void init(void);
    
    /**
     * @brief Основной цикл обработки связи.
     * 
     * - Приём данных от контроллера
     * - Проверка CRC, отправка ACK/NAK
     * - Отправка poll-пакетов (раз в 3 сек)
     * - Отправка cash-пакетов (при наличии кредитов)
     */
    static void loop(void);
    
    /**
     * @brief Задача FreeRTOS для обработки связи.
     * @param pvParameters Не используется.
     * 
     * Приоритет: 1 (низкий).
     * Размер стека: 1024 слова (для UART буферов).
     */
    void TaskRoutine(void *pvParameters __attribute__((unused)));
}
