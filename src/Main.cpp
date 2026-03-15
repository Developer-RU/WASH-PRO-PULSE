/**
 * @file Main.cpp
 * @author Masyukov Pavel
 * @brief Program entry point. Creates and starts all FreeRTOS tasks.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#include "Main.hpp"

/**
 * @brief Настройка и запуск системы.
 * 
 * Создаёт задачи FreeRTOS с различными приоритетами:
 * - Indication (приоритет 2): управление светодиодами
 * - Pulse (приоритет 1): подсчёт импульсов
 * - Button (приоритет 3): обработка кнопки (наивысший приоритет)
 * - Routine (приоритет 1): связь с контроллером
 * 
 * После vTaskStartScheduler() управление передаётся планировщику FreeRTOS.
 */
void setup()
{
    delay(500);  // Задержка перед запуском

    // Создание задач FreeRTOS
    xTaskCreate(
        Indication::TaskIndication,
        (const portCHAR *)"Indication",
        256,           // Размер стека (слова)
        NULL,          // Параметры
        2,             // Приоритет
        NULL           // Дескриптор задачи
    );

    xTaskCreate(
        Pulse::TaskPulse,
        (const portCHAR *)"Pulse",
        256,
        NULL,
        1,             // Низкий приоритет
        NULL
    );

    xTaskCreate(
        Button::TaskButton,
        (const portCHAR *)"Button",
        256,
        NULL,
        3,             // Наивысший приоритет (быстрая реакция)
        NULL
    );

    xTaskCreate(
        Routine::TaskRoutine,
        (const portCHAR *)"Routine",
        1024,          // Большой стек для UART
        NULL,
        1,             // Низкий приоритет
        NULL
    );

    // Запуск планировщика FreeRTOS
    vTaskStartScheduler();
}

/**
 * @brief Основной цикл Arduino.
 * 
 * Не используется — все задачи выполняются в FreeRTOS.
 * После vTaskStartScheduler() управление передано планировщику.
 */
void loop()
{
    // Пусто — задачи выполняются в FreeRTOS
}
