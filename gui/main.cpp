#include <iostream>

#include "state_monitor.h"
#include "state_reader/time_state_reader.h"
#include "state_evaluator/in_range_state_evaluator.h"

#include <unistd.h>
#include <ctime>

using namespace std::chrono;

int main() {
	// Currently example test code
	auto *eval = new InRangeStateEvaluator<time_point<system_clock>>(system_clock::now() + seconds(10),
	                                                                 system_clock::now() + hours(1));

	auto *sr = new TimeStateReader(eval);
	StateMonitor sm;
	sm.addStateReader(sr);

	while (!sr->getStateActive()) {
		sm.monitor();
		sleep(1);
	}
	sm.monitor();

}