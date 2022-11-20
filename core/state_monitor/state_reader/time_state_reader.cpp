#include "time_state_reader.h"

#include "state_evaluator/state_evaluator.h"

using namespace std::chrono;

TimeStateReader::TimeStateReader(StateEvaluator<time_point<system_clock>> *evaluator) {
	setEvaluator(evaluator);
};

time_point<system_clock> TimeStateReader::getStateValue() {
	return system_clock::now();
}