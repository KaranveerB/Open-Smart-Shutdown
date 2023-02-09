#pragma once

#include <iostream>
#include <map>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include <QList>
#include <QTime>

#include "state.h"
#include "state_monitor.h"
#include "thread_pool.h"
#include "events/event_triggers.h"
#include "state_evaluator/state_evaluator.h"
#include "state_reader/state_reader.h"


class StateMonitorManager : public QObject {
Q_OBJECT
private:
    // used in scheduledStateMonitorQueue to schedule StateMonitors for execution (execute reader and evaluate value)
    class ScheduledStateMonitor {
    public:
        explicit ScheduledStateMonitor(unsigned int id, IStateMonitor *stateMonitor);

        IStateMonitor *getStateReader();

        [[nodiscard]] unsigned int getId() const;

        bool isReady();

        // defined here as it is required for scheduledStateMonitorVector
        struct compare {
            bool operator()(const ScheduledStateMonitor &a, const ScheduledStateMonitor &b) const {
                return a.scheduledTime > b.scheduledTime;
            }
        }; // ScheduledStateMonitor
    private:
        unsigned int id;
        IStateMonitor *stateReader;
        std::chrono::time_point<std::chrono::steady_clock> scheduledTime;
    };

public:
    struct Configuration {
        EventTriggers::Action triggerAction;
        QTime activationDelay;
        std::string shellCommand;
    };

    Configuration getConfig();

    void setConfig(Configuration newConfig);

    // returns whether StateMonitorManager is active (after this function runs)
    bool toggleActive();

    /*
     * Adds StateMonitor (and creates corresponding State) with default bufferSize as defined by State's constructor
     * This is less relevant for GUI uses where a default should be shown in the UI (thus a default buffer size is
     * defined by the GUI), but can be useful for CLI uses (and maybe TUI to a lesser extent) where a default
     * buffer size may be omitted entirely (e.g., omitted CLI flags) and is typically not shown to the user.
     */
    [[nodiscard]] unsigned int addStateMonitor(IStateMonitor *stateMonitor);

    [[nodiscard]] unsigned int addStateMonitor(IStateMonitor *stateMonitor, unsigned int stateBufferSize);

private:
    // starts monitoring in a detached thread
    void startMonitor();

    void monitorStates();

    void executeStateMonitor(ScheduledStateMonitor scheduledSR);

    void scheduleStateReader(unsigned int id, IStateMonitor *stateMonitor);

    /*
     * Checks 3 conditions (in order) before the event trigger is triggered:
     * 1. readyForEventTrigger is true (see comment on readyForEventTrigger for more info)
     * 2. current time is past eventTriggerTime's value (see comment on timeTillEventTrigger for more info)
     * TODO: 3. All states become active at least once after the current time is past eventTriggerTime's value.
     *    If any state becomes inactive during this period, readyForEventTrigger is set to false.
     *
     * WARNING: this method is poorly named as it is essentially the manager for all things related to
     * triggering the event trigger. That is, it checks the conditions for readyForEventTrigger, timeTillEventTrigger,
     * and sets their values. Additionally, it also emits the signal to trigger the event trigger.
     * TODO: Rename this to something clearer
     */
    void checkEventTriggerCondition();

    unsigned int nextId = 0; // id next added StateMonitor should use

    bool started = false;

    /*
     * Whether all states are active and time is the only restricting factor for the event trigger.
     * If this is true, then timeTillEventTrigger represents when the event trigger should be triggered.
     * See comment on timeTillEventTrigger for more info on what the time actually represents.
     */
    bool readyForEventTrigger = false;

    // used to schedule StateMonitors for execution (execute reader and evaluate value)
    std::priority_queue<ScheduledStateMonitor, std::vector<ScheduledStateMonitor>,
          ScheduledStateMonitor::compare> scheduledStateMonitorQueue =
          std::priority_queue<ScheduledStateMonitor, std::vector<ScheduledStateMonitor>,
                ScheduledStateMonitor::compare>();

    std::mutex queueMutex;

    std::thread stateMonitorThread;

    std::unordered_map<unsigned int, State *> statesMap;

    Configuration eventTriggerConfig = {EventTriggers::Action::Shutdown, QTime(0, 1, 0), ""};

    /*
     * Time that event trigger should be triggered as long as all states remain NOT inactive until then.
     * If this time is reached with all states staying NOT inactive for the entire duration, a final check is done
     * to ensure all states become active at least once after this time before triggering the event trigger.
     * See comment on checkEventTriggerCondition() for more info on this check.
     */
    std::chrono::time_point<std::chrono::steady_clock> eventTriggerTime;


public slots:

    /*
     * Schedules StateMonitor (and State) for deletion to avoid messing with the scheduledStateMonitorQueue and needing
     * to handle cases where the StateMonitor is being executed and is not in the priority queue.
     * Only the monitorStates() loop removes elements from the priority queue, so it isn't an issue to leave the
     * deletion to the monitorStates loop.
     *
     * An update signal for a state can be triggered for a StateMonitor that this function claims to have deleted
     * (when the StateMonitor is already in the process of being executed when the function is called), but this
     * shouldn't cause issues as signals can be safely ignored. Thus, the fact that this function schedules deletion
     * instead of deleting isn't relevant outside of this class, hence the function name hiding the behaviour of
     * scheduled deletions.
     */
    void deleteStateMonitor(unsigned int id);

signals:

    void stateChanged(unsigned int id, State *state) const;

    void monitoredStatesChanged() const;

    void timeTillEventTriggerChanged(QTime timeRemaining) const;

};
