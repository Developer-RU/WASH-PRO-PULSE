/**
 * @file Routine.cpp
 * @author Masyukov Pavel
 * @brief Main communication task with the master controller.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#include "Routine.hpp"

// Глобальные переменные из других модулей
extern volatile uint8_t credit_count;       ///< Счётчик кредитов (импульсы + кнопка)
extern volatile uint8_t device_state;       ///< Текущее состояние устройства
extern volatile bool is_startup_complete;   ///< Флаг завершения стартовой задержки

namespace Routine
{
    // Объекты и переменные модуля
    Protocol::PacketBuilder packet_builder;  ///< Генератор протокольных пакетов
    Crc crc;                                  ///< Калькулятор CRC

    // Переменные состояния
    uint8_t is_sending{0};           ///< Флаг активной отправки пакета
    uint32_t retry_time{0};          ///< Время последней попытки отправки
    uint32_t timeout_time{0};        ///< Время таймаута отправки
    uint32_t startup_start_time{0};  ///< Время старта устройства
    uint8_t last_sent_count{0};      ///< Последнее отправленное значение

    // Буфер приёма
    uint8_t rx_buffer[PROTOCOL_BUFFER_SIZE];  ///< Буфер приёма данных
    uint16_t rx_bytes = 0;                    ///< Количество принятых байт
    uint16_t rx_data_length = 0;              ///< Длина данных в пакете

    /**
     * @brief Обработка входящих байтов UART.
     * @return true если получен ACK, false иначе.
     */
    static bool process_rx_bytes()
    {
        vTaskDelay(5 / portTICK_PERIOD_MS);

        while (Serial.available() > 0)
        {
            uint8_t byte = (uint8_t)Serial.read();

            // Обработка управляющих байтов (ACK/NAK)
            if (rx_bytes == 0 && (byte == Protocol::Ack || byte == Protocol::Nak))
            {
                if (byte == Protocol::Ack)
                {
                    // Контроллер подтвердил получение — сбрасываем флаги
                    last_sent_count = 0;
                    is_sending = 0;
                    retry_time = 0;
                    timeout_time = 0;
                    return true;
                }
                return false;  // NAK игнорируем
            }
            else
            {
                // Накопление байт пакета
                rx_buffer[rx_bytes++] = byte;
            }

            vTaskDelay(2 / portTICK_PERIOD_MS);
        }
        return false;
    }

    /**
     * @brief Проверка CRC и отправка ответа.
     */
    static void verify_and_respond()
    {
        if (rx_bytes <= 1) return;

        // Извлекаем длину данных из пакета (байты 1-2)
        rx_data_length = (rx_buffer[1] << 8) + rx_buffer[2];

        // Расчёт CRC принятых данных
        crc.start();
        for (uint16_t i = 0; i < rx_data_length + 3; i++)
        {
            crc.add(rx_buffer[i]);
        }

        // Проверка и ответ
        if (rx_buffer[rx_bytes - 1] == crc.get())
        {
            Serial.write(Protocol::Ack);  // CRC верен
        }
        else
        {
            Serial.write(Protocol::Nak);  // CRC неверен
        }

        // Очистка буфера
        for (uint16_t i = 0; i < PROTOCOL_BUFFER_SIZE; i++)
            rx_buffer[i] = 0x00;

        rx_bytes = 0;
        rx_data_length = 0;
    }

    /**
     * @brief Отправка пакета с кредитами.
     * @param count Количество кредитов.
     */
    static void send_credit_packet(uint8_t count)
    {
        packet_builder.build_cash_packet(count);
        last_sent_count = count;
        credit_count = 0;
        timeout_time = retry_time = millis();
        is_sending = 1;
    }

    /**
     * @brief Отправка poll-пакета.
     */
    static void send_poll_packet()
    {
        packet_builder.build_poll_packet();
        timeout_time = retry_time = millis();
        is_sending = 1;
    }

    /**
     * @brief Повторная отправка пакета.
     */
    static void retry_send()
    {
        if (credit_count > 0 && credit_count != last_sent_count)
        {
            send_credit_packet(credit_count);
        }
        else
        {
            send_poll_packet();
        }
    }

    /**
     * @brief Инициализация модуля связи.
     * 
     * Настраивает UART (PA9/PA10), устанавливает состояние Ready,
     * запоминает время старта для 3-секундной задержки.
     */
    static void init(void)
    {
        // Устанавливаем состояние устройства в Ready
        device_state = Protocol::Ready;

        // Настройка UART1: RX=PA10, TX=PA9, скорость 115200
        Serial.setRx(PA10);
        Serial.setTx(PA9);
        Serial.begin(115200);

        // Запоминаем время старта для отсчёта 3 секунд
        startup_start_time = millis();
        is_startup_complete = false;
        credit_count = 0;

        // Даём время на стабилизацию UART
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    /**
     * @brief Основной цикл обработки связи.
     */
    static void loop(void)
    {
        // === Обработка входящих данных ===
        if (Serial.available() > 0)
        {
            process_rx_bytes();
            verify_and_respond();
        }
        else
        {
            // === Отправка пакетов ===

            // Проверка завершения стартовой задержки (3 секунды)
            if (!is_startup_complete && millis() - startup_start_time >= 3000)
            {
                is_startup_complete = true;
            }

            // Отправка только после завершения стартового периода
            if (is_sending == 0 && is_startup_complete)
            {
                // Есть новые кредиты — отправляем cash-пакет немедленно
                if (credit_count > 0 && credit_count != last_sent_count)
                {
                    send_credit_packet(credit_count);
                }
                // Нет кредитов — отправляем poll-пакет раз в 3 секунды
                else if (millis() > timeout_time + 3000 || timeout_time == 0)
                {
                    send_poll_packet();
                }
            }
            else
            {
                // Повторная отправка при отсутствии ответа (через 1 сек, макс. 3 сек)
                if (is_startup_complete && millis() > retry_time + 1000 && millis() < timeout_time + 3000)
                {
                    retry_send();
                }

                // Таймаут — сброс и новая попытка
                if (millis() > timeout_time + 3000)
                {
                    is_sending = 0;
                    retry_time = 0;
                    timeout_time = millis();
                }
            }
        }

        vTaskDelay(20 / portTICK_PERIOD_MS);
    }

    /**
     * @brief Задача FreeRTOS для обработки связи.
     * @param pvParameters Не используется.
     */
    void TaskRoutine(void *pvParameters __attribute__((unused)))
    {
        init();
        for (;;)
        {
            loop();
        }
    }
}
