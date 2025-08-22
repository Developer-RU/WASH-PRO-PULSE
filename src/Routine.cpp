#include "Routine.hpp"

extern uint8_t count;
extern uint8_t state;

namespace RoutineNS
{
    SenderNS::CommandLayer commandLayer;
    SenderNS::AutomatState automatState;

    Crc crc;

    uint8_t sending{0};
    uint32_t repeat{0};
    uint32_t timeout{0};

    uint8_t request[PACKET_SIZE];
    unsigned int bytes_request = 0;
    unsigned int length_request_data = 0;

    static void init(void)
    {
        automatState = SenderNS::AutomatState_Init;
        state = automatState;

        Serial.setRx(PA10);
        Serial.setTx(PA9);

        Serial.begin(115200);
    }

    static void loop(void)
    {
        if (Serial.available() > 0)
        {
            vTaskDelay(5 / portTICK_PERIOD_MS);

            while (Serial.available() > 0)
            {
                uint8_t b = (uint8_t)Serial.read();

                if (bytes_request == 0 && (b == SenderNS::Control_ACK || b == SenderNS::Control_NAK))
                {
                    /*  NC  ASK or NOK  */
                    if (bytes_request == 0 && b == SenderNS::Control_ACK)
                    {
                        count = 0;

                        sending = 0;
                        repeat = 0;
                        timeout = 0;
                    }

                    if (bytes_request == 0 && b == SenderNS::Control_NAK)
                    {
                        //
                    }
                }
                else
                {
                    request[bytes_request] = b;
                    bytes_request++;
                }

                vTaskDelay(2 / portTICK_PERIOD_MS);
            }

            if (bytes_request > 1)
            {
                /*  Генерация и проверка CRC  */
                length_request_data = (request[1] << 8) + request[2];

                crc.start();

                for (int i = 0; i < length_request_data + 3; i++)
                {
                    crc.add(request[i]);
                }

                uint8_t resCrc = crc.get();

                if (request[bytes_request - 1] == resCrc)
                {
                    Serial.write(SenderNS::Control_ACK);

                    if (request[5] == SenderNS::Operation_SetupResponse)
                    {
                        automatState = SenderNS::AutomatState_NotReady;
                    }

                    if (request[5] == SenderNS::Operation_AutomatState)
                    {
                        if (request[6] == SenderNS::Type_AutomatState)
                        {
                            automatState = (SenderNS::AutomatState)request[8];
                            state = automatState;
                        }
                    }
                }
                else
                {
                    // Необходим повторный пакет
                    Serial.write(SenderNS::Control_NAK);
                }

                // Очищаем буферы приема и сбрасываем счетчики отправленых, принятых байт данных
                for (int i = 0; i < PACKET_SIZE; i++)
                    request[i] = 0x00;

                bytes_request = 0;
                length_request_data = 0;
            }

            vTaskDelay(150 / portTICK_PERIOD_MS);
        }

        if (sending == 0)
        {
            if (automatState == SenderNS::AutomatState_Init)
            {
                commandLayer.setupPacket();
            }
            else
            {
                if (count > 0 && automatState == SenderNS::AutomatState_Ready)
                {
                    commandLayer.cashPacket(count);
                }
                else
                {
                    commandLayer.pollPacket();
                }
            }

            timeout = repeat = millis();
            sending = 1;
        }
        else
        {
            if (millis() > repeat + 5000 && millis() < timeout + 5000)
            {
                if (automatState == SenderNS::AutomatState_Init)
                {
                    commandLayer.setupPacket();
                }
                else
                {
                    if (count > 0 && automatState == SenderNS::AutomatState_Ready)
                    {
                        commandLayer.cashPacket(count);
                    }
                    else
                    {
                        commandLayer.pollPacket();
                    }
                }

                repeat = millis();
            }

            if (millis() > timeout + 5000)
            {
                sending = 0;
                repeat = 0;
                timeout = 0;
            }
        }
    }

    void TaskRoutine(void *pvParameters __attribute__((unused)))
    {
        init();

        for (;;)
        {
            loop();
        }
    }
}
