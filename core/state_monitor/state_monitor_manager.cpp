#include "state_monitor_manager.h"

unsigned int StateMonitorManager::addStateMonitor(IStateMonitor *stateMonitor) {
    unsigned int id = nextId++;
    statesMap[id] = new State;
    scheduleStateReader(id, stateMonitor);
    emit monitoredStatesChanged();
    return id;
}

State *StateMonitorManager::getState(unsigned int id) const {
    return statesMap.at(id);
}

void StateMonitorManager::startMonitor() {
    stateMonitorThread = std::thread(&StateMonitorManager::monitorStates, this);
    stateMonitorThread.detach();
}

[[noreturn]] void StateMonitorManager::monitorStates() {
    while (true) {
        if (scheduledStateReaderQueue.empty()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        ScheduledStateReader scheduledSR = scheduledStateReaderQueue.top();
        if (scheduledSR.isReady()) {
            scheduledStateReaderQueue.pop();
            auto *sm = scheduledSR.getStateReader();
            unsigned int id = scheduledSR.getId();
            statesMap[id]->setState(sm->getStateActive());
            emit stateChanged(id, statesMap.at(id));
            scheduleStateReader(id, sm); // reschedule state reader
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

void StateMonitorManager::scheduleStateReader(unsigned int id, IStateMonitor *stateMonitor) {
    scheduledStateReaderQueue.emplace(id, stateMonitor);
}