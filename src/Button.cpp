/**
 * @file Button.cpp
 * @author Masyukov Pavel
 * @brief Implementation of the module for handling manual credit buttons.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "Button.hpp"

extern uint8_t count;

namespace ButtonNS
{
  uint8_t state10{0}, state50{0}; // State flags to prevent continuous incrementing while a button is held down.
  uint8_t flag10{0}, flag50{0};

  static void init(void)
  {
    pinMode(PB12, INPUT_PULLUP);
    pinMode(PB13, INPUT_PULLUP);
  }

  static void loop(void)
  {
    if (digitalRead(PB12) == LOW && flag10 == 0)
    {
      flag10 = 1;
    }
    else if (digitalRead(PB12) == LOW && flag10 == 1)
    {
      if (state10 == 0)
      {
        state10 = 1;
        count += 10;
      }
    }
    else if (digitalRead(PB12) == HIGH && flag10 == 1)
    {
      state10 = 0;
      flag10 = 0;
    }

    if (digitalRead(PB13) == LOW && flag50 == 0)
    {
      flag50 = 1;
    }
    else if (digitalRead(PB13) == LOW && flag50 == 1)
    {
      if (state50 == 0)
      {
        state50 = 1;
        count += 50;
      }
    }
    else if (digitalRead(PB13) == HIGH && flag50 == 1)
    {
      state50 = 0;
      flag50 = 0;
    }

    vTaskDelay(10);
  }

  void TaskButton(void *pvParameters __attribute__((unused)))
  {
    init();

    for (;;)
    {
      loop();
    }
  }
};