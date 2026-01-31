/**
 * @file Main.cpp
 * @author Masyukov Pavel
 * @brief Program entry point. This is where all FreeRTOS tasks are created and started.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "Main.hpp"

void setup()
{
  delay(500);
  
  xTaskCreate(IndicationNS::TaskIndication, (const portCHAR *)"Indication", 128, NULL, 1, NULL);
  xTaskCreate(PulseNS::TaskPulse, (const portCHAR *)"Pulse", 128, NULL, 2, NULL);
  xTaskCreate(ButtonNS::TaskButton, (const portCHAR *)"Button", 128, NULL, 2, NULL);
  xTaskCreate(RoutineNS::TaskRoutine, (const portCHAR *)"Routine", 128, NULL, 2, NULL);

  vTaskStartScheduler();
}

void loop()
{
  //
}
