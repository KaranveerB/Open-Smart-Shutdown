#pragma once

#include <string>

class SystemMonitor {
public:
    virtual float getCpuUsage() = 0;
    virtual float getDiskUsage(std::string disk) = 0;
    virtual float getNetUpAmount() = 0;
    virtual float getNetDownAmount() = 0;
    virtual float getNetCombinedAmount() = 0;
};
