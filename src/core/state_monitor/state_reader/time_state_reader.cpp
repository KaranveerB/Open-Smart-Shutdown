#include "time_state_reader.h"

QTime TimeStateReader::getStateValue() const {
    return QTime::currentTime();
}

std::string TimeStateReader::stateValueToString(QTime value) const {
    return value.toString("hh:mm:ss").toStdString();
}

