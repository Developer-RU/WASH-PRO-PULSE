/**
 * @file Button.cpp
 * @author Masyukov Pavel
 * @brief Implementation of the module for handling manual credit buttons.
 * @version 1.0.0
 * @see https://github.com/pavelmasyukov/WASH-PRO-PULSE
 */
#include "Button.hpp"

extern uint8_t count; ///< Global credit counter.

namespace ButtonNS
{
  uint8_t state10{0}, state50{0}; ///< State flags to prevent multiple triggers when buttons are held down.
  uint8_t flag10{0}, flag50{0};   ///< Flags to track the first button press.

  /**
   * @brief Initializes the button pins.
   * 
   * Configures pins PB12 and PB13 as inputs with pull-up resistors.
   */
  static void init(void)
  {
    pinMode(PB12, INPUT_PULLUP);
    pinMode(PB13, INPUT_PULLUP);
  }

  /**
   * @brief Main button processing loop.
   * 
   * Checks the button states and adds 10 or 50 credits to the global `count` counter.
   * Uses simple debouncing and protection against multiple triggers when held down.
   */
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
        count += CREDIT_VALUE_BUTTON_1;
      }
    }
    else if (digitalRead(PB12) == HIGH && flag10 == 1)
    {
      state10 = 0;
      flag10 = 0;
    }

    // --- Button for 50 credits ---
    if (digitalRead(PB13) == LOW && flag50 == 0)
    {
      flag50 = 1;
    }
    else if (digitalRead(PB13) == LOW && flag50 == 1)
    {
      if (state50 == 0)
      {
        state50 = 1;
        count += CREDIT_VALUE_BUTTON_2;
      }
    }
    else if (digitalRead(PB13) == HIGH && flag50 == 1)
    {
      state50 = 0;
      flag50 = 0;
    }

    vTaskDelay(BUTTON_DEBOUNCE_DELAY_MS);
  }

  /**
   * @brief FreeRTOS task for button handling.
   * 
   * @param pvParameters Unused pointer to task parameters.
   */
  void TaskButton(void *pvParameters __attribute__((unused)))
  {
    init();

    for (;;)
    {
      loop();
    }
  }
};