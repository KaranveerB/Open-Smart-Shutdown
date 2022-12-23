#include "state_monitor_tracker.h"

StateMonitorTracker::StateMonitorTracker(unsigned int id, QString name, const StateMonitorManager &stateMonitorItem,
                                         QObject *parent) : QObject(parent), id{id}, nameQString{name} {

    // Clion error for connect: "constexpr variable 'begin' must be initialized by a
    // constant expression" can be safely ignored here (likely bug in clion).
    // Error isn't present with SIGNAL() SLOT(), but using current approach leads to compile-time type-checking
    QObject::connect(&stateMonitorItem, &StateMonitorManager::stateChanged,
                     this, &StateMonitorTracker::updateStateMonitorTracker);
}

void StateMonitorTracker::updateState(State &state) {
    bool stateBool = state.getState();

    // TODO: Switch to have colors as well (keep text too for our friends with protanopia out there)
    QString newStateQString = stateBool ? "Active" : "Inactive";

    if (newStateQString != stateQString) {
        stateQString = newStateQString;
        emit stateMonitorTrackerStateChanged(stateQString);
    }

}


unsigned int StateMonitorTracker::getId() const {
    return id;
}

void StateMonitorTracker::updateStateMonitorTracker(unsigned int id, State *state) {
    if (this->id == id) {
        updateState(*state);
    }
}