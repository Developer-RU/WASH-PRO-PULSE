/**
 * @file Routine.cpp
 * @author Masyukov Pavel
 * @brief Implementation of the main routine task, handling communication with the master controller.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "Routine.hpp"

extern uint8_t count; ///< Global credit counter.
extern uint8_t state; ///< Global state of the machine.

namespace RoutineNS
{
    SenderNS::CommandLayer commandLayer; ///< Instance of the class for creating commands.
    SenderNS::AutomatState automatState; ///< Current state of the machine.

    Crc crc; ///< Instance of the class for CRC calculation.

    uint8_t sending{0};   ///< Flag indicating that a packet has been sent and a response is expected.
    uint32_t repeat{0};   ///< Timer for re-sending a packet.
    uint32_t timeout{0};  ///< Timer for resetting the sending state.

    uint8_t request[PACKET_SIZE];          ///< Buffer for receiving incoming data.
    unsigned int bytes_request = 0;        ///< Number of received bytes.
    unsigned int length_request_data = 0;  ///< Length of data in the received packet.

    /**
     * @brief Initializes the Routine task.
     * 
     * Sets the initial state of the machine and configures the Serial port
     * for data exchange with the controller.
     */
    static void init(void)
    {
        automatState = SenderNS::AutomatState_Init;
        state = automatState;

        Serial.setRx(PA10);
        Serial.setTx(PA9);

        Serial.begin(115200);
    }

    /**
     * @brief Main loop of the Routine task.
     * 
     * Processes incoming data from the controller and sends packets
     * with status or credit count.
     */
    static void loop(void)
    {
        if (Serial.available() > 0)
        {   
            vTaskDelay(SERIAL_BUFFER_FILL_DELAY_MS / portTICK_PERIOD_MS);

            while (Serial.available() > 0)
            {
                uint8_t b = (uint8_t)Serial.read();

                if (bytes_request == 0 && (b == SenderNS::Control_ACK || b == SenderNS::Control_NAK))
                {
                    // Handle standalone ACK/NAK responses
                    if (b == SenderNS::Control_ACK)
                    {
                        count = 0;
                        sending = 0;
                        repeat = 0;
                        timeout = 0;
                        state = STATE_RESET;
                    }

                    if (b == SenderNS::Control_NAK)
                    {
                        // NAK received, maybe trigger a faster retry or log it.
                        // Currently does nothing.
                    }
                }
                else
                {
                    // Accumulate bytes for a data packet
                    request[bytes_request] = b;
                    bytes_request++;
                }

                vTaskDelay(SERIAL_BYTE_READ_DELAY_MS / portTICK_PERIOD_MS);
            }

            if (bytes_request > 1)
            {
                // CRC generation and check
                length_request_data = (request[1] << 8) + request[2];

                crc.start();

                for (int i = 0; i < length_request_data + 3; i++)
                {
                    crc.add(request[i]);
                }

                uint8_t resCrc = crc.get();

                if (request[bytes_request - 1] == resCrc)
                {
                    Serial.write(SenderNS::Control_ACK); // Protocol logic

                    if (request[OFFSET_OPERATION_VALUE] == SenderNS::Operation_SetupResponse && request[OFFSET_DATA_1_VALUE] == 0xFF)
                    {
                        automatState = SenderNS::AutomatState_NotReady;
                    }

                    if (request[OFFSET_OPERATION_VALUE] == SenderNS::Operation_AutomatState)
                    {
                        if (request[OFFSET_DATA_1_TYPE] == SenderNS::Type_AutomatState)
                        {
                            automatState = (SenderNS::AutomatState)request[OFFSET_DATA_1_VALUE];
                        }
                    }
                }
                else
                {
                    // Packet re-transmission is required
                    Serial.write(SenderNS::Control_NAK);
                }

                // Clear receive buffers and reset sent/received byte counters
                for (int i = 0; i < PACKET_SIZE; i++)
                    request[i] = 0x00;

                bytes_request = 0;
                length_request_data = 0;
            }
            else if (bytes_request == 1) {
                // A single byte was received that was not ACK or NAK.
                // This is likely an error or incomplete packet. Resetting.
                bytes_request = 0;
            }

            vTaskDelay(PACKET_PROCESSING_DELAY_MS / portTICK_PERIOD_MS);
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
                    state = automatState;
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
            if (millis() > repeat + PACKET_RETRY_TIMEOUT_MS && millis() < timeout + PACKET_RETRY_TIMEOUT_MS)
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

            if (millis() > timeout + PACKET_RETRY_TIMEOUT_MS)
            {
                sending = 0;
                repeat = 0;
                timeout = 0;
            }
        }
    }

    /**
     * @brief FreeRTOS task for main logic and communication.
     * 
     * @param pvParameters Unused pointer to task parameters.
     */
    void TaskRoutine(void *pvParameters __attribute__((unused)))
    {
        init();

        for (;;)
        {
            loop();
        }
    }
}
