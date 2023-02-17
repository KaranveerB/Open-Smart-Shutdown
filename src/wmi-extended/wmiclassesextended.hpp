#pragma once

#include <cstdint>
#include <string>

#include <wmi.hpp>
#include <wmiresult.hpp>

/**
 * This header file only contains Win32 classes containing the values I need.
 * For performance and simplicity, I don't include what I don't need.
 * Other values can be added as needed.
 *
 * There are other Win32 classes used that I will strip down later.
 */

namespace Wmi {
    struct Win32_PerfFormattedData_PerfDisk_PhysicalDisk {
        Win32_PerfFormattedData_PerfDisk_PhysicalDisk() :
              DiskWriteBytesPerSec(),
              Name() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "DiskWriteBytesPerSec", (*this).DiskWriteBytesPerSec);
            result.extract(index, "Name", (*this).Name);
        }

        static std::string getWmiClassName() {
            return "Win32_PerfFormattedData_PerfDisk_PhysicalDisk";
        }


        uint64_t DiskWriteBytesPerSec;
        std::string Name;
    };

    struct Win32_PerfFormattedData_Tcpip_NetworkInterface {
        Win32_PerfFormattedData_Tcpip_NetworkInterface() :
              BytesTotalPerSec(),
              Name() {}

        void setProperties(const WmiResult &result, std::size_t index) {
            result.extract(index, "BytesTotalPerSec", (*this).BytesTotalPerSec);
            result.extract(index, "Name", (*this).Name);
        }

        static std::string getWmiClassName() {
            return "Win32_PerfFormattedData_Tcpip_NetworkInterface";
        }

        uint64_t BytesTotalPerSec;
        std::string Name;

    };
}