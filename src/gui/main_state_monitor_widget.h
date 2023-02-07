#pragma once

#include <chrono>
#include <vector>

#include <QHeaderView>
#include <QObject>
#include <QTreeWidget>

#include "state_monitor/state_monitor_manager.h"

#include "configure_widget.h"
#include "state_monitor_creator_widget.h"
#include "state_monitor_tracker.h"
#include "state_monitor_widget_delegate.h"

class MainStateMonitorWidget : public QTreeWidget {
Q_OBJECT
public:
    explicit MainStateMonitorWidget(QWidget *parent = nullptr);

    void addStateMonitor(IStateMonitor *sm,
                         const StateMonitorCreatorWidget::StateMonitorMetaInfo &metaInfo);

    bool toggleStart();

public slots:

    void createNewStateMonitor();

    // configuration for the overall application (currently only activation delay and event trigger)
    void configure();

    void updateStateMonitorTrackerName(QString name);

    void updateStateMonitorTrackerState(QString state);

    void updateStateMonitorTrackerStateValue(const QString& stateValue);

    void updateTimeTillEventTrigger(QTime timeRemaining);

    void handleItemClicked(QTreeWidgetItem *item, int column);

signals:

    void timeTillEventTriggerUpdated(QTime timeRemaining);

private:
    int getRow(StateMonitorTracker *caller);

    std::vector<StateMonitorTracker *> stateMonitorTrackers;

    StateMonitorManager stateMonitorManager;
};
