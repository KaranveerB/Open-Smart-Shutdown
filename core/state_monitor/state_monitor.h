#pragma once

#include <chrono>

#include "state_evaluator/state_evaluator.h"
#include "state_reader/state_reader.h"

class IStateMonitor {
public:
    virtual bool getStateActive() = 0;

    virtual std::chrono::duration<int64_t> getPollingInterval() = 0;
};

template<class T>
class StateMonitor : public IStateMonitor {
public:
    StateMonitor(StateReader<T> *stateReader, StateEvaluator<T> *stateEvaluator) : stateReader{stateReader},
                                                                                   stateEvaluator{stateEvaluator} {};

    StateMonitor(StateReader<T> *stateReader, StateEvaluator<T> *stateEvaluator,
                 std::chrono::duration<int64_t> pollingInterval) : stateReader{stateReader},
                                                                   stateEvaluator{stateEvaluator},
                                                                   pollingInterval{pollingInterval} {};

    bool getStateActive() override;

    std::chrono::duration<int64_t> getPollingInterval() override;

private:
    StateReader<T> *stateReader;
    StateEvaluator<T> *stateEvaluator;
    std::chrono::duration<int64_t> pollingInterval = std::chrono::seconds(1);
};

#include "state_monitor.tpp"