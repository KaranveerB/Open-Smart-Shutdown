#include "in_range_state_parser.h"

template<typename T>
InRangeStateParser<T>::InRangeStateParser(StateReader<T> stateReader, T min, T max) : StateParser<T>(stateReader){
	this->min = min;
	this->max = max;
}

template<typename T>
bool InRangeStateParser<T>::isStateActive() {
	T currentStateValue = this->stateReader.getStateValue();
	return min <= currentStateValue && currentStateValue <= max;
}

