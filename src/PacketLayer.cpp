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

///////////////////////////////////////////////////

        // Serial.print(Control_START, HEX);
        // Serial.print(" ");
        // Serial.print(dataLen >> 8, HEX);
        // Serial.print(" ");
        // Serial.print(dataLen, HEX);
        // Serial.print(" ");
        // for (uint16_t i = 0; i < dataLen; i++)
        // {
        //     Serial.print(data[i], HEX);
        //     Serial.print(" ");
        //     vTaskDelay(2);
        // }
        // Serial.println(crc.get());
        // Serial.println(crc.get());

/////////////////////////////////////////////////////

        // vTaskDelay(150);
    }

    void PacketLayer::sendControl(uint8_t byte)
    {
        Serial.write(byte);
        vTaskDelay(150);
    }

    void PacketLayer::sendData(uint8_t data[], uint16_t dataLen)
    {
        for (uint16_t i = 0; i < dataLen; i++)
        {
            Serial.write(data[i]);
            vTaskDelay(2);
        }
    }
}