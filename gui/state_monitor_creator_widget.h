#pragma once

#include <QComboBox>
#include <QDialog>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTimeEdit>

#include "state_monitor.h"
#include "state_evaluator/state_evaluator_master.h"
#include "state_reader/state_reader_master.h"


class StateMonitorCreatorWidget : public QDialog {
Q_OBJECT
public:
    explicit StateMonitorCreatorWidget(QWidget *parent = nullptr);
    IStateMonitor *getStateMonitor() const;
    QString getStateMonitorName() const;
private:
    QLineEdit *nameLineEdit;
    QWidget *evaluatorDataInput;
    QWidget *readerDataInput;

    IStateMonitor *stateMonitor;
};
