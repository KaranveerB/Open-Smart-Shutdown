#pragma once

#include <iostream>
#include <map>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include <QList>

#include "state_evaluator/state_evaluator.h"
#include "state_evaluator/in_range_state_evaluator.h"
#include "state_reader/state_reader.h"
#include "state_reader/time_state_reader.h"
#include "state_monitor.h"
#include "state.h"
#include "events/event_triggers.h"
#include "thread_pool.h"


class StateMonitorManager : public QObject {
Q_OBJECT
private:
    class ScheduledStateReader {
    public:
        explicit ScheduledStateReader(unsigned int id, IStateMonitor *stateReader) :
                stateReader{stateReader},
                id{id},
                scheduledTime{std::chrono::steady_clock::now() + stateReader->getPollingInterval()} {};


        IStateMonitor *getStateReader() {
            return stateReader;
        }

        bool isReady() {
            return scheduledTime <= std::chrono::steady_clock::now();
        }

        unsigned int getId() const {
            return id;
        }

        struct compare {
            bool operator()(const ScheduledStateReader &a, const ScheduledStateReader &b) const {
                return a.scheduledTime > b.scheduledTime;
            }
        };

    private:
        IStateMonitor *stateReader;
        std::chrono::time_point<std::chrono::steady_clock> scheduledTime;
        unsigned int id;
    };

public:
    struct Configuration {
        EventTriggers::Action triggerAction;
        QTime activationDelay;
        std::string shellCommand;
    };

    [[nodiscard]] unsigned int addStateMonitor(IStateMonitor *stateMonitor);

    State *getState(unsigned int id) const;

    Configuration getConfig() {
        return config;
    }

    void setConfig(Configuration newConfig) {
        this->config = std::move(newConfig);
    }

    // returns new state
    bool toggleStart() {
        started = !started;
        if (started) {
            startMonitor();
        } else {
            readyForEventTrigger = false;
        }
        return started;
    }

private:
    void startMonitor();

    void scheduleStateReader(unsigned int id, IStateMonitor *stateMonitor);

    void monitorStates();

    void executeStateMonitor(ScheduledStateReader scheduledSR);

    void checkEventTriggerCondition();

    std::priority_queue<ScheduledStateReader, std::vector<ScheduledStateReader>,
            ScheduledStateReader::compare> scheduledStateReaderQueue =
            std::priority_queue<ScheduledStateReader, std::vector<ScheduledStateReader>,
                    ScheduledStateReader::compare>();

    std::mutex queueMutex;

    std::thread stateMonitorThread;

    std::unordered_map<unsigned int, State *> statesMap;

    Configuration config = {EventTriggers::Action::Shutdown, QTime(0, 1, 0), ""};

    unsigned int nextId = 0;

    bool started = false;

    bool readyForEventTrigger = false;
    std::chrono::time_point<std::chrono::steady_clock> eventTriggerTime;



public slots:
    void deleteStateMonitor(unsigned int id);

signals:

    void stateChanged(unsigned int id, State *state) const;

    void monitoredStatesChanged() const;

    void timeTillEventTriggerChanged(QTime timeRemaining) const;
};
