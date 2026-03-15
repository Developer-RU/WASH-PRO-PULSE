/**
 * @file Protocol.hpp
 * @author Masyukov Pavel
 * @brief Protocol packet definitions and builder class.
 * @version 2.0.0
 * @see https://github.com/Developer-RU/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"

#define PROTOCOL_BUFFER_SIZE 64  ///< Максимальный размер буфера пакета

namespace Protocol
{
    /**
     * @brief Управляющие байты протокола.
     */
    enum ControlByte
    {
        Start = 0x01,  ///< Байт начала пакета
        Ack   = 0x06,  ///< Подтверждение получения
        Nak   = 0x15,  ///< Отрицательное подтверждение
    };

    /**
     * @brief Типы данных в TLV-формате.
     */
    enum DataType
    {
        Operation         = 0x01,  ///< Тип операции
        DeviceState       = 0x08,  ///< Состояние устройства
        CashlessCredit    = 0x0F,  ///< Безналичные кредиты (оригинальное имя)
    };

    /**
     * @brief Типы операций.
     */
    enum OperationType
    {
        Poll = 0x03,  ///< Опрос состояния
    };

    /**
     * @brief Состояния устройства.
     */
    enum DeviceStatus
    {
        Ready = 0x03,  ///< Устройство готово к работе
    };

    /**
     * @brief Класс формирования и отправки протокольных пакетов.
     *
     * Формирует пакеты в формате:
     * [START] [Length: 2 байта] [TLV Data] [CRC]
     */
    class PacketBuilder
    {
    public:
        uint8_t tx_buffer[PROTOCOL_BUFFER_SIZE];  ///< Буфер передачи
        uint8_t buffer_pos;                        ///< Текущая позиция в буфере

        PacketBuilder();
        ~PacketBuilder();

        void build_poll_packet();                 ///< Построить poll-пакет
        void build_cash_packet(uint8_t credits);  ///< Построить cash-пакет

    private:
        void add_uint8(uint8_t type, uint8_t value);    ///< Добавить 8-битный TLV-элемент
        void add_uint32(uint8_t type, uint32_t value);  ///< Добавить 32-битный TLV-элемент
        void send_packet(uint8_t length);               ///< Отправить пакет
    };
}
