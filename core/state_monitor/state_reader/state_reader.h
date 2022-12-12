#pragma once

#include <chrono>

#include "state_evaluator/state_evaluator.h"

class IStateReader {
public:
    virtual bool getStateActive() = 0;

    virtual std::chrono::duration<int64_t> getPollingInterval() = 0;
};


template<class T>
class StateReader : public IStateReader {
public:
//    explicit StateReader(StateEvaluator<T> stateEvaluator) : evaluator(stateEvaluator) {};
//
//    StateReader(StateEvaluator<T> stateEvaluator, std::chrono::duration<int64_t> pollingInterval) : evaluator(
//            stateEvaluator), pollingInterval(pollingInterval) {};
// TODO: See comment in time_state_reader.h

    bool getStateActive() override;

    std::chrono::duration<int64_t> getPollingInterval() override;

protected:
    virtual T getStateValue() = 0;

    void setEvaluator(StateEvaluator<T> *newEvaluator);

    void setPollingInterval(std::chrono::duration<int64_t> newPollingInterval);


private:
    StateEvaluator<T> *evaluator;
    std::chrono::duration<int64_t> pollingInterval = std::chrono::seconds(1);
};

#include "state_reader.tpp"