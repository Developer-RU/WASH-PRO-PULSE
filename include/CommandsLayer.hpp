/**
 * @file CommandsLayer.hpp
 * @author Masyukov Pavel
 * @brief Header for the CommandLayer class and protocol definitions.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#pragma once

#include "Global.hpp"
#include "PacketLayer.hpp"

#define PACKET_SIZE 512 ///< Defines the maximum size of a communication packet.

namespace SenderNS
{
   enum Control
    {
        Control_START	 		= 0x01,
        Control_ACK		 		= 0x06,
        Control_NAK		 		= 0x15,
    };

    enum Type 
    {
        Type_Operation			= 0x01,
        Type_ProtocolVersion	= 0x02,
        Type_Manufacturer		= 0x03,
        Type_Model				= 0x04,
        Type_SerialNumber		= 0x05,
        Type_SoftwareVersion	= 0x06,

        Type_PaymentState		= 0x07,

        Type_AutomatState		= 0x08,

        Type_Error				= 0x09,
        Type_CashCredit			= 0x0A,

        Type_ProductNumber		= 0x0B,
        Type_ProductAmount		= 0x0C,
        
        Type_VendApprove		= 0x0D,
        Type_VendResult			= 0x0E,
        
        Type_ProductName		= 0x10,
        Type_ProductPrice		= 0x11,

        Type_TaxSystem			= 0x12,
        Type_TaxRate			= 0x13,

        Type_CashlessCredit		= 0x0F
    };

    enum Operation 
    {
        Operation_SetupRequest  = 0x01,
        Operation_SetupResponse = 0x02,
        Operation_Poll			= 0x03,
        Operation_AutomatState	= 0x04,
        Operation_VendRequest	= 0x05,
        Operation_VendResponse	= 0x06,
        Operation_VendResult	= 0x07,

        Operation_FiscalRequest = 0x08
    };

    enum AutomatState 
    {
        AutomatState_Init 		= 0x01,
        AutomatState_NotReady 	= 0x02,
        AutomatState_Ready 		= 0x03,
        AutomatState_Error 		= 0x10,
    };

    /** @brief Builds the data payload for different command types. */
    class CommandLayer
    {
        public:
            
            uint8_t PacketData[PACKET_SIZE];
            uint8_t posElementRes   = 0;

            CommandLayer(void);
            virtual ~CommandLayer(void);

            void setupPacket(void);

            void pollPacket(void);
            void cashPacket(uint8_t count);

            virtual void addUInt8(uint8_t Type_Command, uint8_t Data_Command);
            virtual void addUInt32(uint8_t Type_Command, uint32_t Data_Command);
    };
}