#pragma once

#include "state_evaluator/state_evaluator.h"

class IStateReader {
public:
	virtual bool getStateActive() = 0;
};


template<class T>
class StateReader : public IStateReader {
public:
	bool getStateActive() override {
		return evaluator->evaluateState(getStateValue());
	};
protected:
	virtual T getStateValue() = 0;

	void setEvaluator(StateEvaluator<T> *newEvaluator) {
		evaluator = newEvaluator;
	}

private:
	StateEvaluator<T> *evaluator;
};
