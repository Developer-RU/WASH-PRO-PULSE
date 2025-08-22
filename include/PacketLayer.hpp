#pragma once

#include "Global.hpp"
#include "CommandsLayer.hpp"

namespace SenderNS
{
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