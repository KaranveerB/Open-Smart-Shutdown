#include "time_state_reader.h"

QTime TimeStateReader::getStateValue() const {
    return QTime::currentTime();
}

