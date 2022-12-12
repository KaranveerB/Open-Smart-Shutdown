#include "time_state_reader.h"

using std::chrono::time_point;
using std::chrono::system_clock;

TimeStateReader::TimeStateReader(StateEvaluator<time_point<system_clock>> *stateEvaluator) {
    setEvaluator(stateEvaluator);
}

TimeStateReader::TimeStateReader(StateEvaluator<time_point<system_clock>> *stateEvaluator,
                                 std::chrono::duration<int64_t> pollingInterval) {
    setEvaluator(stateEvaluator);
    setPollingInterval(pollingInterval);

}

time_point<system_clock> TimeStateReader::getStateValue() {
    return system_clock::now();
}

