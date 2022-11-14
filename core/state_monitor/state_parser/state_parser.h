#ifndef OPEN_SMART_SHUTDOWN_STATE_PARSER_H
#define OPEN_SMART_SHUTDOWN_STATE_PARSER_H

#include "state_reader/state_reader.h"

template<typename T>
class StateParser {
public:
	virtual bool isStateActive() = 0;
protected:
	StateReader<T> stateReader;
	explicit StateParser(StateReader<T> stateReader) {
		this->stateReader = stateReader;
	}
};


#endif //OPEN_SMART_SHUTDOWN_STATE_PARSER_H
