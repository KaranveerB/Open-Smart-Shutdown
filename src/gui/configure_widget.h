#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QTime>

#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QTimeEdit>
#include <QPushButton>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "events/event_triggers.h"
#include "state_monitor/state_monitor_manager.h"

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
