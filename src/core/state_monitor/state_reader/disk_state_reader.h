#pragma once

#include <iomanip>
#include <sstream>

#include "state_reader.h"
#include "system_monitor/system_monitor.h"

#if WIN32

#include "system_monitor/win_system_monitor.h"

#endif

class DiskStateReader : public StateReader<float> {
public:
    DiskStateReader(std::string driveName) {
#if WIN32
        sysMon = new WinSystemMonitor();
#endif
        // TODO: do other OSes

        // test if driveName is valid
        try {
            sysMon->getDiskUsage(driveName);
        } catch (std::exception &e) {
            throw std::exception("invalid driveName");
        }
        driveName = driveName;
    }

    float getStateValue() const override {
        return sysMon->getDiskUsage(driveName);
    };

    std::string stateValueToString(float value) const override {
        std::ostringstream output;
        output << std::fixed << std::setprecision(2) << value << "MB/s";
        return output.str();
    }

private:
    SystemMonitor *sysMon;
    std::string driveName;
};
