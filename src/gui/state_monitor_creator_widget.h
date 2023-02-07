#pragma once

#include <chrono>

#include <QDialog>

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTimeEdit>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "state_monitor/state_monitor.h"
#include "state_monitor/state_evaluator/state_evaluator_master.h"
#include "state_monitor/state_reader/state_reader_master.h"


class StateMonitorCreatorWidget : public QDialog {
Q_OBJECT
public:
    struct StateMonitorMetaInfo {
        QString name;
        QString typeName;
        unsigned int bufferSize;
    };

    explicit StateMonitorCreatorWidget(QWidget *parent = nullptr);

    IStateMonitor *getStateMonitor() const;

    StateMonitorMetaInfo getStateMonitorMetaInfo() const;

private slots:

    void updateDataInputs(StateReaderType newStateReaderType);

    void updateDataInputs(StateEvaluatorType newStateEvaluatorType);

private:
    void insertDivider();

    template<class T>
    StateEvaluator<T> *createStateEvaluator() const;

    template<>
    StateEvaluator<float> *createStateEvaluator() const;

    template<>
    StateEvaluator<std::string> *createStateEvaluator() const;

    template<>
    StateEvaluator<QTime> *createStateEvaluator() const;

    template<class T>
    StateEvaluator<T> *createStateEvaluator(T min, T max) const;

    // Helpful when restricting QTimeEdit to minutes and not avoiding invisible seconds and millisecond values.
    static QTime truncateQTimeToMinutes(QTime time);

    StateReaderType currentStateReaderType;
    StateEvaluatorType currentStateEvaluatorType;

    QVBoxLayout *mainLayout;
    QLineEdit *nameLineEdit;
    QLineEdit *readerDataInput;
    QSpinBox *bufferSizeSpinBox;
    QSpinBox *pollingIntervalSpinBox;
    QHBoxLayout *evaluatorDataInputLayout;
    QWidget *evaluatorDataInput1;
    QWidget *evaluatorDataInput2;
};
