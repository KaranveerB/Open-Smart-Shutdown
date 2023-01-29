#include "win_system_monitor.h"

using namespace Wmi;

float WinSystemMonitor::getCpuUsage() {
    auto processor = retrieveWmi<Win32_Processor>();
    return (float) (processor.LoadPercentage);
}

float WinSystemMonitor::getDiskUsage(std::string diskName) {
    auto disks = retrieveAllWmi<Win32_PerfFormattedData_PerfDisk_PhysicalDisk>();
    for (const auto &disk: disks) {
        std::string driveId = disk.Name;
        if (diskName != "_Total") {
            driveId = driveId.substr(driveId.length() - 2);
        }

        if (driveId == diskName) {
            auto bytes = (float) disk.DiskWriteBytesPerSec;
            auto megabytes = bytes / 1e6f;
            megabytes = round(megabytes * 1000.0f) / 1000.0f; // round to 3 decimal places
            return megabytes;
        }
    }
    throw std::runtime_error("invalid drive");
}

float WinSystemMonitor::getNetUpAmount() {
    return 0.0f;
}

float WinSystemMonitor::getNetDownAmount() {
    return 0.0f;
}

float WinSystemMonitor::getNetCombinedAmount() {
    // TODO: Allow choosing of network adapter. Currently selects the first one (as most have one physical net adpater)
    auto netIface = retrieveWmi<Win32_PerfFormattedData_Tcpip_NetworkInterface>();
    auto bytes = (float) netIface.BytesTotalPerSec;
    auto kilobytes = bytes / 1e3f;
    kilobytes = round(kilobytes * 10.0f) / 10.0f; // round to 2 decimal places
    return kilobytes;
}
