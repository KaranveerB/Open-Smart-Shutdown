template<class T>
bool StateMonitor<T>::getStateActive() const {
    return stateEvaluator->evaluateState(stateReader->getStateValue());
};

template<class T>
std::chrono::duration<int64_t> StateMonitor<T>::getPollingInterval() const {
    return pollingInterval;
};