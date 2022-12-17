#include "main_state_monitor_widget.h"

#include <utility>

MainStateMonitorWidget::MainStateMonitorWidget(QWidget *parent) : QWidget(parent) {
    stateMonitorListLayout = new QHBoxLayout(this);
    this->setLayout(stateMonitorListLayout);

    // TODO: remove test code
    auto *eval = new InRangeStateEvaluator<std::chrono::time_point<std::chrono::system_clock>>(
            std::chrono::system_clock::now() + std::chrono::seconds(10),
            std::chrono::system_clock::now() + std::chrono::hours(1));
    auto *sr = new TimeStateReader;
    auto *sm = new StateMonitor<std::chrono::time_point<std::chrono::system_clock>>
            (sr, eval);

    addStateMonitor(sm, 0, "Test SM");

    stateMonitorManager.startMonitor();

    QObject::connect(&stateMonitorManager, SIGNAL(stateChanged()), this, SLOT(updateStateMonitorWidgets()));
    // TODO: Move updating widget logic to sub-widgets. Pass new state in stateChanged()
}

void MainStateMonitorWidget::addStateMonitor(IStateMonitor *sm, unsigned int id, QString name) {
    stateMonitorManager.addStateMonitor(sm, id);
    auto *newStateMonitorWidget = new StateMonitorWidget(id, std::move(name),
                                                         *stateMonitorManager.getState(id), this);
    stateMonitorListLayout->addWidget(newStateMonitorWidget);
}

void MainStateMonitorWidget::updateStateMonitorWidgets() {
    int size = stateMonitorListLayout->count();
    for (int i = 0; i < size; i++) {
        auto stateMonitorWidget = (StateMonitorWidget *) stateMonitorListLayout->itemAt(i)->widget();
        unsigned int id = stateMonitorWidget->getId();
        stateMonitorWidget->updateState(*stateMonitorManager.getState(id));
    }
}