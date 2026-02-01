/**
 * @file PacketLayer.hpp
 * @author Masyukov Pavel
 * @brief Header for the PacketLayer class, which handles low-level packet construction and sending.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"
#include "CommandsLayer.hpp"

namespace SenderNS
{
    /** @brief Handles the framing and sending of data packets over UART. */
    class PacketLayer
    {

    public:
    
        /**
         * @brief Constructor for the PacketLayer class.
         */
        PacketLayer(void);
        /**
         * @brief Destructor for the PacketLayer class.
         */
        virtual ~PacketLayer(void);

        /**
         * @brief Sends a data packet framed with a start byte, length, and CRC.
         * @param data Pointer to the data array.
         * @param dataLen Length of the data.
         */
        void sendPacket(uint8_t data[], uint16_t dataLen);
        /**
         * @brief Sends a single control byte (e.g., ACK or NAK).
         * @param byte The control byte.
         */
        void sendControl(uint8_t byte);
        /**
         * @brief Sends raw data without framing.
         * @param data Pointer to the data array.
         * @param dataLen Length of the data.
         */
        void sendData(uint8_t data[], uint16_t dataLen);
    };
}