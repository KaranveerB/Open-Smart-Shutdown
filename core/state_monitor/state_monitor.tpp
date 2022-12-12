template<class T>
bool StateMonitor<T>::getStateActive() {
    return stateEvaluator->evaluateState(stateReader->getStateValue());
};

template<class T>
std::chrono::duration<int64_t> StateMonitor<T>::getPollingInterval() {
    return pollingInterval;
};