#include "state_monitor_tracker.h"

StateMonitorTracker::StateMonitorTracker(unsigned int id, const StateMonitorManager &stateMonitorManager,
                                         QObject *parent) : QObject(parent), id{id} {

    QObject::connect(&stateMonitorManager, &StateMonitorManager::stateChanged,
                     this, &StateMonitorTracker::updateStateMonitorTracker);
}

void StateMonitorTracker::updateState(State &state) {
    State::StateStatus stateStatus = state.getState();

    QString newStateQString;

    switch (stateStatus) {
        case State::Active:
            newStateQString = "Active";
            break;
        case State::Inactive:
            newStateQString = "Inactive";
            break;
        case State::Buffered:
            newStateQString = QLatin1String("Buffer (" + std::to_string(state.getBufferCount()) + ")");
            break;
        case State::Waiting:
            newStateQString = "Waiting";
            break;
        case State::Error:
            newStateQString = "Error";
            break;
    }

    QString newStateValueQString = QString::fromStdString(state.getStateValueString());

    // only signal update if there is a change
    if (newStateQString != stateQString) {
        stateQString = newStateQString;
        emit stateMonitorTrackerStateChanged(stateQString);
        stateQString = newStateQString;
    }

    if (newStateValueQString != stateValueQString) {
        stateValueQString = newStateValueQString;
        emit stateMonitorTrackerStateValueChanged(stateValueQString);
        stateValueQString = newStateValueQString;
    }

}

void StateMonitorTracker::deleteStateMonitorTracker() {
    emit scheduleStateMonitorDeletion(id);
    delete this;
}

void StateMonitorTracker::updateStateMonitorTracker(unsigned int id, State *state) {
    // All StateMonitorTrackers react to this, but it should only be acted upon if the id matches the instance's id.
    if (this->id == id) {
        updateState(*state);
    }
}

