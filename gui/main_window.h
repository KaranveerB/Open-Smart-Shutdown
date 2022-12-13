#pragma once

#include <QHBoxLayout>
#include <QMainWindow>
#include <QPushButton>
#include "state_monitor_manager.h"

#include <chrono>

using namespace std::chrono; // TODO: Remove this which is for easier testing

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    MainWindow();

    QPushButton *testButton;

private slots:

    void refreshStateStatuses() {
        if (!smman.states->empty()) {
            testButton->setText("yay");
        }
    };

private:

    StateMonitorManager smman;

    void testStateMonitorManager() { // TODO: Remove this test code
        auto *eval = new InRangeStateEvaluator<time_point<system_clock>>(system_clock::now() + seconds(10),
                                                                         system_clock::now() + hours(1));
        auto *sr = new TimeStateReader;
        auto *sm = new StateMonitor<time_point<system_clock>>(sr, eval);
        smman.addStateMonitor(sm);

        smman.startMonitor();
    };
};
