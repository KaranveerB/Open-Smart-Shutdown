#pragma once

#include <cmath>

#include <wmi.hpp>
#include <wmiclasses.hpp>
#include "wmiclassesextended.hpp"

#include "system_monitor.h"

class WinSystemMonitor : public SystemMonitor {
public:
    float getCpuUsage() override;
    float getDiskUsage(std::string diskName) override;
    float getNetUpAmount() override;
    float getNetDownAmount() override;
    float getNetCombinedAmount() override;
};
