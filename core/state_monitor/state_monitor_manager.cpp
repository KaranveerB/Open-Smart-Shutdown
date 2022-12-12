#include "state_monitor_manager.h"

void StateMonitorManager::addStateReader(IStateReader *stateReader) {
    stateReaders.push_back(stateReader);
}

void StateMonitorManager::startMonitor() {
    setupScheduledStateReaderQueue();
    std::thread stateMonitorThread(&StateMonitorManager::monitorStates, this);
    stateMonitorThread.detach();
//    stateMonitorThread.join();
}

void StateMonitorManager::monitorStates() {
    bool ret = false;
    while (!ret) {
        ScheduledStateReader scheduledSR = scheduledStateReaderQueue.top();
        if (scheduledSR.isReady()) {
            scheduledStateReaderQueue.pop();
            auto* sr = scheduledSR.getStateReader();
            ret = sr->getStateActive();
            scheduleStateReader(sr); // reschedule state reader
        } else {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    std::cout << "task completed" << std::endl;
}

void StateMonitorManager::setupScheduledStateReaderQueue() {
    // clear schedule queue
    scheduledStateReaderQueue = std::priority_queue<ScheduledStateReader, std::vector<ScheduledStateReader>,
            ScheduledStateReader::compare>();

    // load scheduled tasks
    for (auto* sr : stateReaders) {
        scheduleStateReader(sr);
    }
}

void StateMonitorManager::scheduleStateReader(IStateReader *stateReader) {
    scheduledStateReaderQueue.emplace(stateReader);
}


