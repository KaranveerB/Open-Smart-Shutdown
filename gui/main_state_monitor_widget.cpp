#include "main_state_monitor_widget.h"

#include <utility>

MainStateMonitorWidget::MainStateMonitorWidget(QWidget *parent) : QWidget(parent) {
    stateMonitorListLayout = new QHBoxLayout(this);
    this->setLayout(stateMonitorListLayout);

    // TODO: remove test code
    auto *eval = new InRangeStateEvaluator<std::chrono::time_point<std::chrono::system_clock>>(
            std::chrono::system_clock::now() + std::chrono::seconds(5),
            std::chrono::system_clock::now() + std::chrono::hours(1));
    auto *sr = new TimeStateReader;
    auto *sm = new StateMonitor<std::chrono::time_point<std::chrono::system_clock>>
            (sr, eval);

//    addStateMonitor(sm, 0, "Test SM");

    stateMonitorManager.startMonitor();
}

void MainStateMonitorWidget::addStateMonitor(IStateMonitor *sm, unsigned int id, QString name) {
    stateMonitorManager.addStateMonitor(sm, id);
    auto *newStateMonitorWidget = new StateMonitorWidget(id, std::move(name),
                                                         stateMonitorManager, this);
    stateMonitorListLayout->addWidget(newStateMonitorWidget);
}

void MainStateMonitorWidget::createNewStateMonitor() {
    StateMonitorCreatorWidget smCreatorWidget;
    smCreatorWidget.exec();
    if (smCreatorWidget.result() == QDialog::Accepted) {
        IStateMonitor* stateMonitor = smCreatorWidget.getStateMonitor();
        QString name = smCreatorWidget.getStateMonitorName();
        MainStateMonitorWidget::addStateMonitor(stateMonitor, 0, name);
    }
}
