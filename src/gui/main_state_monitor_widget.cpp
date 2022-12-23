#include "main_state_monitor_widget.h"

MainStateMonitorWidget::MainStateMonitorWidget(QWidget *parent) : QTreeWidget(parent) {
    QStringList headerLabels;
    headerLabels << tr("Type") << tr("Name") << tr("State") << tr("") << tr("");

    setHeaderLabels(headerLabels);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);

    setItemDelegate(new StateMonitorWidgetDelegate);


    stateMonitorManager.startMonitor();
}

void MainStateMonitorWidget::addStateMonitor(IStateMonitor *sm, QString name) {
    unsigned int id = stateMonitorManager.addStateMonitor(sm);
    auto *newStateMonitorTracker = new StateMonitorTracker(id, name,
                                                           stateMonitorManager, this);
    stateMonitorTrackers.append(newStateMonitorTracker);

    auto *item = new QTreeWidgetItem; // TODO: Customize this

    item->setText(0, "Time");
    item->setText(1, name);
    item->setText(2, "waiting...");

    addTopLevelItem(item);

    connect(newStateMonitorTracker, &StateMonitorTracker::stateMonitorTrackerStateChanged, this,
            &MainStateMonitorWidget::updateStateMonitorTrackerState);
    connect(newStateMonitorTracker, &StateMonitorTracker::stateMonitorTrackerNameChanged, this,
            &MainStateMonitorWidget::updateStateMonitorTrackerName);

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

int MainStateMonitorWidget::getRow(StateMonitorTracker *caller) {
    int row = 0;
    for (auto const *stateMonitorWidget: stateMonitorTrackers) {
        if (caller == stateMonitorWidget) {
            return row;
        } else {
            row++;
        }
    }
}

void MainStateMonitorWidget::updateStateMonitorTrackerName(QString name) {
    int row = getRow((StateMonitorTracker *) sender());

    QTreeWidgetItem *item = topLevelItem(row);

    if (item) {
        item->setText(1, name);
    }
}

void MainStateMonitorWidget::updateStateMonitorTrackerState(QString state) {
    int row = getRow((StateMonitorTracker *) sender());

    QTreeWidgetItem *item = topLevelItem(row);

    if (item) {
        item->setText(2, state);
    }

}