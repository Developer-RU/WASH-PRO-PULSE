<p align="left">
  <img src="/images/version.svg" alt="version">
  <img src="/images/license.svg" alt="license">
</p>

# WASH-PRO-PULSE

This project is a pulse converter based on the **STM32F103C8T6** microcontroller. The device is designed to read credit pulses from cashless payment (acquiring) terminals and transmit data to an equipment controller, such as a self-service car wash controller.

The code has been tested and is used for connecting to the **WASH-PRO** self-service car wash controller.

## Purpose and Functionality

The main purpose of the device is to integrate modern payment systems with equipment that is controlled by pulses or proprietary protocols.

**Key Features:**

1.  **Pulse Reading:** The device is configured to read pulses with 3.3V logic levels from acquiring terminals like **Vendista** and **Vendotek**. It uses a hardware interrupt on pin `PA0` for this purpose.

2.  **Manual Credit Addition:** It's possible to add credits manually using two buttons:
    *   Button on `PB12`: adds 10 credits.
    *   Button on `PB13`: adds 50 credits.

3.  **Data Exchange:** The device communicates with the main controller (e.g., WASH-PRO) via UART (`PA9`/`PA10`) using a protocol that partially mimics the **"Efor Online"** protocol. It sends packets with status information and the number of received credits.

4.  **Status Indication:** Two LEDs on pins `PB14` and `PB15` are used for visual feedback on the device's operation and status.

## Software Architecture

The firmware is written using **FreeRTOS**, which allows for the parallel execution of several tasks:

*   `TaskPulse`: Handles incoming pulses from the payment terminal.
*   `TaskButton`: Processes button presses for manual credit addition.
*   `TaskRoutine`: Manages the main logic, state, and data exchange with the main controller via UART.
*   `TaskIndication`: Controls the LED indicators.

This multitasking architecture ensures system responsiveness and reliability.

## Hardware

*   **Microcontroller:** STM32F103C8T6 ("Blue Pill").
*   **Интерфейсы:**
*   **Interfaces:**
    *   `PA0`: Input for pulses from the acquiring terminal (INPUT_PULLUP).
    *   `PB12`, `PB13`: Inputs for buttons (INPUT_PULLUP).
    *   `PA9` (TX), `PA10` (RX): UART for communication with the main controller.
    *   `PB14`, `PB15`: Outputs for LED indication.

## Potential Applications

Although this project was initially created for WASH-PRO car washes, its architecture allows it to be adapted for use with other devices and systems that require converting pulse signals into data for a master controller.
