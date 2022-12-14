#pragma once

#include <chrono>

#include <QTreeWidget>
#include <QHeaderView>

#include "state_monitor/state_monitor_manager.h"

#include "state_monitor_tracker.h"
#include "state_monitor_widget_delegate.h"
#include "state_monitor_creator_widget.h"

class MainStateMonitorWidget : public QTreeWidget {
    Q_OBJECT
public:
    explicit MainStateMonitorWidget(QWidget *parent = nullptr);

    void addStateMonitor(IStateMonitor *sm, QString name); // TODO: Add proper fields

public slots:
    void createNewStateMonitor();

    void updateStateMonitorTrackerName(QString name);
    void updateStateMonitorTrackerState(QString state);

private:
    int getRow(StateMonitorTracker *caller);

    QList<StateMonitorTracker *> stateMonitorTrackers;

    StateMonitorManager stateMonitorManager;
};
