#pragma once

#include <iomanip>
#include <sstream>

#include "state_reader.h"
#include "system_monitor/system_monitor.h"

#if WIN32
#include "system_monitor/win_system_monitor.h"
#endif

class NetStateReader : public StateReader<float> {
public:
    NetStateReader() {
#if WIN32
        sysMon = new WinSystemMonitor();
#endif
        // TODO: do other OSes
    }

    float getStateValue() const override {
        return sysMon->getNetCombinedAmount();
    };

    std::string stateValueToString(float value) const override {
        std::ostringstream output;
        output << std::fixed << std::setprecision(2) << value << "kB/s";
        return output.str();
    }

private:
    SystemMonitor *sysMon;
};
