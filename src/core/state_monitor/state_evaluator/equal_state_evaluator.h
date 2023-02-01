#pragma once

#include "state_evaluator.h"

template<class T>
class EqualStateEvaluator : public StateEvaluator<T> {
public:
    explicit EqualStateEvaluator(T other) : other{other} {};

    bool evaluateState(T state) const {
        return state == other;
    };

private:
    T other;
};