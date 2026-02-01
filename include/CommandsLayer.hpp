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
   /// @brief Protocol control bytes.
   enum Control 
    {
        Control_START	 		= 0x01, ///< Start of packet
        Control_ACK		 		= 0x06, ///< Acknowledge
        Control_NAK		 		= 0x15, ///< Negative Acknowledge
    };

    /// @brief Data types and commands in the TLV (Type-Length-Value) structure.
    enum Type 
    {
        Type_Operation			= 0x01, ///< Operation type (Setup, Poll, etc.)
        Type_ProtocolVersion	= 0x02, ///< Protocol version
        Type_Manufacturer		= 0x03, ///< Manufacturer
        Type_Model				= 0x04, ///< Model
        Type_SerialNumber		= 0x05, ///< Serial number
        Type_SoftwareVersion	= 0x06, ///< Software version

        Type_PaymentState		= 0x07, ///< Payment system state

        Type_AutomatState		= 0x08, ///< Vending machine state

        Type_Error				= 0x09, ///< Error code
        Type_CashCredit			= 0x0A, ///< Cash credit

        Type_ProductNumber		= 0x0B, ///< Product number
        Type_ProductAmount		= 0x0C, ///< Product amount
        
        Type_VendApprove		= 0x0D, ///< Vend approve
        Type_VendResult			= 0x0E, ///< Vend result
        
        Type_ProductName		= 0x10, ///< Product name
        Type_ProductPrice		= 0x11, ///< Product price

        Type_TaxSystem			= 0x12, ///< Tax system
        Type_TaxRate			= 0x13, ///< Tax rate

        Type_CashlessCredit		= 0x0F  ///< Cashless credit
    };

    /// @brief Operation types.
    enum Operation 
    {
        Operation_SetupRequest  = 0x01, ///< Setup request
        Operation_SetupResponse = 0x02, ///< Setup response
        Operation_Poll			= 0x03, ///< Poll state
        Operation_AutomatState	= 0x04, ///< Transmit machine state
        Operation_VendRequest	= 0x05, ///< Vend request
        Operation_VendResponse	= 0x06, ///< Vend response
        Operation_VendResult	= 0x07, ///< Vend result

        Operation_FiscalRequest = 0x08  ///< Fiscal data request
    };

    /// @brief Vending machine states.
    enum AutomatState 
    {
        AutomatState_Init 		= 0x01, ///< Initialization
        AutomatState_NotReady 	= 0x02, ///< Not ready
        AutomatState_Ready 		= 0x03, ///< Ready
        AutomatState_Error 		= 0x10, ///< Error
    };

    /** @brief Builds the data payload for different command types. */
    class CommandLayer
    {
        public:
            uint8_t PacketData[PACKET_SIZE]; ///< Buffer for assembling the data packet.
            uint8_t posElementRes   = 0;     ///< Current position in the `PacketData` buffer.

            /** @brief Class constructor. */
            CommandLayer(void);
            /** @brief Class destructor. */
            virtual ~CommandLayer(void);

            /** @brief Forms and sends an initialization packet. */
            void setupPacket(void);

            /** @brief Forms and sends a status poll packet. */
            void pollPacket(void);
            /** @brief Forms and sends a packet with credit information. */
            void cashPacket(uint8_t count);

            /** @brief Adds a TLV element with an 8-bit value to the packet. */
            virtual void addUInt8(uint8_t Type_Command, uint8_t Data_Command);
            /** @brief Adds a TLV element with a 32-bit value to the packet. */
            virtual void addUInt32(uint8_t Type_Command, uint32_t Data_Command);
    };
}