#pragma once

#include "Global.hpp"

namespace IndicationNS
{
    static void init(void);
    static void loop(void);

    void TaskIndication(void *pvParameters __attribute__((unused)));
};