#include "main_state_monitor_widget.h"

// TODO: Add edit button

MainStateMonitorWidget::MainStateMonitorWidget(QWidget *parent) : QTreeWidget(parent) {
    QStringList headerLabels;
    headerLabels << tr("Type") << tr("Name") << tr("Value") << tr("State") << tr("");

    setHeaderLabels(headerLabels);

    auto fm = fontMetrics();
    QHeaderView *header = this->header();

    header->setMinimumSectionSize(24);
    header->resizeSection(0, fm.horizontalAdvance("Shell     ")); // clips too often so extended in size
    header->setSectionResizeMode(1, QHeaderView::Stretch);
    header->setSectionResizeMode(2, QHeaderView::Stretch);
    header->resizeSection(2, fm.horizontalAdvance("99:99:99  "));
    header->resizeSection(3, fm.horizontalAdvance("Buffer (99)  "));
    header->resizeSection(4, 24);
    header->setSectionResizeMode(4, QHeaderView::Fixed);
    header->setStretchLastSection(false);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setAlternatingRowColors(true);
    setRootIsDecorated(false);

    setItemDelegate(new StateMonitorWidgetDelegate);

    QObject::connect(&stateMonitorManager, &StateMonitorManager::timeTillEventTriggerChanged, this,
                     &MainStateMonitorWidget::updateTimeTillEventTrigger);
    QObject::connect(this, &QTreeWidget::itemClicked, this, &MainStateMonitorWidget::handleItemClicked);
}

void
MainStateMonitorWidget::addStateMonitor(IStateMonitor *sm, StateMonitorCreatorWidget::StateMonitorMetaInfo metaInfo) {
    unsigned int id = stateMonitorManager.addStateMonitor(sm);
    auto *newStateMonitorTracker = new StateMonitorTracker(id, stateMonitorManager, this);

    auto *item = new QTreeWidgetItem; // TODO: Customize this

    item->setText(0, metaInfo.typeName);
    item->setText(1, metaInfo.name);
    item->setText(3, "Waiting");

    addTopLevelItem(item);
    stateMonitorTrackerMap[indexOfTopLevelItem(item)] = newStateMonitorTracker;

    connect(newStateMonitorTracker, &StateMonitorTracker::stateMonitorTrackerStateChanged, this,
            &MainStateMonitorWidget::updateStateMonitorTrackerState);
    connect(newStateMonitorTracker, &StateMonitorTracker::stateMonitorTrackerNameChanged, this,
            &MainStateMonitorWidget::updateStateMonitorTrackerName);
    connect(newStateMonitorTracker, &StateMonitorTracker::stateMonitorTrackerStateValueChanged, this,
            &MainStateMonitorWidget::updateStateMonitorTrackerStateValue);
    connect(newStateMonitorTracker, &StateMonitorTracker::scheduleStateMonitorDeletion, &stateMonitorManager,
            &StateMonitorManager::deleteStateMonitor);
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

void MainStateMonitorWidget::configure() {
    ConfigureWidget configureWidget(stateMonitorManager.getConfig());
    configureWidget.exec();
    if (configureWidget.result() == QDialog::Accepted) {
        stateMonitorManager.setConfig(configureWidget.getData());
    }
}

bool MainStateMonitorWidget::toggleStart() {
    return stateMonitorManager.toggleStart();
}

int MainStateMonitorWidget::getRow(StateMonitorTracker *caller) {
    for (const auto &[key, value]: stateMonitorTrackerMap) {
        if (value == caller) {
            return key;
        }
    }
    return -1;
}

void MainStateMonitorWidget::handleItemClicked(QTreeWidgetItem *item, int column) {
    if (column == 4) { // delete button
        int row = indexOfTopLevelItem(item);
        takeTopLevelItem(row);
        auto *stateMonitorTracker = stateMonitorTrackerMap.at(row);

        stateMonitorTracker->deleteStateMonitor();
        stateMonitorTrackerMap.erase(row);
        for (auto it = stateMonitorTrackerMap.begin(); it != stateMonitorTrackerMap.end(); it++) {
            if (it->first > row) {
                auto node = stateMonitorTrackerMap.extract(it->first);
                node.key() = it->first - 1;
                stateMonitorTrackerMap.insert(std::move(node));
            }
        }
        delete item;
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
        item->setText(3, state);
    }

}

void MainStateMonitorWidget::updateStateMonitorTrackerStateValue(QString stateValue) {
    int row = getRow((StateMonitorTracker *) sender());

    QTreeWidgetItem *item = topLevelItem(row);

    if (item) {
        item->setText(2, stateValue);
    }
}

void MainStateMonitorWidget::updateTimeTillEventTrigger(QTime timeRemaining) {
    emit timeTillEventTriggerUpdated(timeRemaining);
}