#include "main_state_monitor_widget.h"

MainStateMonitorWidget::MainStateMonitorWidget(QWidget *parent) : QTreeWidget(parent) {
    QStringList headerLabels;
    headerLabels << tr("Type") << tr("Name") << tr("State") << tr("") << tr("");

    setHeaderLabels(headerLabels);

    auto fm = fontMetrics();
    QHeaderView *header = this->header();

    header->setMinimumSectionSize(24);
    header->resizeSection(0, fm.horizontalAdvance("Shell  "));
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    header->resizeSection(2, fm.horizontalAdvance("Inactive  "));
    header->resizeSection(3, 24);
    header->setSectionResizeMode(3, QHeaderView::Fixed);
    header->resizeSection(4, 24);
    header->setSectionResizeMode(4, QHeaderView::Fixed);
    header->setStretchLastSection(false);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);

    setItemDelegate(new StateMonitorWidgetDelegate);


    stateMonitorManager.startMonitor();
}

void MainStateMonitorWidget::addStateMonitor(IStateMonitor *sm, StateMonitorCreatorWidget::StateMonitorMetaInfo metaInfo) {
    unsigned int id = stateMonitorManager.addStateMonitor(sm);
    auto *newStateMonitorTracker = new StateMonitorTracker(id, stateMonitorManager, this);
    stateMonitorTrackers.append(newStateMonitorTracker);

    auto *item = new QTreeWidgetItem; // TODO: Customize this

    item->setText(0, metaInfo.typeName);
    item->setText(1, metaInfo.name);
    item->setText(2, "Waiting");

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
        auto metaInfo = smCreatorWidget.getStateMonitorMetaInfo();
        MainStateMonitorWidget::addStateMonitor(stateMonitor, metaInfo);
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