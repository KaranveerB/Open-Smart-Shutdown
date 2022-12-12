#pragma once

#include <iostream>
#include <queue>
#include <thread>
#include <vector>

#include "state_evaluator/state_evaluator.h"
#include "state_evaluator/in_range_state_evaluator.h"
#include "state_reader/state_reader.h"
#include "state_reader/time_state_reader.h"


class StateMonitorManager {
private:
    class ScheduledStateReader {
    public:
        explicit ScheduledStateReader(IStateReader *stateReader) :
                stateReader(stateReader),
                scheduledTime(std::chrono::steady_clock::now() + stateReader->getPollingInterval()) {};


        IStateReader *getStateReader() {
            return stateReader;
        }

        bool isReady() {
            return scheduledTime <= std::chrono::steady_clock::now();
        }

        struct compare {
            bool operator()(const ScheduledStateReader &a, const ScheduledStateReader &b) const {
                return a.scheduledTime <= b.scheduledTime;
            }
        };

    private:
        IStateReader *stateReader;
        std::chrono::time_point<std::chrono::steady_clock> scheduledTime;

    };

public:
    void addStateReader(IStateReader *stateReader);

    void startMonitor();

private:
    void setupScheduledStateReaderQueue();

    void scheduleStateReader(IStateReader *stateReader);

    void monitorStates();

    std::vector<IStateReader *> stateReaders;
    std::priority_queue<ScheduledStateReader,
            std::vector<ScheduledStateReader>,
            ScheduledStateReader::compare> scheduledStateReaderQueue;

};
