#pragma once

#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QTextEdit>
#include <QTime>
#include <QTimeEdit>
#include <QVBoxLayout>
#include <QPushButton>

#include "state_monitor/state_monitor_manager.h"
#include "events/event_triggers.h"

class ConfigureWidget : public QDialog {
Q_OBJECT
public:

    ConfigureWidget(StateMonitorManager::Configuration config, QWidget *parent = nullptr);

    StateMonitorManager::Configuration getData();

private:
    QTimeEdit *activationDelayEdit;
    QComboBox *triggerActionComboBox;
    QTextEdit *shellTextEdit;
};
