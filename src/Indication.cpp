/**
 * @file Indication.cpp
 * @author Masyukov Pavel
 * @brief Модуль светодиодной индикации состояния устройства.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#include "Indication.hpp"

// Глобальные переменные
extern volatile uint8_t device_state;       ///< Состояние устройства
extern volatile uint8_t credit_count;       ///< Счётчик кредитов
extern volatile bool is_startup_complete;   ///< Флаг завершения стартовой задержки

namespace Indication
{
    // Переменные модуля
    uint32_t last_status_blink_time = 0;    ///< Время последнего переключения статусного LED
    bool status_led_state = false;          ///< Текущее состояние статусного LED
    
    uint32_t last_data_led_time = 0;        ///< Время последнего изменения LED данных
    uint8_t last_credit_count = 0;          ///< Последнее обработанное значение кредитов

    /**
     * @brief Инициализация выходов светодиодов.
     * 
     * PB14 (LED_HEARTBEAT): индикатор обмена данными
     * PB15 (LED_STATUS): индикатор состояния устройства
     * 
     * Активный уровень: LOW (включён)
     */
    static void init(void)
    {
        pinMode(PB14, OUTPUT);
        digitalWrite(PB14, HIGH);  // Выключен

        pinMode(PB15, OUTPUT);
        digitalWrite(PB15, HIGH);  // Выключен
    }

    /**
     * @brief Обновление статусного светодиода (PB15).
     * 
     * Режимы:
     * - Стартовая задержка (0-3 сек): быстрое мигание (100 мс)
     * - Рабочий режим: медленное мигание (1000 мс)
     */
    static void update_status_led()
    {
        const uint32_t blink_interval = is_startup_complete ? 1000 : 100;
        
        if (millis() - last_status_blink_time >= blink_interval)
        {
            last_status_blink_time = millis();
            status_led_state = !status_led_state;
            digitalWrite(PB15, status_led_state ? LOW : HIGH);
        }
    }

    /**
     * @brief Обновление LED обмена данными (PB14).
     * 
     * Логика:
     * - Включается при появлении новых кредитов
     * - Выключается после отправки (credit_count == 0)
     */
    static void update_data_led()
    {
        if (is_startup_complete && credit_count > 0 && credit_count != last_credit_count)
        {
            // Новые кредиты — включаем LED
            digitalWrite(PB14, LOW);
            last_credit_count = credit_count;
            last_data_led_time = millis();
        }
        else if (is_startup_complete && (credit_count == 0 || credit_count == last_credit_count))
        {
            // Данные отправлены или нет изменений — выключаем LED
            digitalWrite(PB14, HIGH);
        }
    }

    /**
     * @brief Основной цикл индикации.
     */
    static void loop(void)
    {
        update_status_led();
        update_data_led();
        
        vTaskDelay(50);  // Задержка для стабильности
    }

    /**
     * @brief Задача FreeRTOS для управления индикацией.
     * @param pvParameters Не используется.
     */
    void TaskIndication(void *pvParameters __attribute__((unused)))
    {
        init();
        for (;;)
        {
            loop();
        }
    }
}
