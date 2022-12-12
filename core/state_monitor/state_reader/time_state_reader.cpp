#include "time_state_reader.h"

using std::chrono::time_point;
using std::chrono::system_clock;

time_point<system_clock> TimeStateReader::getStateValue() {
    return system_clock::now();
}

