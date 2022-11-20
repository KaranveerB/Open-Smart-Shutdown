#include "state_monitor.h"

void StateMonitor::addStateReader(IStateReader *stateReader) {
	stateReaders.push_back(stateReader);
}

void StateMonitor::monitor() {
	// Currently example test code
	for (int i = 0; i < stateReaders.size(); i++) {
		if (stateReaders[i]->getStateActive()) {
			std::cout << i << ": active" << std::endl;
		} else {
			std::cout << i << ": inactive" << std::endl;
		}
	}
}
