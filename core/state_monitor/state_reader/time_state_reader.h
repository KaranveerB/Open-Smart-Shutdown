#pragma once

#include <chrono>

#include "state_reader.h"
#include "state_evaluator/state_evaluator.h"

class TimeStateReader : public StateReader<std::chrono::time_point<std::chrono::system_clock>> {
public:
	explicit TimeStateReader(StateEvaluator<std::chrono::time_point<std::chrono::system_clock>>* evaluator);
private:
	std::chrono::time_point<std::chrono::system_clock> getStateValue() override;
};