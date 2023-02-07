#pragma once

#include <QString>

#include "state_monitor/state.h"
#include "state_monitor/state_monitor_manager.h"

class StateMonitorTracker : public QObject {
Q_OBJECT
public:
    StateMonitorTracker(unsigned int id, const StateMonitorManager &stateMonitorManager,
                        QObject *parent = nullptr);

    void updateState(State &state);

    /*
     * Deletion of the state monitor itself is scheduled rather than done immediately. The StateMonitorTracker is
     * deleted immediately. Regardless, after this function returns, it should be treated as if everything is deleted
     * once the function returns as StateMonitorManager handles the rest of the deletion process.
     */
    void deleteStateMonitorTracker();

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

    /*
     * See comment on deleteStateMonitorTracker for more information. StateMonitorManager should react to this signal
     * to schedule deletion of the underlying StateMonitor instance.
     */
    void scheduleStateMonitorDeletion(unsigned int id);

};
