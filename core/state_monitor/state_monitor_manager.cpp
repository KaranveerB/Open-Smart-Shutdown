#include "state_monitor_manager.h"
#include "state_monitor.h"
#include "events/event_triggers.h"

void StateMonitorManager::addStateMonitor(IStateMonitor *stateMonitor) {
    stateMonitors.push_back(stateMonitor);
    states->emplace_back(new StateQObject("TEST NAME"));
    emit monitoredStatesChanged();
}

void StateMonitorManager::startMonitor() {
    setupScheduledStateReaderQueue();
    std::thread stateMonitorThread(&StateMonitorManager::monitorStates, this);
    stateMonitorThread.detach();
}

void StateMonitorManager::monitorStates() {
    bool ret = false;
    while (!ret) {
        ScheduledStateReader scheduledSR = scheduledStateReaderQueue.top();
        if (scheduledSR.isReady()) {
            scheduledStateReaderQueue.pop();
            auto* sm = scheduledSR.getStateReader();
            ret = sm->getStateActive();
            scheduleStateReader(sm); // reschedule state reader
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    emit stateChanged();
    std::cout << "task completed" << std::endl;
}

void StateMonitorManager::setupScheduledStateReaderQueue() {
    // clear schedule queue
    scheduledStateReaderQueue = std::priority_queue<ScheduledStateReader, std::vector<ScheduledStateReader>,
            ScheduledStateReader::compare>();

    // load scheduled tasks
    for (auto* sm : stateMonitors) {
        scheduleStateReader(sm);
    }
}

void StateMonitorManager::scheduleStateReader(IStateMonitor *stateReader) {
    scheduledStateReaderQueue.emplace(stateReader);
}


