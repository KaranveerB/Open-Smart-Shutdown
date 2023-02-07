#include "main_state_monitor_widget.h"

// TODO: Add edit button

MainStateMonitorWidget::MainStateMonitorWidget(QWidget *parent) : QTreeWidget(parent) {
    // | type | name            | value | state | delBtn |
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
    header->resizeSection(3, fm.horizontalAdvance("Buffer (999)  "));
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

void MainStateMonitorWidget::addStateMonitor(IStateMonitor *sm,
                                             const StateMonitorCreatorWidget::StateMonitorMetaInfo &metaInfo) {
    unsigned int id = stateMonitorManager.addStateMonitor(sm, metaInfo.bufferSize);
    auto *newStateMonitorTracker = new StateMonitorTracker(id, stateMonitorManager, this);

    auto *item = new QTreeWidgetItem; // TODO: Customize this

    item->setText(0, metaInfo.typeName);
    item->setText(1, metaInfo.name);
    item->setText(3, "Waiting");

    addTopLevelItem(item);
    stateMonitorTrackers.emplace_back(newStateMonitorTracker);

    connect(newStateMonitorTracker, &StateMonitorTracker::stateMonitorTrackerStateChanged, this,
            &MainStateMonitorWidget::updateStateMonitorTrackerState);
    connect(newStateMonitorTracker, &StateMonitorTracker::stateMonitorTrackerNameChanged, this,
            &MainStateMonitorWidget::updateStateMonitorTrackerName);
    connect(newStateMonitorTracker, &StateMonitorTracker::stateMonitorTrackerStateValueChanged, this,
            &MainStateMonitorWidget::updateStateMonitorTrackerStateValue);
    connect(newStateMonitorTracker, &StateMonitorTracker::scheduleStateMonitorDeletion, &stateMonitorManager,
            &StateMonitorManager::deleteStateMonitor);
}

bool MainStateMonitorWidget::toggleStart() {
    return stateMonitorManager.toggleStart();
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


void MainStateMonitorWidget::updateStateMonitorTrackerStateValue(const QString &stateValue) {
    int row = getRow((StateMonitorTracker *) sender());

    QTreeWidgetItem *item = topLevelItem(row);

    if (item) {
        item->setText(2, stateValue);
    }
}

void MainStateMonitorWidget::updateTimeTillEventTrigger(QTime timeRemaining) {
    emit timeTillEventTriggerUpdated(timeRemaining);
}

void MainStateMonitorWidget::handleItemClicked(QTreeWidgetItem *item, int column) {
    if (column == 4) { // delete button
        int row = indexOfTopLevelItem(item);
        takeTopLevelItem(row);
        auto *stateMonitorTracker = stateMonitorTrackers.at(row);
        stateMonitorTracker->deleteStateMonitorTracker();
        stateMonitorTrackers.erase(stateMonitorTrackers.begin() + row);
        delete item;
    }
}

int MainStateMonitorWidget::getRow(StateMonitorTracker *caller) {
    for (unsigned int i = 0; i < stateMonitorTrackers.size(); i++) {
        if (stateMonitorTrackers.at(i) == caller) {
            return (int) i;
        }
    }

    throw std::runtime_error("invalid caller for getRow");
}