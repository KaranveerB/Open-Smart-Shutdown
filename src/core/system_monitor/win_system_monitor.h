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
    float getNetUpAmount() override; // TODO: implement
    float getNetDownAmount() override; // TODO: implement
    float getNetCombinedAmount() override;
};
