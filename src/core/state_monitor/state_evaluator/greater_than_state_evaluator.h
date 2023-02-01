#pragma once

#include "state_evaluator.h"

template<class T>
class GreaterThanStateEvaluator : public StateEvaluator<T> {
public:
    explicit GreaterThanStateEvaluator(T min) : min{min} {};

    bool evaluateState(T state) const {
        return state > min;
    };

private:
    T min;
};