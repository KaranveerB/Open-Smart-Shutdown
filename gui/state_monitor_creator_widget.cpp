#include "state_monitor_creator_widget.h"

/*
 * -----------------------------
 * | [sel reader type]         |
 * | [data for reader       ]  |
 * | ------------------------- |
 * | [sel eval type]           |
 * | [data for type         ]  |
 * | ------------------------- |
 * |        [confirm] [cancel] |
 * -----------------------------
 */

StateMonitorCreatorWidget::StateMonitorCreatorWidget(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Create new state monitor");

    auto *mainWidget = new QWidget(this);
    auto *mainWidgetLayout = new QVBoxLayout(mainWidget);

    nameLineEdit = new QLineEdit("State monitor name", mainWidget);
    mainWidgetLayout->addWidget(nameLineEdit);

    auto *evaluatorTypeSelector = new QComboBox(mainWidget);
    evaluatorTypeSelector->addItem("Date/Time");
    mainWidgetLayout->addWidget(evaluatorTypeSelector);

    // TODO: Don't manually set this
    setMinimumSize(300, 300);
}

IStateMonitor *StateMonitorCreatorWidget::getStateMonitor() const {
    return stateMonitor;
}

QString StateMonitorCreatorWidget::getStateMonitorName() const {
    return nameLineEdit->text();
}
