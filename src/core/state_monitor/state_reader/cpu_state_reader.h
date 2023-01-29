#pragma once

#include <iomanip>
#include <sstream>

#include "state_reader.h"
#include "system_monitor/system_monitor.h"

#if WIN32
#include "system_monitor/win_system_monitor.h"
#endif

class CpuStateReader : public StateReader<float> {
public:
    CpuStateReader() {
#if WIN32
        sysMon = new WinSystemMonitor();
#endif
        // TODO: do other OSes
    }
    float getStateValue() const override {
        return sysMon->getCpuUsage();
    };

    std::string stateValueToString(float value) const override {
        std::ostringstream output;
        output << std::fixed << std::setprecision(2) << value << "%";
        return output.str();
    }

private:
    SystemMonitor *sysMon;
};
