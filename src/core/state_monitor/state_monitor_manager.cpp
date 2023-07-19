#include "state_monitor_manager.h"

// ScheduledStateMonitor
StateMonitorManager::ScheduledStateMonitor::ScheduledStateMonitor(unsigned int id, IStateMonitor *stateMonitor) :
      stateReader{stateMonitor},
      id{id},
      scheduledTime{std::chrono::steady_clock::now() + stateMonitor->getPollingInterval()} {}

IStateMonitor *StateMonitorManager::ScheduledStateMonitor::getStateReader() {
    return stateReader;
}

unsigned int StateMonitorManager::ScheduledStateMonitor::getId() const {
    return id;
}

bool StateMonitorManager::ScheduledStateMonitor::isReady() {
    return scheduledTime <= std::chrono::steady_clock::now();
}
// End of ScheduledStateMonitor

StateMonitorManager::Configuration StateMonitorManager::getConfig() {
    return eventTriggerConfig;
}

void StateMonitorManager::setConfig(StateMonitorManager::Configuration newConfig) {
    this->eventTriggerConfig = std::move(newConfig);
}

bool StateMonitorManager::toggleActive() {
    started = !started;
    if (started) {
        startMonitor();
    } else {
        // make sure event trigger won't be triggered prematurely due re-using an old timeTillEventTrigger value when
        // ready for readyForEventTrigger is true
        readyForEventTrigger = false;
    }
    return started;
}

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

void StateMonitorManager::startMonitor() {
    stateMonitorThread = std::thread(&StateMonitorManager::monitorStates, this);
    stateMonitorThread.detach();
    emit timeTillEventTriggerChanged(eventTriggerConfig.activationDelay);
}

void StateMonitorManager::monitorStates() {
    ThreadPool pool(5);
    while (started) {
        std::unique_lock<std::mutex> lock(queueMutex);
        if (scheduledStateMonitorQueue.empty()) {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }

        ScheduledStateMonitor scheduledSR = scheduledStateMonitorQueue.top();
        if (scheduledSR.isReady()) {
            scheduledStateMonitorQueue.pop();
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

void StateMonitorManager::executeStateMonitor(ScheduledStateMonitor scheduledSR) {
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
    scheduledStateMonitorQueue.emplace(id, stateMonitor);
}

void StateMonitorManager::checkEventTriggerCondition() {
    // 1. readyForEventTrigger
    for (const auto &state: statesMap) {
	  	if (state.second->getState() == State::StateStatus::Active || state.second->getState() == State::StateStatus::Buffered) {
			// state active, continue
		} else {
			// state inactive
			if (readyForEventTrigger) {
			  readyForEventTrigger = false;
			  emit timeTillEventTriggerChanged(eventTriggerConfig.activationDelay);
			}
			return; // readyForEventTrigger requires no inactive states, thus we return without checking any more states
		}
    }

    if (!readyForEventTrigger) {
        readyForEventTrigger = true;
        unsigned int delaySeconds =
              eventTriggerConfig.activationDelay.minute() * 60 + eventTriggerConfig.activationDelay.second();
        eventTriggerTime = std::chrono::steady_clock::now() + std::chrono::seconds(delaySeconds);
    }

    // 2. eventTriggerTime
    auto timeDiff = eventTriggerTime - std::chrono::steady_clock::now();
    int timeDiff_ms = (int) std::chrono::duration_cast<std::chrono::milliseconds>(timeDiff).count();
    QTime timeRemaining = QTime::fromMSecsSinceStartOfDay(timeDiff_ms);
    emit timeTillEventTriggerChanged(timeRemaining);

    // TODO: 3. All states active after eventTriggerTime

    if (eventTriggerTime <= std::chrono::steady_clock::now()) {
        // trigger event trigger
        emit timeTillEventTriggerChanged(QTime(0, 0, 0));
        EventTriggers::triggerEvent(eventTriggerConfig.triggerAction, eventTriggerConfig.shellCommand);
    }
}

void StateMonitorManager::deleteStateMonitor(unsigned int id) {
    if (started) {
        statesMap.at(id)->scheduleForDeletion();
    }
}

