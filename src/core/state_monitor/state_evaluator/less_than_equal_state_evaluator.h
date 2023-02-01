#pragma once

#include "state_evaluator.h"

template<class T>
class LessThanEqualStateEvaluator : public StateEvaluator<T> {
public:
    explicit LessThanEqualStateEvaluator(T max) : max{max} {};

    bool evaluateState(T state) const {
        return state <= max;
    };

private:
    T max;
};