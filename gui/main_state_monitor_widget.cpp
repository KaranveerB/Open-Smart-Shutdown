#include "main_state_monitor_widget.h"

#include <utility>

MainStateMonitorWidget::MainStateMonitorWidget(QWidget *parent) : QWidget(parent) {
    stateMonitorListLayout = new QHBoxLayout(this);
    this->setLayout(stateMonitorListLayout);

    stateMonitorManager.startMonitor();
}

void MainStateMonitorWidget::addStateMonitor(IStateMonitor *sm, QString name) {
    unsigned int id = stateMonitorManager.addStateMonitor(sm);
    auto *newStateMonitorWidget = new StateMonitorWidget(id, std::move(name),
                                                         stateMonitorManager, this);
    stateMonitorListLayout->addWidget(newStateMonitorWidget);
}

void MainStateMonitorWidget::createNewStateMonitor() {
    StateMonitorCreatorWidget smCreatorWidget;
    smCreatorWidget.exec();
    if (smCreatorWidget.result() == QDialog::Accepted) {
        IStateMonitor *stateMonitor = smCreatorWidget.getStateMonitor();
        QString name = smCreatorWidget.getStateMonitorName();
        MainStateMonitorWidget::addStateMonitor(stateMonitor, name);
    }
}
