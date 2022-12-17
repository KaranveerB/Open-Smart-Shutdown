#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#include "state.h"

class StateMonitorWidget : public QWidget {
public:
    StateMonitorWidget(unsigned int id, QString name, State &state, QWidget *parent = nullptr);

    void updateState(State &state);

    unsigned int getId();
private:

    unsigned int id;
    QLabel *nameLabel;
    QLabel *stateLabel;
};
