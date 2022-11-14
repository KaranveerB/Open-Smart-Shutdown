#ifndef OPEN_SMART_SHUTDOWN_IN_RANGE_STATE_PARSER_H
#define OPEN_SMART_SHUTDOWN_IN_RANGE_STATE_PARSER_H

#include "state_parser.h"

template <typename T>
class InRangeStateParser : StateParser<T> {
public:
	bool isStateActive();
	InRangeStateParser(StateReader<T> stateReader, T min, T max);
private:
	T min;
	T max;
};


#endif //OPEN_SMART_SHUTDOWN_IN_RANGE_STATE_PARSER_H
