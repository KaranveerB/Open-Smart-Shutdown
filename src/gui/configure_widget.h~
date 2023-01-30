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

#include "events/event_triggers.h"

class ConfigureWidget : public QDialog {
Q_OBJECT
public:
    struct Configuration {
        EventTriggers::Action triggerAction;
        QTime activationDelay;
        QString shellCommand;
    };

    ConfigureWidget( Configuration config, QWidget *parent = nullptr);
    Configuration getData();

private:
    QTimeEdit *activationDelayEdit;
    QComboBox *triggerActionComboBox;
    QTextEdit *shellTextEdit;
};
