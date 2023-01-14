#pragma once

#include <chrono>

#include <QComboBox>
#include <QDialog>
#include <QTimeEdit>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include "state_monitor/state_monitor.h"
#include "state_monitor/state_evaluator/state_evaluator_master.h"
#include "state_monitor/state_reader/state_reader_master.h"


class StateMonitorCreatorWidget : public QDialog {
Q_OBJECT
public:
    struct StateMonitorMetaInfo {
        QString name;
        QString typeName;
    };

    explicit StateMonitorCreatorWidget(QWidget *parent = nullptr);

    IStateMonitor *getStateMonitor() const;

    StateMonitorMetaInfo getStateMonitorMetaInfo() const;

private slots:

    void updateReaderDataInputWidget(StateReaderType stateReaderType);

    void updateEvaluatorDataInputWidget(StateEvaluatorType stateEvaluatorType);

private:

    template<class T>
    StateReader<T> *createStateReader() const;

    template<class T>
    StateEvaluator<T> *createStateEvaluator() const;

	static QTime truncateQTimeToMinutes(QTime time);

    StateReaderType currentStateReaderType;
    StateEvaluatorType currentStateEvaluatorType;

    QWidget *mainWidget;
    QVBoxLayout *mainWidgetLayout;
    QLineEdit *nameLineEdit;
    QLineEdit *readerDataInput = nullptr;
    QHBoxLayout *evaluatorDataInputLayout;
    QWidget *evaluatorDataInputWidget = nullptr;
    QWidget *evaluatorDataInput1 = nullptr;
    QWidget *evaluatorDataInput2 = nullptr;
};
