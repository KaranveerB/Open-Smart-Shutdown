#pragma once

#include "state_evaluator.h"

template<class T>
class GreaterThanEqualStateEvaluator : public StateEvaluator<T> {
public:
    explicit GreaterThanEqualStateEvaluator(T min) : min{min} {};

    bool evaluateState(T state) const {
        return state >= min;
    };

private:
    T min;
};