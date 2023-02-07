#pragma once

#include <cmath>

#include <wmi.hpp>
#include <wmiclasses.hpp>
#include "wmiclassesextended.hpp"

#include "system_monitor.h"

class WinSystemMonitor : public SystemMonitor {
public:
    float getCpuUsage() override;

    // we use diskName here instead of disk as that's how it's defined on windows
    // driveLabel == diskName in this case
    float getDiskUsage(std::string diskName) override;

    float getNetUpAmount() override; // TODO: implemented

    float getNetDownAmount() override; // TODO: implemented

    float getNetCombinedAmount() override;
};
