template<class T>
bool StateReader<T>::getStateActive() {
    return evaluator->evaluateState(getStateValue());
};

template<class T>
std::chrono::duration<int64_t> StateReader<T>::getPollingInterval() {
    return pollingInterval;
};

template<class T>
void StateReader<T>::setEvaluator(StateEvaluator<T> *newEvaluator) {
    evaluator = newEvaluator;
}

template<class T>
void StateReader<T>::setPollingInterval(std::chrono::duration<int64_t> newPollingInterval) {
    pollingInterval = newPollingInterval;
}
