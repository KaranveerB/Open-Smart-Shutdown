#pragma once

#include <string>

#include "state_monitor/state_evaluator/state_evaluator.h"

template<class T>
class StateReader {
public:
    virtual T getStateValue() const = 0;

    /*
     * converts state value to its string representation
     *
     * This function takes value as an argument instead of internally using getStateValue() as it may produce different
     * values than the ones used in evaluation if the functions are called separately. By taking value as a argument,
     * one can re-use the value used in evaluation for the string, creating consistency in the value used and displayed.
     */
    virtual std::string stateValueToString(T value) const = 0;
};
