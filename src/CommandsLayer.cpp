#include "CommandsLayer.hpp"

namespace SenderNS
{
    CommandLayer::CommandLayer(void) {}
    CommandLayer::~CommandLayer(void) {}

    void CommandLayer::addUInt8(uint8_t Type_Command, uint8_t Data_Command)
    {
        PacketData[posElementRes++] = Type_Command;
        PacketData[posElementRes++] = 0x01;
        PacketData[posElementRes++] = Data_Command;
    }

    void CommandLayer::addUInt32(uint8_t Type_Command, uint32_t Data_Command)
    {
        char float_bytes[sizeof(float)];
        memcpy(float_bytes, &Data_Command, sizeof(float));

        PacketData[posElementRes++] = Type_Command;
        PacketData[posElementRes++] = sizeof(float);
        for (uint8_t i = 0; i < sizeof(float); i++)
            PacketData[posElementRes++] = float_bytes[i];
    }

    void CommandLayer::setupPacket(void)
    {
        PacketLayer packetLayer;
        posElementRes = 0;

        addUInt8(Type_Operation, Operation_SetupRequest);

        packetLayer.sendPacket(PacketData, posElementRes);
    }

    void CommandLayer::pollPacket(void)
    {
        PacketLayer packetLayer;
        posElementRes = 0;

        addUInt8(Type_Operation, Operation_Poll);
        addUInt8(Type_AutomatState, AutomatState_Ready);
        addUInt8(AutomatState_Init, AutomatState_Ready);

        packetLayer.sendPacket(PacketData, posElementRes);
    }

    void CommandLayer::cashPacket(uint8_t count)
    {
        PacketLayer packetLayer;
        posElementRes = 0;

        addUInt8(Type_Operation, Operation_Poll);
        addUInt8(Type_AutomatState, AutomatState_Ready);
        addUInt32(Type_CashlessCredit, count * 100);

        packetLayer.sendPacket(PacketData, posElementRes);
    }
}