#pragma once

template <class T>
class StateEvaluator {
public:
	virtual bool evaluateState(T state) = 0;
};
