#pragma once

#include <chrono>

#include "state_reader.h"
#include "state_evaluator/state_evaluator.h"
#include "state_evaluator/in_range_state_evaluator.h"

class TimeStateReader : public StateReader<std::chrono::time_point<std::chrono::system_clock>> {
public:
    std::chrono::time_point<std::chrono::system_clock> getStateValue() override;
};