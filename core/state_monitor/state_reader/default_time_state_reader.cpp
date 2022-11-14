#include "default_time_state_reader.h"

std::chrono::time_point<std::chrono::system_clock> DefaultTimeStateReader::getStateValue() {
	return std::chrono::system_clock::now();
}
