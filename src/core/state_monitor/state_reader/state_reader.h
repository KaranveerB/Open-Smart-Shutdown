#pragma once

#include <chrono>

#include "state_monitor/state_evaluator/state_evaluator.h"

template<class T>
class StateReader {
public:
    virtual T getStateValue() const = 0;
};
