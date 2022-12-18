template <class T>
bool InRangeStateEvaluator<T>::evaluateState(T state) const {
    return min <= state && state <= max;
}
