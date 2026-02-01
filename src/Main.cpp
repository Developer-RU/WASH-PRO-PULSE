/**
 * @file Main.cpp
 * @author Masyukov Pavel
 * @brief Program entry point. This is where all FreeRTOS tasks are created and started.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "Main.hpp"

/**
 * @brief Main setup function. Called once on startup.
 * 
 * Initializes and starts all FreeRTOS tasks:
 * - TaskIndication: Manages LED indication.
 * - TaskPulse: Processes pulses from the payment terminal.
 * - TaskButton: Handles button presses for manual credit addition.
 * - TaskRoutine: Manages the main logic and data exchange with the controller.
 */
void setup()
{
  delay(500);
  
  // Create and start FreeRTOS tasks with different priorities
  xTaskCreate(IndicationNS::TaskIndication, (const portCHAR *)"Indication", 128, NULL, 1, NULL);
  xTaskCreate(PulseNS::TaskPulse, (const portCHAR *)"Pulse", 128, NULL, 2, NULL);
  xTaskCreate(ButtonNS::TaskButton, (const portCHAR *)"Button", 128, NULL, 2, NULL);
  xTaskCreate(RoutineNS::TaskRoutine, (const portCHAR *)"Routine", 128, NULL, 2, NULL);

  // Start the FreeRTOS scheduler. After this call, control is passed to the tasks.
  vTaskStartScheduler();
}

/**
 * @brief Main Arduino loop.
 * 
 * Not used in this project, as all logic is implemented in FreeRTOS tasks.
 * The FreeRTOS scheduler takes over CPU control after vTaskStartScheduler() is called.
 */
void loop()
{
  // User code can be placed here if needed, but it will not be executed
  // because FreeRTOS takes control after setup().
}
