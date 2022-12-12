#include <iostream>

#include "state_monitor.h"
#include "state_reader/time_state_reader.h"
#include "state_evaluator/in_range_state_evaluator.h"
#include "state_monitor_manager.h"

#include <unistd.h>

using namespace std::chrono;

int main() {
	// Currently example test code
	auto *eval = new InRangeStateEvaluator<time_point<system_clock>>(system_clock::now() + seconds(10),
	                                                                 system_clock::now() + hours(1));

	auto *sr = new TimeStateReader;
    auto *sm = new StateMonitor<time_point<system_clock>>(sr, eval);
    StateMonitorManager smman;
    smman.addStateMonitor(sm);

    smman.startMonitor();

    sleep(14); // arbitrary sleep < 10 to allow state monitor to finish test monitor
    // in real program, there is no need for sleep as the thread runs as long as the application is running

}