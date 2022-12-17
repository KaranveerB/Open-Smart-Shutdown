#include "state_monitor_widget.h"

StateMonitorWidget::StateMonitorWidget(unsigned int id, QString name, State &state, QWidget *parent) : id{id},
                                                                                                       QWidget(parent) {
    auto *qhBoxLayout = new QHBoxLayout(this);
    this->setLayout(qhBoxLayout);

    // TODO: Remove temp code
    nameLabel = new QLabel(this);
    nameLabel->setText(name);
    stateLabel = new QLabel(this);
    updateState(state);

    qhBoxLayout->addWidget(nameLabel);
    qhBoxLayout->addWidget(stateLabel);
}

void StateMonitorWidget::updateState(State &state) {
    bool stateBool = state.getState();
    QString stateQString = stateBool ? "Active" : "Inactive"; // TODO: Switch to have colors as well (keep text too for our friends with protanopia out there)
    stateLabel->setText(stateQString);
}

unsigned int StateMonitorWidget::getId() {
    return id;
}
