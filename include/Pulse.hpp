#pragma once

#include "Global.hpp"

namespace PulseNS
{
    static void init(void);
    static void loop(void);

    void TaskPulse(void *pvParameters __attribute__((unused)));
};