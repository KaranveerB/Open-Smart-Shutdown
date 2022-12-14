#pragma once

#include <chrono>

#include <QHBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include <QListWidget>

#include "state_monitor_manager.h"

using namespace std::chrono; // TODO: Remove this which is for easier testing

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();

    QListWidget *stateListWidget;

private slots:

    void refreshStateList() {

        // TODO: Remove this test code
        stateListWidget->clear();
        auto state = stateMonitorManager.getState(0);
        stateListWidget->addItem("state: = " + QString::number(state->getState()));
    };

private:

    StateMonitorManager stateMonitorManager;

    void testStateMonitorManager() { // TODO: Remove this test code
        auto *eval = new InRangeStateEvaluator<time_point<system_clock>>(system_clock::now() + seconds(10),
                                                                         system_clock::now() + hours(1));
        auto *sr = new TimeStateReader;
        auto *sm = new StateMonitor<time_point<system_clock>>(sr, eval);
        stateMonitorManager.addStateMonitor(sm, 0);

        stateMonitorManager.startMonitor();
    };
};
