#pragma once

#include <chrono>

#include "state_reader.h"
#include "state_evaluator/state_evaluator.h"
#include "state_evaluator/in_range_state_evaluator.h"

class TimeStateReader : public StateReader<std::chrono::time_point<std::chrono::system_clock>> {
public:
    explicit TimeStateReader(StateEvaluator<std::chrono::time_point<std::chrono::system_clock>> *stateEvaluator);

    TimeStateReader(StateEvaluator<std::chrono::time_point<std::chrono::system_clock>> *stateEvaluator,
                    std::chrono::duration<int64_t> pollingInterval);

//    using StateReader<std::chrono::time_point<std::chrono::system_clock>>::StateReader; // TODO: should inherit parent constructors

private:
    std::chrono::time_point<std::chrono::system_clock> getStateValue() override;
};