/**
 * @file Pulse.cpp
 * @author Masyukov Pavel
 * @brief Module for counting pulses from an external device.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#include "Pulse.hpp"

// Глобальные переменные из других модулей
extern volatile uint8_t credit_count;       ///< Счётчик кредитов (импульсы + кнопка)
extern volatile bool is_startup_complete;   ///< Флаг завершения стартовой задержки

namespace Pulse
{
    // Переменные модуля
    uint32_t pulse_burst_end_time = 0;   ///< Время окончания пачки импульсов
    volatile uint8_t pulse_buffer = 0;   ///< Буфер накопления импульсов в прерывании
    uint32_t last_pulse_time = 0;        ///< Время последнего импульса (для защиты от дребезга)

    /**
     * @brief Прерывание по входу импульсов (PA0).
     * 
     * Срабатывает по спадающему фронту сигнала.
     * 
     * Защита от дребезга: минимальный интервал между импульсами 10 мс.
     * Ограничение: максимум 250 импульсов в пачке (защита от переполнения).
     * Игнорирование во время стартовой задержки (первые 3 секунды).
     */
    void pulse_isr()
    {
        // Игнорирование импульсов во время стартовой задержки (3 секунды)
        if (!is_startup_complete)
        {
            return;
        }

        uint32_t now = millis();

        // Защита от дребезга: минимальный интервал 10 мс между импульсами
        if (now - last_pulse_time < 10)
        {
            return;
        }
        last_pulse_time = now;

        // Ограничение максимального количества импульсов в пачке (250)
        if (pulse_buffer < 250)
        {
            pulse_buffer++;
        }
        
        // Обновление времени окончания пачки
        pulse_burst_end_time = millis();
    }

    /**
     * @brief Инициализация входа импульсов.
     * 
     * Настройка PA0 как вход с плавающим состоянием (INPUT_FLOATING).
     * Подключение прерывания pulse_isr() по спадающему фронту.
     */
    static void init(void)
    {
        pinMode(PA0, INPUT_FLOATING);
        attachInterrupt(PA0, pulse_isr, FALLING);
    }

    /**
     * @brief Обработка пачки импульсов.
     * 
     * Если прошло 100 мс без новых импульсов — пачка завершена.
     * Перенос накопленных импульсов в глобальный счётчик credit_count.
     * Используется критическая секция для атомарности операции.
     */
    static void loop(void)
    {
        // Проверка окончания пачки импульсов (таймаут 100 мс)
        if (millis() > pulse_burst_end_time + PULSE_BURST_TIMEOUT_MS && pulse_buffer > 0)
        {
            // Критическая секция: запрет прерываний на время копирования
            noInterrupts();
            credit_count += pulse_buffer;  // Добавление к глобальному счётчику
            pulse_buffer = 0;               // Очистка буфера
            interrupts();                   // Разрешение прерываний
        }

        vTaskDelay(PULSE_TASK_INTERVAL_MS);
    }

    /**
     * @brief Задача FreeRTOS для обработки импульсов.
     * @param pvParameters Не используется.
     */
    void TaskPulse(void *pvParameters __attribute__((unused)))
    {
        init();
        pulse_burst_end_time = millis();

        for (;;)
        {
            loop();
        }
    }
}
