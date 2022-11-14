#ifndef OPEN_SMART_SHUTDOWN_DEFAULT_TIME_STATE_READER_H
#define OPEN_SMART_SHUTDOWN_DEFAULT_TIME_STATE_READER_H

#include <chrono>

#include "state_reader.h"

class DefaultTimeStateReader : StateReader<std::chrono::time_point<std::chrono::system_clock>> {
	std::chrono::time_point<std::chrono::system_clock> getStateValue() override;
};


#endif //OPEN_SMART_SHUTDOWN_DEFAULT_TIME_STATE_READER_H
