#pragma once

#include "state_evaluator.h"

template <class T>
class InRangeStateEvaluator : public StateEvaluator<T> {
public:
	InRangeStateEvaluator(T min, T max) : min{min}, max{max} {};
	bool evaluateState(T state) const;
private:
	T min;
	T max;
};

#include "in_range_state_evaluator.tpp"