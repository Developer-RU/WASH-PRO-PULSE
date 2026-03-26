/**
 * @file Routine.cpp
 * @author Masyukov Pavel
 * @brief Main communication task with the master controller.
 * @version 2.4.0
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
    uint32_t timeout_time{0};        ///< Время начала серии отправок (таймаут отсюда)
    uint32_t startup_start_time{0};  ///< Время старта устройства
    uint8_t last_sent_count{0};      ///< Последнее отправленное значение

    // Буфер приёма
    uint8_t rx_buffer[PROTOCOL_BUFFER_SIZE];  ///< Буфер приёма данных
    uint16_t rx_bytes = 0;                    ///< Количество принятых байт
    uint16_t rx_data_length = 0;              ///< Длина данных в пакете
    uint32_t last_rx_byte_time = 0;           ///< Время последнего принятого байта
    const uint32_t RX_TIMEOUT = 50;           ///< Таймаут приёма пакета (мс)

    // Переменные для контроля связи
    uint32_t last_activity_time = 0;          ///< Время последней успешной активности (ACK, NAK, любой байт)
    uint32_t last_reset_time = 0;             ///< Время последнего сброса UART
    const uint32_t IDLE_TIMEOUT = 10000;      ///< Таймаут бездействия 10 секунд
    const uint32_t MIN_RESET_INTERVAL = 5000; ///< Минимальный интервал между перезапусками UART (5 сек)

    /**
     * @brief Очистка буфера приёма.
     */
    static void clear_rx_buffer()
    {
        rx_bytes = 0;
        rx_data_length = 0;
        memset(rx_buffer, 0, PROTOCOL_BUFFER_SIZE);
        last_rx_byte_time = 0;
    }

    /**
     * @brief Логический сброс состояния отправки (без перезапуска UART).
     */
    static void reset_sending_state()
    {
        is_sending = 0;
        retry_time = 0;
        timeout_time = 0;
        last_sent_count = 0;
        clear_rx_buffer();
        last_activity_time = millis();
    }

    /**
     * @brief Полный сброс связи (логика + переинициализация UART) без потери кредитов.
     */
    static void reset_communication()
    {
        uint32_t now = millis();
        // Не выполняем частые сбросы
        if (now - last_reset_time < MIN_RESET_INTERVAL && last_reset_time != 0) {
            // Только логический сброс, UART не трогаем
            reset_sending_state();
            return;
        }

        // Логический сброс
        is_sending = 0;
        retry_time = 0;
        timeout_time = 0;
        last_sent_count = 0;
        clear_rx_buffer();

        // Физический перезапуск UART
        Serial.end();
        vTaskDelay(10 / portTICK_PERIOD_MS);
        Serial.begin(115200);
        vTaskDelay(50 / portTICK_PERIOD_MS);

        last_reset_time = now;
        last_activity_time = now;

        // После сброса сразу отправляем poll-пакет
        send_poll_packet(false);
    }

    /**
     * @brief Обработка входящих байтов UART.
     * @return true если получен ACK, false иначе.
     */
    static bool process_rx_bytes()
    {
        vTaskDelay(5 / portTICK_PERIOD_MS);
        bool received = false;

        while (Serial.available() > 0)
        {
            uint8_t byte = (uint8_t)Serial.read();
            last_rx_byte_time = millis();
            last_activity_time = millis();   // любой байт считается активностью
            received = true;

            // Обработка управляющих байтов (ACK/NAK)
            if (rx_bytes == 0 && (byte == Protocol::Ack || byte == Protocol::Nak))
            {
                if (byte == Protocol::Ack)
                {
                    reset_sending_state();    // ACK – полный сброс
                    return true;
                }
                else // NAK
                {
                    // NAK – пакет не прошёл CRC, но связь есть
                    // Очищаем буфер, но состояние отправки не сбрасываем (продолжаем повторять)
                    clear_rx_buffer();
                    return false;
                }
            }
            else
            {
                // Накопление байт пакета
                if (rx_bytes < PROTOCOL_BUFFER_SIZE)
                {
                    rx_buffer[rx_bytes++] = byte;
                }
                else
                {
                    clear_rx_buffer(); // переполнение
                }
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

        // Проверка минимальной длины пакета
        if (rx_data_length + 4 > PROTOCOL_BUFFER_SIZE)
        {
            clear_rx_buffer();
            return;
        }

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
            reset_sending_state();        // получен корректный пакет – связь есть
        }
        else
        {
            Serial.write(Protocol::Nak);  // CRC неверен
            // Не сбрасываем состояние, но буфер очищаем
        }

        clear_rx_buffer();
    }

    /**
     * @brief Отправка пакета с кредитами.
     * @param count Количество кредитов.
     * @param is_retry true – повторная отправка (не обновлять timeout_time)
     */
    static void send_credit_packet(uint8_t count, bool is_retry = false)
    {
        packet_builder.build_cash_packet(count);
        last_sent_count = count;
        credit_count = 0;
        if (!is_retry) {
            timeout_time = retry_time = millis();
        } else {
            retry_time = millis();
        }
        is_sending = 1;
        last_activity_time = millis();
    }

    /**
     * @brief Отправка poll-пакета.
     * @param is_retry true – повторная отправка (не обновлять timeout_time)
     */
    static void send_poll_packet(bool is_retry = false)
    {
        packet_builder.build_poll_packet();
        if (!is_retry) {
            timeout_time = retry_time = millis();
        } else {
            retry_time = millis();
        }
        is_sending = 1;
        last_activity_time = millis();
    }

    /**
     * @brief Повторная отправка текущего пакета.
     */
    static void retry_send()
    {
        if (credit_count > 0 && credit_count != last_sent_count)
        {
            send_credit_packet(credit_count, true);
        }
        else
        {
            send_poll_packet(true);
        }
    }

    /**
     * @brief Инициализация модуля связи.
     */
    static void init(void)
    {
        device_state = Protocol::Ready;

        Serial.setRx(PA10);
        Serial.setTx(PA9);
        Serial.begin(115200);

        startup_start_time = millis();
        is_startup_complete = false;
        credit_count = 0;

        reset_sending_state();
        last_reset_time = 0;

        vTaskDelay(1000 / portTICK_PERIOD_MS);
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
            // Если в буфере есть неполный пакет и прошло больше RX_TIMEOUT – очищаем
            if (rx_bytes > 0 && (millis() - last_rx_byte_time) > RX_TIMEOUT)
            {
                clear_rx_buffer();
            }

            // === Отправка пакетов ===

            // Проверка завершения стартовой задержки (3 секунды)
            if (!is_startup_complete && millis() - startup_start_time >= 3000)
            {
                is_startup_complete = true;
            }

            // Если нет никакой активности (ACK, NAK, любой байт) более IDLE_TIMEOUT – сбрасываем связь
            if (is_startup_complete && (millis() - last_activity_time) > IDLE_TIMEOUT)
            {
                reset_communication(); // полный сброс (включая UART)
                vTaskDelay(20 / portTICK_PERIOD_MS);
                return;
            }

            // Отправка только после завершения стартового периода
            if (is_sending == 0 && is_startup_complete)
            {
                // Есть новые кредиты – отправляем cash-пакет немедленно
                if (credit_count > 0 && credit_count != last_sent_count)
                {
                    send_credit_packet(credit_count, false);
                }
                // Нет кредитов – отправляем poll-пакет раз в 3 секунды
                else if (millis() > timeout_time + 3000 || timeout_time == 0)
                {
                    send_poll_packet(false);
                }
            }
            else if (is_startup_complete)
            {
                // Повторная отправка при отсутствии ответа (через 1 сек)
                if (millis() > retry_time + 1000 && millis() < timeout_time + 3000)
                {
                    retry_send();
                }

                // Таймаут отправки (3 секунды без ACK)
                if (millis() > timeout_time + 3000)
                {
                    // Сбрасываем состояние и сразу отправляем новый пакет
                    reset_sending_state();
                    // Отправляем poll-пакет (или cash, если есть кредиты)
                    if (credit_count > 0)
                        send_credit_packet(credit_count, false);
                    else
                        send_poll_packet(false);
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