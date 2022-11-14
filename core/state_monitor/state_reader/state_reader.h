#ifndef OPEN_SMART_SHUTDOWN_STATE_READER_H
#define OPEN_SMART_SHUTDOWN_STATE_READER_H

#include <ctime>

template<typename T>
class StateReader {
public:
	virtual T getStateValue() = 0;
};


#endif //OPEN_SMART_SHUTDOWN_STATE_READER_H
