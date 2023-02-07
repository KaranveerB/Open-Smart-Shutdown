#pragma once

#include <string>

/**
 * Abstract SystemMonitor to simplify writing cross-platform code.
 * A platform-specific implementation of this class should be made and used within the relevant state readers.
 */
class SystemMonitor {
public:
    virtual float getCpuUsage() = 0;

    virtual float getDiskUsage(std::string disk) = 0;

    virtual float getNetUpAmount() = 0;

    virtual float getNetDownAmount() = 0;

    virtual float getNetCombinedAmount() = 0;
};
