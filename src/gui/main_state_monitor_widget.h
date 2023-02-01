#pragma once

#include <chrono>

#include <QHeaderView>
#include <QObject>
#include <QTreeWidget>

#include "state_monitor/state_monitor_manager.h"

#include "configure_widget.h"
#include "state_monitor_tracker.h"
#include "state_monitor_widget_delegate.h"
#include "state_monitor_creator_widget.h"

class MainStateMonitorWidget : public QTreeWidget {
    Q_OBJECT
public:
    explicit MainStateMonitorWidget(QWidget *parent = nullptr);

    void addStateMonitor(IStateMonitor *sm, StateMonitorCreatorWidget::StateMonitorMetaInfo metaInfo); // TODO: Add proper fields

    bool toggleStart();
public slots:
    void createNewStateMonitor();
    void configure();

    void updateStateMonitorTrackerName(QString name);
    void updateStateMonitorTrackerState(QString state);
    void updateStateMonitorTrackerStateValue(QString stateValue);

    void updateTimeTillEventTrigger(QTime timeRemaining);

    void handleItemClicked(QTreeWidgetItem *item, int column);
signals:
    void timeTillEventTriggerUpdated(QTime timeRemaining);

private:
    int getRow(StateMonitorTracker *caller);

    std::unordered_map<int, StateMonitorTracker*> stateMonitorTrackerMap;

    StateMonitorManager stateMonitorManager;
};
