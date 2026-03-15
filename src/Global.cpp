/**
 * @file Global.cpp
 * @author Masyukov Pavel
 * @brief Глобальные переменные, общие для всех модулей.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#include "Global.hpp"

/**
 * @brief Счётчик кредитов.
 * 
 * Изменяется:
 * - Pulse: добавление импульсов
 * - Button: добавление по нажатию (+50)
 * - Routine: сброс после отправки контроллеру
 * 
 * Критические секции используются для атомарности.
 */
volatile uint8_t credit_count{0};

/**
 * @brief Текущее состояние устройства.
 * 
 * Значения:
 * - 0x03 (Ready): устройство готово к работе
 * 
 * Изменяется в Routine, читается в Indication.
 */
volatile uint8_t device_state{0};

/**
 * @brief Флаг завершения стартовой задержки.
 * 
 * Первые 3 секунды после включения:
 * - false: импульсы игнорируются, отправка заблокирована
 * 
 * После 3 секунд:
 * - true: нормальная работа
 */
volatile bool is_startup_complete{false};
