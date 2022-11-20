#pragma once

#include "state_evaluator.h"

template <class T>
class InRangeStateEvaluator : public StateEvaluator<T> {
public:
	InRangeStateEvaluator(T min, T max) : min{min}, max{max} {};
	bool evaluateState(T state) {
		return min <= state && state <= max;
	}
private:
	T min;
	T max;
};
