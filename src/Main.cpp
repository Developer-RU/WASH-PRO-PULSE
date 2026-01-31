#include "Main.hpp"

void setup()
{
  delay(500);
  
  xTaskCreate(IndicationNS::TaskIndication, (const portCHAR *)"Indication", 128, NULL, 2, NULL);
  xTaskCreate(PulseNS::TaskPulse, (const portCHAR *)"Pulse", 128, NULL, 2, NULL);
  xTaskCreate(ButtonNS::TaskButton, (const portCHAR *)"Button", 128, NULL, 2, NULL);
  xTaskCreate(RoutineNS::TaskRoutine, (const portCHAR *)"Routine", 128, NULL, 2, NULL);

  vTaskStartScheduler();
}

void loop()
{
  //
}
