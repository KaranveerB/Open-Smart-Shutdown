template<class T>
bool StateMonitor<T>::getStateActive() {
    prevStateValue = stateReader->getStateValue();
    return stateEvaluator->evaluateState(prevStateValue);
};

template<class T>
std::string StateMonitor<T>::getStateValueString() const {
    return stateReader->stateValueToString(prevStateValue);
}

template<class T>
std::chrono::duration<int64_t> StateMonitor<T>::getPollingInterval() const {
    return pollingInterval;
};