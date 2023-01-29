#pragma once

#include <QTime>

#include "state_reader.h"

class TimeStateReader : public StateReader<QTime> {
public:
    QTime getStateValue() const override {
        return QTime::currentTime();
    };

    std::string stateValueToString(QTime value) const override {
        return value.toString("hh:mm:ss").toStdString();
    }
};