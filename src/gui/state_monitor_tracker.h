#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QItemDelegate>

#include "state_monitor/state_monitor_manager.h"
#include "state_monitor/state.h"

class StateMonitorTracker : public QObject {
Q_OBJECT
public:
    StateMonitorTracker(unsigned int id, const StateMonitorManager &stateMonitorManager,
                        QObject *parent = nullptr);

    void updateState(State &state);

    void deleteStateMonitor();

private:
    unsigned int id;
    QString nameQString;
    QString stateQString;
    QString stateValueQString;

private slots:

    void updateStateMonitorTracker(unsigned int id, State *state);

signals:

    void stateMonitorTrackerNameChanged(QString name);

    void stateMonitorTrackerStateChanged(QString state);

    void stateMonitorTrackerStateValueChanged(QString stateValue);

    void scheduleStateMonitorDeletion(unsigned int id);

};
