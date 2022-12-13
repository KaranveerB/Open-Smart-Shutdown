#pragma once

#include <iostream>
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


class StateMonitorManager : public QObject {
Q_OBJECT
private:
    class ScheduledStateReader {
    public:
        explicit ScheduledStateReader(IStateMonitor *stateReader) :
                stateReader(stateReader),
                scheduledTime(std::chrono::steady_clock::now() + stateReader->getPollingInterval()) {};


        IStateMonitor *getStateReader() {
            return stateReader;
        }

        bool isReady() {
            return scheduledTime <= std::chrono::steady_clock::now();
        }

        struct compare {
            bool operator()(const ScheduledStateReader &a, const ScheduledStateReader &b) const {
                return a.scheduledTime < b.scheduledTime;
            }
        };

    private:
        IStateMonitor *stateReader;
        std::chrono::time_point<std::chrono::steady_clock> scheduledTime;

    };

public:
    void addStateMonitor(IStateMonitor *stateMonitor);

    void startMonitor();

    QList<StateQObject *> *states = new QList<StateQObject *>;

private:
    void setupScheduledStateReaderQueue();

    void scheduleStateReader(IStateMonitor *stateReader);

    void monitorStates();

    std::vector<IStateMonitor *> stateMonitors;
    std::priority_queue<ScheduledStateReader,
            std::vector<ScheduledStateReader>,
            ScheduledStateReader::compare> scheduledStateReaderQueue;

signals:
    void stateChanged();
    void monitoredStatesChanged();
};
