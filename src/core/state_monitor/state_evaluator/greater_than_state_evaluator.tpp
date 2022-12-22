template<class T>
bool GreaterThanStateEvaluator<T>::evaluateState(T state) const {
    return state > min;
}
