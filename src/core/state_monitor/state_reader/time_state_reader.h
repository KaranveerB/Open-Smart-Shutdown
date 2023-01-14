#pragma once

#include <QTime>

#include "state_reader.h"
#include "state_monitor/state_evaluator/state_evaluator.h"
#include "state_monitor/state_evaluator/in_range_state_evaluator.h"

class TimeStateReader : public StateReader<QTime> {
public:
    QTime getStateValue() const override;

    std::string stateValueToString(QTime value) const override;
};