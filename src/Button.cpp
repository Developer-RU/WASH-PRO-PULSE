/**
 * @file Button.cpp
 * @author Masyukov Pavel
 * @brief Module for manual credit addition via button.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#include "Button.hpp"

// Глобальные переменные из других модулей
extern volatile uint8_t credit_count;  ///< Счётчик кредитов (импульсы + кнопка)

namespace Button
{
    // Переменные модуля
    uint8_t button_state{0};       ///< Текущее состояние кнопки
    uint8_t button_flag{0};        ///< Флаг нажатия кнопки
    uint32_t last_press_time = 0;  ///< Время последнего нажатия (для антидребезга)

    /**
     * @brief Инициализация входа кнопки.
     * 
     * Настройка PB6 как вход с подтяжкой к земле (INPUT_PULLDOWN).
     * Кнопка подключается между PB6 и +3.3V.
     */
    static void init(void)
    {
        pinMode(PB6, INPUT_PULLDOWN);
    }

    /**
     * @brief Обработка нажатий кнопки.
     * 
     * Алгоритм:
     * 1. Детектирование фронта (кнопка нажата, флаг = 0)
     * 2. Задержка 50 мс для антидребезга
     * 3. Подтверждение нажатия: добавление 50 кредитов
     * 4. Ожидание отпускания кнопки
     * 5. Сброс состояния
     * 
     * Критическая секция используется для атомарного сложения кредитов.
     */
    static void loop(void)
    {
        // Шаг 1: Детектирование нажатия (кнопка нажата, флаг ещё не установлен)
        if (digitalRead(PB6) == HIGH && button_flag == 0)
        {
            button_flag = 1;
            last_press_time = millis();
        }

        // Шаг 2-3: Подтверждение нажатия после задержки 50 мс
        if (button_flag == 1 && millis() - last_press_time > 50)
        {
            if (button_state == 0)
            {
                button_state = 1;

                // Критическая секция: атомарное добавление кредитов
                noInterrupts();
                credit_count += 50;  // Добавление 50 кредитов
                interrupts();
            }
            button_flag = 2;  // Шаг 4: ожидание отпускания
        }

        // Шаг 5: Сброс при отпускании кнопки
        if (digitalRead(PB6) == LOW && button_flag == 2)
        {
            button_state = 0;
            button_flag = 0;
        }

        vTaskDelay(10);  // Задержка для стабильности
    }

    /**
     * @brief Задача FreeRTOS для обработки кнопки.
     * @param pvParameters Не используется.
     */
    void TaskButton(void *pvParameters __attribute__((unused)))
    {
        init();
        for (;;)
        {
            loop();
        }
    }
}
