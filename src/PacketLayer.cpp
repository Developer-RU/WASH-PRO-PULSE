/**
 * @file PacketLayer.cpp
 * @author Masyukov Pavel
 * @brief Implementation of the PacketLayer class for sending framed data.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "PacketLayer.hpp"

namespace SenderNS
{
    PacketLayer::PacketLayer(void) {}
    PacketLayer::~PacketLayer(void) {}

    /**
     * @brief Forms and sends a complete data packet.
     * 
     * The packet has the following structure:
     * [START_BYTE][LEN_HI][LEN_LO][DATA...][CRC]
     * 
     * @param data Pointer to the data array to be sent.
     * @param dataLen Length of the data.
     */
    void PacketLayer::sendPacket(uint8_t data[], uint16_t dataLen)
    {
        Crc crc;

        crc.start();
        crc.add(Control_START);
        crc.add(dataLen >> 8);
        crc.add(dataLen);
        crc.add(data, dataLen);

        Serial.write(Control_START);
        Serial.write(dataLen >> 8);
        Serial.write(dataLen);
        sendData(data, dataLen);
        Serial.write(crc.get());

        // A small delay might be needed here depending on the receiver, but it's currently commented out.
        // vTaskDelay(150);
    }

    /**
     * @brief Sends a control byte (ACK/NAK).
     * 
     * @param byte The control byte to send.
     */
    void PacketLayer::sendControl(uint8_t byte)
    {
        Serial.write(byte);
        vTaskDelay(150);
    }

    /**
     * @brief Sends a data array with a small delay between bytes.
     * 
     * @param data Pointer to the data array.
     * @param dataLen Length of the data.
     */
    void PacketLayer::sendData(uint8_t data[], uint16_t dataLen)
    {
        for (uint16_t i = 0; i < dataLen; i++)
        {
            Serial.write(data[i]);
            vTaskDelay(2);
        }
    }
}