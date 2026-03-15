/**
 * @file Protocol.cpp
 * @author Masyukov Pavel
 * @brief Protocol packet builder for UART communication.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#include "Protocol.hpp"

namespace Protocol
{
    /**
     * @brief Конструктор PacketBuilder.
     * Инициализирует позицию буфера в 0.
     */
    PacketBuilder::PacketBuilder() : buffer_pos(0) {}

    /**
     * @brief Деструктор PacketBuilder.
     */
    PacketBuilder::~PacketBuilder() {}

    /**
     * @brief Добавление 8-битного значения в пакет (TLV-формат).
     * @param type Тип данных (1 байт).
     * @param value Значение (1 байт).
     * 
     * Формат TLV: [Type: 1 байт] [Length: 1 байт] [Value: N байт]
     */
    void PacketBuilder::add_uint8(uint8_t type, uint8_t value)
    {
        tx_buffer[buffer_pos++] = type;       // Тип
        tx_buffer[buffer_pos++] = 0x01;       // Длина (1 байт)
        tx_buffer[buffer_pos++] = value;      // Значение
    }

    /**
     * @brief Добавление 32-битного значения в пакет (TLV-формат).
     * @param type Тип данных (1 байт).
     * @param value Значение (4 байта).
     * 
     * Формат TLV: [Type: 1 байт] [Length: 4 байта] [Value: 4 байта]
     */
    void PacketBuilder::add_uint32(uint8_t type, uint32_t value)
    {
        uint8_t value_bytes[sizeof(uint32_t)];
        memcpy(value_bytes, &value, sizeof(uint32_t));

        tx_buffer[buffer_pos++] = type;                    // Тип
        tx_buffer[buffer_pos++] = sizeof(uint32_t);        // Длина (4 байта)
        for (uint8_t i = 0; i < sizeof(uint32_t); i++)
        {
            tx_buffer[buffer_pos++] = value_bytes[i];      // Значение (4 байта)
        }
    }

    /**
     * @brief Отправка пакета через UART.
     * @param length Длина данных в буфере.
     * 
     * Формат пакета:
     * [START: 1 байт] [Length: 2 байта] [Data: N байт] [CRC: 1 байт]
     */
    void PacketBuilder::send_packet(uint8_t length)
    {
        Crc crc;
        crc.start();
        
        // Расчёт CRC: START + Length + Data
        crc.add(Start);
        crc.add(length >> 8);      // Старший байт длины
        crc.add(length);           // Младший байт длины
        crc.add(tx_buffer, length); // Данные
        
        // Отправка пакета
        Serial.write(Start);           // Байт старта
        Serial.write(length >> 8);     // Старший байт длины
        Serial.write(length);          // Младший байт длины
        
        // Отправка данных
        for (uint8_t i = 0; i < length; i++)
        {
            Serial.write(tx_buffer[i]);
        }
        
        Serial.write(crc.get());  // Контрольная сумма
    }

    /**
     * @brief Формирование и отправка poll-пакета.
     * 
     * Запрос состояния от контроллера.
     * Содержит: Operation=Poll, DeviceState=Ready.
     */
    void PacketBuilder::build_poll_packet()
    {
        buffer_pos = 0;
        add_uint8(Operation, Poll);       // Операция: опрос
        add_uint8(DeviceState, Ready);    // Состояние: готов
        send_packet(buffer_pos);
    }

    /**
     * @brief Формирование и отправка cash-пакета.
     * @param credits Количество кредитов для передачи.
     * 
     * Передача накопленных кредитов контроллеру.
     * Содержит: Operation=Poll, DeviceState=Ready, CashlessCredit=credits*100.
     */
    void PacketBuilder::build_cash_packet(uint8_t credits)
    {
        buffer_pos = 0;
        add_uint8(Operation, Poll);                  // Операция: опрос
        add_uint8(DeviceState, Ready);               // Состояние: готов
        add_uint32(CashlessCredit, credits * 100);   // Кредиты * 100 (32 бита)
        send_packet(buffer_pos);
    }
}
