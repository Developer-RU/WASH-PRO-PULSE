#pragma once

#include "Global.hpp"
#include "CommandsLayer.hpp"

namespace RoutineNS
{
    static void init(void);
    static void loop(void);

    void TaskRoutine(void *pvParameters __attribute__((unused)));
}
