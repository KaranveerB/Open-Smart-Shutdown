#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#include "state_monitor_manager.h"
#include "state.h"

class StateMonitorWidget : public QWidget {
public:
    StateMonitorWidget(unsigned int id, QString name, const StateMonitorManager &stateMonitorManager,
                       QWidget *parent = nullptr);

    void updateState(State &state);

    unsigned int getId() const;

private slots:

    void updateStateMonitorWidget(unsigned int id, State *state);

private:
    unsigned int id;
    QLabel *nameLabel;
    QLabel *stateLabel;
};
