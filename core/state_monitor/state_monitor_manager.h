#pragma once

#include <iostream>
#include <map>
#include <queue>
#include <thread>
#include <vector>

#include <QList>

#include "state_evaluator/state_evaluator.h"
#include "state_evaluator/in_range_state_evaluator.h"
#include "state_reader/state_reader.h"
#include "state_reader/time_state_reader.h"
#include "state_monitor.h"
#include "state.h"
#include "events/event_triggers.h"


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
                return a.scheduledTime < b.scheduledTime;
            }
        };

    private:
        IStateMonitor *stateReader;
        std::chrono::time_point<std::chrono::steady_clock> scheduledTime;
        unsigned int id;
    };

public:
    void addStateMonitor(IStateMonitor *stateMonitor, unsigned int id);

    void startMonitor();

    State *getState(unsigned int id) const;

private:
    void scheduleStateReader(unsigned int id, IStateMonitor *stateMonitor);

    [[noreturn]] void monitorStates();

    std::priority_queue<ScheduledStateReader, std::vector<ScheduledStateReader>,
            ScheduledStateReader::compare> scheduledStateReaderQueue =
            std::priority_queue<ScheduledStateReader, std::vector<ScheduledStateReader>,
                    ScheduledStateReader::compare>();

    std::thread stateMonitorThread;

    std::unordered_map<unsigned int, State *> statesMap;
signals:

    void stateChanged(unsigned int id, State *state) const;

    void monitoredStatesChanged() const;
};
