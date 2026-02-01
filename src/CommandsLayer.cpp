/**
 * @file CommandsLayer.cpp
 * @author Masyukov Pavel
 * @brief Implementation of the CommandLayer class for building specific command packets.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "CommandsLayer.hpp"

namespace SenderNS
{
    CommandLayer::CommandLayer(void) {}
    CommandLayer::~CommandLayer(void) {}

    /**
     * @brief Adds a TLV (Type-Length-Value) element for an 8-bit value to the data packet.
     * 
     * @param Type_Command The command/data type.
     * @param Data_Command The 8-bit value.
     */
    void CommandLayer::addUInt8(uint8_t Type_Command, uint8_t Data_Command)
    {
        PacketData[posElementRes++] = Type_Command;
        PacketData[posElementRes++] = 0x01;
        PacketData[posElementRes++] = Data_Command;
    }

    /**
     * @brief Adds a TLV (Type-Length-Value) element for a 32-bit value to the data packet.
     * 
     * @param Type_Command The command/data type.
     * @param Data_Command The 32-bit value.
     */
    void CommandLayer::addUInt32(uint8_t Type_Command, uint32_t Data_Command)
    {
        char float_bytes[sizeof(float)];
        memcpy(float_bytes, &Data_Command, sizeof(float));

        PacketData[posElementRes++] = Type_Command;
        PacketData[posElementRes++] = sizeof(float);
        for (uint8_t i = 0; i < sizeof(float); i++)
            PacketData[posElementRes++] = float_bytes[i];
    }

    /**
     * @brief Forms and sends a SetupRequest packet.
     * 
     * This packet is used to initialize a session with the controller.
     */
    void CommandLayer::setupPacket(void)
    {
        PacketLayer packetLayer;
        posElementRes = 0;

        addUInt8(Type_Operation, Operation_SetupRequest);

        packetLayer.sendPacket(PacketData, posElementRes);
    }

    /**
     * @brief Forms and sends a Poll packet.
     * 
     * This packet is used to maintain the connection and report readiness.
     */
    void CommandLayer::pollPacket(void)
    {
        PacketLayer packetLayer;
        posElementRes = 0;

        addUInt8(Type_Operation, Operation_Poll);
        addUInt8(Type_AutomatState, AutomatState_Ready);
        addUInt8(AutomatState_Init, AutomatState_Ready);

        packetLayer.sendPacket(PacketData, posElementRes);
    }

    /**
     * @brief Forms and sends a packet with information about the deposited credit.
     * 
     * @param count The number of credits (1 credit = 1 ruble). The value is multiplied by 100 to convert to kopecks.
     */
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