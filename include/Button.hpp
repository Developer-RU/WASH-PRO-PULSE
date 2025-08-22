#pragma once

#include "Global.hpp"

namespace ButtonNS
{
    static void init(void);
    static void loop(void);

    void TaskButton(void *pvParameters __attribute__((unused)));
};