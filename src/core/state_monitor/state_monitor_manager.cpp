#include "state_monitor_manager.h"

unsigned int StateMonitorManager::addStateMonitor(IStateMonitor *stateMonitor) {
    unsigned int id = nextId++;
    statesMap[id] = new State;
    scheduleStateReader(id, stateMonitor);
    emit monitoredStatesChanged();
    return id;
}

unsigned int StateMonitorManager::addStateMonitor(IStateMonitor *stateMonitor, unsigned int stateBufferSize) {
    unsigned int id = nextId++;
    statesMap[id] = new State(stateBufferSize);
    scheduleStateReader(id, stateMonitor);
    emit monitoredStatesChanged();
    return id;
}

State *StateMonitorManager::getState(unsigned int id) const {
    return statesMap.at(id);
}

void StateMonitorManager::deleteStateMonitor(unsigned int id) {
    if (started) {
        statesMap.at(id)->scheduleForDeletion();
    }
}

void StateMonitorManager::startMonitor() {
    stateMonitorThread = std::thread(&StateMonitorManager::monitorStates, this);
    stateMonitorThread.detach();
    emit timeTillEventTriggerChanged(config.activationDelay);
}

void StateMonitorManager::monitorStates() {
    ThreadPool pool(5);
    while (started) {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (scheduledStateReaderQueue.empty()) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        ScheduledStateReader scheduledSR = scheduledStateReaderQueue.top();
        if (scheduledSR.isReady()) {
            scheduledStateReaderQueue.pop();
            lock.unlock();
            pool.enqueueTask([this, scheduledSR] {
                executeStateMonitor(scheduledSR);
            });
        } else {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    // TODO: integrate with Waiting state
}

void StateMonitorManager::executeStateMonitor(ScheduledStateReader scheduledSR) {
    auto *sm = scheduledSR.getStateReader();
    unsigned int id = scheduledSR.getId();
    if (statesMap[id]->isScheduledForDeletion()) {
        delete sm;
        statesMap.erase(id);
    } else {
        try {
            statesMap[id]->update(sm->getStateActive());
            statesMap[id]->setStateValueString(sm->getStateValueString());
        } catch (std::exception &exception) {
            statesMap[id]->setError();
            std::string errorMsg = "Internal Error: ";
            errorMsg += exception.what();
            statesMap[id]->setStateValueString(errorMsg);
        }
        emit stateChanged(id, statesMap.at(id));
        checkEventTriggerCondition();
        scheduleStateReader(id, sm); // reschedule state reader
    }

}

void StateMonitorManager::scheduleStateReader(unsigned int id, IStateMonitor *stateMonitor) {
    std::lock_guard<std::mutex> lock(queueMutex);
    scheduledStateReaderQueue.emplace(id, stateMonitor);
}

void StateMonitorManager::checkEventTriggerCondition() {
    for (const auto &state: statesMap) {
        if (state.second->getState() == State::StateStatus::Inactive) {
            if (readyForEventTrigger) {
                readyForEventTrigger = false;
                emit timeTillEventTriggerChanged(config.activationDelay);
            }
            return;
        }
    }

    if (!readyForEventTrigger) {
        readyForEventTrigger = true;
        unsigned int delaySeconds = config.activationDelay.minute() * 60 + config.activationDelay.second();
        eventTriggerTime = std::chrono::steady_clock::now() + std::chrono::seconds(delaySeconds);
    }

    auto timeDiff = eventTriggerTime - std::chrono::steady_clock::now();
    int timeDiff_ms = (int) std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count();
    QTime timeRemaining = QTime::fromMSecsSinceStartOfDay(timeDiff_ms);
    emit timeTillEventTriggerChanged(timeRemaining);

    if (eventTriggerTime <= std::chrono::steady_clock::now()) {
        emit timeTillEventTriggerChanged(QTime(0, 0, 0));
        EventTriggers::triggerEvent(config.triggerAction, config.shellCommand);
    }
}
