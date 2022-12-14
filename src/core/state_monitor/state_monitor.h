#pragma once

#include <chrono>

#include "state_evaluator/state_evaluator.h"
#include "state_reader/state_reader.h"

class IStateMonitor {
public:
    virtual bool getStateActive() const = 0;

    virtual std::chrono::duration<int64_t> getPollingInterval() const = 0;
};

template<class T>
class StateMonitor : public IStateMonitor {
public:
    StateMonitor(StateReader<T> *stateReader, StateEvaluator<T> *stateEvaluator) : stateReader{stateReader},
                                                                                   stateEvaluator{stateEvaluator} {
        if (stateReader == nullptr || stateEvaluator == nullptr) {
            throw std::logic_error("can't create state monitor with nullptr state reader or state evaluator");
        }
    };

    StateMonitor(StateReader<T> *stateReader, StateEvaluator<T> *stateEvaluator,
                 std::chrono::duration<int64_t> pollingInterval) : stateReader{stateReader},
                                                                   stateEvaluator{stateEvaluator},
                                                                   pollingInterval{pollingInterval} {
        if (stateReader == nullptr || stateEvaluator == nullptr) {
            throw std::logic_error("can't create state monitor with nullptr state reader or state evaluator");
        }
    };

    bool getStateActive() const override;

    std::chrono::duration<int64_t> getPollingInterval() const override;

private:
    StateReader<T> *stateReader;
    StateEvaluator<T> *stateEvaluator;
    std::chrono::duration<int64_t> pollingInterval = std::chrono::seconds(1);
};

#include "state_monitor.tpp"