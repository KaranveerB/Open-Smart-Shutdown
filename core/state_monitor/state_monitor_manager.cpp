#include "state_monitor_manager.h"

void StateMonitorManager::addStateMonitor(IStateMonitor *stateMonitor, unsigned int id) {
    if (statesMap.find(id) != statesMap.end()) {
        throw std::logic_error("invalid ID for state monitor (duplicate key)");
    }
    statesMap[id] = new State;
    scheduleStateReader(id, stateMonitor);
    emit monitoredStatesChanged();
}

State *StateMonitorManager::getState(unsigned int id) {
    return statesMap[id];
}

void StateMonitorManager::startMonitor() {
    stateMonitorThread = std::thread(&StateMonitorManager::monitorStates, this);
    stateMonitorThread.detach();
}

[[noreturn]] void StateMonitorManager::monitorStates() {
    while (true) {
        ScheduledStateReader scheduledSR = scheduledStateReaderQueue.top();
        if (scheduledSR.isReady()) {
            scheduledStateReaderQueue.pop();
            auto *sm = scheduledSR.getStateReader();
            statesMap[scheduledSR.getId()]->setState(sm->getStateActive());
            scheduleStateReader(scheduledSR.getId(), sm); // reschedule state reader
            emit stateChanged();
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void StateMonitorManager::scheduleStateReader(unsigned int id, IStateMonitor *stateMonitor) {
    scheduledStateReaderQueue.emplace(id, stateMonitor);
}


