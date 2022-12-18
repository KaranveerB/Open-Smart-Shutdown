#pragma once

template <class T>
class StateEvaluator {
public:
	virtual bool evaluateState(T state) const = 0;
};
