#include "state_monitor_tracker.h"

StateMonitorTracker::StateMonitorTracker(unsigned int id, const StateMonitorManager &stateMonitorManager,
                                         QObject *parent) : QObject(parent), id{id} {

    // Clion error for connect: "constexpr variable 'begin' must be initialized by a
    // constant expression" can be safely ignored here (likely bug in clion).
    // Error isn't present with SIGNAL() SLOT(), but using current approach leads to compile-time type-checking
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
    }

    QString newStateValueQString = QString::fromStdString(state.getStateValueString());

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

void StateMonitorTracker::deleteStateMonitor() {
    emit scheduleStateMonitorDeletion(id);
    delete this;
}

void StateMonitorTracker::updateStateMonitorTracker(unsigned int id, State *state) {
    if (this->id == id) {
        updateState(*state);
    }
}

