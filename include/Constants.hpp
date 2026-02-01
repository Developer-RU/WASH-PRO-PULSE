/**
 * @file Constants.hpp
 * @author Masyukov Pavel
 * @brief Defines global constants for the project.
 * @version 0.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */

#pragma once

// === Task Delays and Timings ===

// --- Routine Task ---
const uint32_t SERIAL_BUFFER_FILL_DELAY_MS = 5;   ///< Delay to allow serial buffer to fill before reading.
const uint32_t SERIAL_BYTE_READ_DELAY_MS = 2;     ///< Small delay between reading consecutive bytes from serial.
const uint32_t PACKET_PROCESSING_DELAY_MS = 150;  ///< Delay after processing an incoming packet.
const uint32_t PACKET_RETRY_TIMEOUT_MS = 5000;    ///< Interval for re-sending a packet if no ACK is received.

// --- Pulse Task ---
const uint32_t PULSE_BURST_TIMEOUT_MS = 100;      ///< Time without pulses to consider a burst complete.
const uint32_t PULSE_TASK_INTERVAL_MS = 100;      ///< Check interval for the pulse task.

// --- Button Task ---
const uint32_t BUTTON_DEBOUNCE_DELAY_MS = 10;     ///< Delay for button debouncing.

// --- Indication Task ---
const uint32_t INDICATION_TASK_INTERVAL_MS = 100; ///< Blink interval for the indication task.

// === Application Logic ===

// --- Button Credits ---
const uint8_t CREDIT_VALUE_BUTTON_1 = 10;         ///< Credits added by the first button.
const uint8_t CREDIT_VALUE_BUTTON_2 = 50;         ///< Credits added by the second button.

// --- State Machine ---
const uint8_t STATE_RESET = 0;                    ///< A neutral state, used to reset after a transaction.

// === Pin Definitions ===

// --- Communication ---
const auto UART_TX_PIN = PA9;
const auto UART_RX_PIN = PA10;

// --- Inputs ---
const auto PULSE_INPUT_PIN = PA0;
const auto BUTTON_10_PIN = PB12;
const auto BUTTON_50_PIN = PB13;

// --- Outputs ---
const auto LED_HEARTBEAT_PIN = PB14;
const auto LED_STATUS_PIN = PB15;

// === Communication Protocol ===

/**
 * @brief Defines offsets for accessing fields in a received data packet.
 * Assumes a TLV (Type-Length-Value) structure for the data payload.
 */
enum PacketOffsets {
    OFFSET_OPERATION_TYPE = 3,  ///< Offset to the 'Type' field of the operation TLV.
    OFFSET_OPERATION_VALUE = 5, ///< Offset to the 'Value' field of the operation TLV.
    OFFSET_DATA_1_TYPE = 6,     ///< Offset to the 'Type' field of the first data TLV.
    OFFSET_DATA_1_VALUE = 8,    ///< Offset to the 'Value' field of the first data TLV.
};