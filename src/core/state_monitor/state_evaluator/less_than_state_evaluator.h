#pragma once

#include "state_evaluator.h"

template<class T>
class LessThanStateEvaluator : public StateEvaluator<T> {
public:
    explicit LessThanStateEvaluator(T max) : max{max} {};

    bool evaluateState(T state) const {
        return state < max;
    };

private:
    T max;
};