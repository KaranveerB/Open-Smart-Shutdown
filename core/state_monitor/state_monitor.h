#pragma once

#include <any>
#include <vector>
#include <iostream>

#include "state_evaluator/state_evaluator.h"
#include "state_evaluator/in_range_state_evaluator.h"
#include "state_reader/state_reader.h"
#include "state_reader/time_state_reader.h"

class StateMonitor {
private:
	std::vector<IStateReader *> stateReaders;
public:
	void addStateReader(IStateReader *stateReader);
	void monitor();
};
