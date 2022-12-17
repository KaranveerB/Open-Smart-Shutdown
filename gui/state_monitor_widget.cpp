#include "state_monitor_widget.h"

StateMonitorWidget::StateMonitorWidget(unsigned int id, QString name, const StateMonitorManager &stateMonitorManager, QWidget *parent)
        : id{id},
          QWidget(parent) {

    auto *qhBoxLayout = new QHBoxLayout(this);
    this->setLayout(qhBoxLayout);

    // TODO: Remove temp code
    nameLabel = new QLabel(this);
    nameLabel->setText(name);
    stateLabel = new QLabel(this);
    updateState(*stateMonitorManager.getState(id));

    qhBoxLayout->addWidget(nameLabel);
    qhBoxLayout->addWidget(stateLabel);

    // TODO: Why does Clion give error for connect?
    QObject::connect(&stateMonitorManager, &StateMonitorManager::stateChanged, this, &StateMonitorWidget::updateStateMonitorWidget);
}

void StateMonitorWidget::updateState(State &state) {
    bool stateBool = state.getState();
    QString stateQString = stateBool ? "Active"
                                     : "Inactive"; // TODO: Switch to have colors as well (keep text too for our friends with protanopia out there)
    stateLabel->setText(stateQString);
}

unsigned int StateMonitorWidget::getId() {
    return id;
}

void StateMonitorWidget::updateStateMonitorWidget(unsigned int id, State *state) {
    if (this->id == id) {
        updateState(*state);
    }
}