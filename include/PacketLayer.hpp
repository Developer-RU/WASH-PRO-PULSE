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
    
        PacketLayer(void);
        virtual ~PacketLayer(void);

        void sendPacket(uint8_t data[], uint16_t dataLen);
        void sendControl(uint8_t byte);
        void sendData(uint8_t data[], uint16_t dataLen);
    };
}