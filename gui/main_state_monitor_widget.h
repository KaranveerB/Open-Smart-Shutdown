#pragma once

#include <chrono>
#include <utility>

#include <QWidget>
#include <QHBoxLayout>

#include "state_monitor_widget.h"
#include "state_monitor_manager.h"
#include "state_monitor_creator_widget.h"

class MainStateMonitorWidget : public QWidget {
Q_OBJECT
public:
    explicit MainStateMonitorWidget(QWidget *parent = nullptr);

    void addStateMonitor(IStateMonitor *sm, unsigned int id, QString name); // TODO: Add proper fields

public slots:

    void updateStateMonitorWidgets();

    void createNewStateMonitor();

private:

    QHBoxLayout *stateMonitorListLayout;

    StateMonitorManager stateMonitorManager;
};
