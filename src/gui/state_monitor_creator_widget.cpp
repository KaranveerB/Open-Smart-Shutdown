#include "state_monitor_creator_widget.h"

/*
 * -----------------------------
 * | [state monitor name]      |
 * |---------------------------|
 * | [sel reader type]         |
 * | [data for reader       ]  |
 * | ------------------------- |
 * | [sel eval type]           |
 * | [data for eval         ]  |
 * | ------------------------- |
 * |        [confirm] [cancel] |
 * -----------------------------
 */

/*
 * TODO:
 * - clean up code!!!
 * - think of better way for state reader input
 *      - prob only for shell so separate-up button?
 * - have QComboBox's organized alphabetically
 * - have SM creation in separate class in core (easier to adapt to CLI after)
 */

using std::chrono::time_point;
using std::chrono::system_clock;

StateMonitorCreatorWidget::StateMonitorCreatorWidget(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Create new state monitor");

    mainWidget = new QWidget(this);
    mainWidgetLayout = new QVBoxLayout(mainWidget);

    nameLineEdit = new QLineEdit("State monitor name", mainWidget);
    mainWidgetLayout->addWidget(nameLineEdit);

    auto *readerTypeSelector = new QComboBox(mainWidget);
    readerTypeSelector->addItem("Time", QVariant(StateReaderType::Time));
    readerTypeSelector->addItem("Network usage", QVariant(StateReaderType::Network));
    readerTypeSelector->addItem("Disk usage", QVariant(StateReaderType::Disk));
    readerTypeSelector->addItem("CPU usage", QVariant(StateReaderType::Cpu));
    readerTypeSelector->addItem("Shell command (String output)", QVariant(StateReaderType::StrShell));
    readerTypeSelector->addItem("Shell command (Int/Float output)", QVariant(StateReaderType::FloatShell));
    mainWidgetLayout->addWidget(readerTypeSelector);

    QObject::connect(readerTypeSelector, &QComboBox::currentIndexChanged,
                     [this, readerTypeSelector]() {
                         auto type = readerTypeSelector->currentData().value<StateReaderType>();
                         updateReaderDataInputWidget(type);
                     });


    readerDataInput = new QLineEdit("State reader data input", mainWidget);
    mainWidgetLayout->addWidget(readerDataInput);

    auto *evaluatorTypeSelector = new QComboBox(mainWidget);
    evaluatorTypeSelector->addItem("Between", QVariant(StateEvaluatorType::InRange));
    evaluatorTypeSelector->addItem("Equal", QVariant(StateEvaluatorType::Equal));
    evaluatorTypeSelector->addItem("Greater than", QVariant(StateEvaluatorType::Greater));
    evaluatorTypeSelector->addItem("Greater than or equal", QVariant(StateEvaluatorType::GreaterOrEqual));
    evaluatorTypeSelector->addItem("Less than", QVariant(StateEvaluatorType::Less));
    evaluatorTypeSelector->addItem("Less than or equal", QVariant(StateEvaluatorType::LessOrEqual));

    mainWidgetLayout->addWidget(evaluatorTypeSelector);

    QObject::connect(evaluatorTypeSelector, &QComboBox::currentIndexChanged,
                     [this, evaluatorTypeSelector]() {
                         auto type = evaluatorTypeSelector->currentData().value<StateEvaluatorType>();
                         updateEvaluatorDataInputWidget(type);
                     });

    evaluatorDataInputWidget = new QWidget(mainWidget);
    evaluatorDataInputLayout = new QHBoxLayout(evaluatorDataInputWidget);
    evaluatorDataInputWidget->setLayout(evaluatorDataInputLayout);
    mainWidgetLayout->addWidget(evaluatorDataInputWidget);

    evaluatorDataInput1 = new QTimeEdit(evaluatorDataInputWidget);
    evaluatorDataInput2 = new QTimeEdit(evaluatorDataInputWidget);

    ((QTimeEdit *) evaluatorDataInput1)->setTime(truncateQTimeToMinutes(QTime::currentTime()));
    ((QTimeEdit *) evaluatorDataInput2)->setTime(truncateQTimeToMinutes(QTime::currentTime()));

    evaluatorDataInputLayout->addWidget(evaluatorDataInput1);
    evaluatorDataInputLayout->addWidget(evaluatorDataInput2);

    currentStateEvaluatorType = evaluatorTypeSelector->currentData().value<StateEvaluatorType>();
    updateReaderDataInputWidget(readerTypeSelector->currentData().value<StateReaderType>());

    auto *completeButtonsWidget = new QWidget(mainWidget);
    auto *completeButtonLayout = new QHBoxLayout(completeButtonsWidget);
    completeButtonLayout->setAlignment(Qt::AlignRight);
    mainWidgetLayout->addWidget(completeButtonsWidget);

    auto *addButton = new QPushButton("Add state monitor", completeButtonsWidget);
    QObject::connect(addButton, &QPushButton::pressed, this, &QDialog::accept);
    auto *cancelButton = new QPushButton("Cancel", completeButtonsWidget);
    QObject::connect(cancelButton, &QPushButton::pressed, this, &QDialog::reject);

    completeButtonLayout->addWidget(addButton);
    completeButtonLayout->addWidget(cancelButton);

    // TODO: Don't manually set this
    setMinimumSize(350, 300);
}

template<class T>
StateEvaluator<T> *StateMonitorCreatorWidget::createStateEvaluator() const {
    throw std::runtime_error("unknown type for creating evaluator");
}

template<>
StateEvaluator<float> *StateMonitorCreatorWidget::createStateEvaluator<float>() const {
    float data1;
    float data2;

    auto *data1Input = (QDoubleSpinBox *) evaluatorDataInput1;
    auto *data2Input = (QDoubleSpinBox *) evaluatorDataInput2;
    data1 = (float) data1Input->value();

    if (currentStateEvaluatorType == StateEvaluatorType::InRange) {
        data2 = (float) data2Input->value();
    }

    return createStateEvaluator<float>(data1, data2);

}

template<>
StateEvaluator<std::string> *StateMonitorCreatorWidget::createStateEvaluator<std::string>() const {
    std::string data1;
    std::string data2;

    auto *data1Input = (QLineEdit *) evaluatorDataInput1;
    auto *data2Input = (QLineEdit *) evaluatorDataInput2;
    data1 = data1Input->text().toStdString();

    if (currentStateEvaluatorType == StateEvaluatorType::InRange) {
        data2 = data2Input->text().toStdString();
    }

    return createStateEvaluator<std::string>(data1, data2);

}

template<>
StateEvaluator<QTime> *StateMonitorCreatorWidget::createStateEvaluator<QTime>() const {
    QTime data1;
    QTime data2;

    auto *data1Input = (QTimeEdit *) evaluatorDataInput1;
    auto *data2Input = (QTimeEdit *) evaluatorDataInput2;
    data1 = data1Input->time();

    if (currentStateEvaluatorType == StateEvaluatorType::InRange) {
        data2 = data2Input->time();
    }

    return createStateEvaluator<QTime>(data1, data2);

}

template<class T>
StateEvaluator<T> *StateMonitorCreatorWidget::createStateEvaluator(T data1, T data2) const {
    switch (currentStateEvaluatorType) {
        case InRange:
            return new InRangeStateEvaluator<T>(data1, data2);
        case Greater:
            return new GreaterThanStateEvaluator<T>(data1);
        case Less:
            return new LessThanStateEvaluator<T>(data1);
        case GreaterOrEqual:
            return new GreaterThanEqualStateEvaluator<T>(data1);
        case LessOrEqual:
            return new LessThanEqualStateEvaluator<T>(data1);
        case Equal:
            return new EqualStateEvaluator<T>(data1);
    }
}


IStateMonitor *StateMonitorCreatorWidget::getStateMonitor() const {
    switch (currentStateReaderType) {
        case Time: {
            typedef QTime T;
            auto *stateReader = new TimeStateReader();
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator);
        }
        case Cpu: {
            typedef float T;
            auto *stateReader = new CpuStateReader();
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator);
        }
        case Disk: {
            typedef float T;
            auto *stateReader = new DiskStateReader("_Total"); // TODO: Allow selecting disk
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator);
        }
        case Network: {
            typedef float T;
            auto *stateReader = new NetStateReader();
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator);
        }
        case StrShell: {
            typedef std::string T;
            std::string readerData = readerDataInput->text().toStdString();
            auto *stateReader = new StringShellStateReader(readerData);
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator);
        }
        case FloatShell: {
            typedef float T;
            std::string readerData = readerDataInput->text().toStdString();
            auto *stateReader = new FloatShellStateReader(readerData);
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator);
        }
    }
}


QTime StateMonitorCreatorWidget::truncateQTimeToMinutes(QTime time) {
    time = time.addSecs(-time.second());
    time = time.addMSecs(-time.msec());
    return time;
}

StateMonitorCreatorWidget::StateMonitorMetaInfo StateMonitorCreatorWidget::getStateMonitorMetaInfo() const {
    StateMonitorMetaInfo metaInfo;
    metaInfo.name = nameLineEdit->text();
    // TODO: Use map (or better solution) and move this somewhere else
    switch (currentStateReaderType) {
        case Time:
            metaInfo.typeName = "Time";
            break;
        case Cpu:
            metaInfo.typeName = "CPU";
            break;
        case Disk:
            metaInfo.typeName = "Disk";
            break;
        case Network:
            metaInfo.typeName = "Net";
            break;
        case FloatShell:
            metaInfo.typeName = "Shell (Int/Float output)";
            break;
        case StrShell:
            metaInfo.typeName = "Shell (String output)";
            break;
    }
    return metaInfo;
}

void StateMonitorCreatorWidget::updateReaderDataInputWidget(StateReaderType stateReaderType) {
    readerDataInput->setEnabled(stateReaderType == StateReaderType::FloatShell ||
                                stateReaderType == StateReaderType::StrShell); // currently, only relevant for shell

    currentStateReaderType = stateReaderType;

    delete evaluatorDataInput1;
    delete evaluatorDataInput2;

    switch (stateReaderType) {
        case Time: {
            evaluatorDataInput1 = new QTimeEdit(evaluatorDataInputWidget);
            evaluatorDataInput2 = new QTimeEdit(evaluatorDataInputWidget);

            ((QTimeEdit *) evaluatorDataInput1)->setTime(truncateQTimeToMinutes(QTime::currentTime()));
            ((QTimeEdit *) evaluatorDataInput2)->setTime(truncateQTimeToMinutes(QTime::currentTime()));
        }
            break;
        case FloatShell:
        case Cpu:
        case Disk:
        case Network: {
            evaluatorDataInput1 = new QDoubleSpinBox(evaluatorDataInputWidget);
            evaluatorDataInput2 = new QDoubleSpinBox(evaluatorDataInputWidget);
        }
            break;
        case StrShell: {
            // TODO: Set it so str shell only works with exactly as otherwise, it doesn't really make sense
            evaluatorDataInput1 = new QLineEdit(evaluatorDataInputWidget);
            evaluatorDataInput2 = new QLineEdit(evaluatorDataInputWidget);
        }

    }

    evaluatorDataInputLayout->addWidget(evaluatorDataInput1);
    evaluatorDataInputLayout->addWidget(evaluatorDataInput2);

    updateEvaluatorDataInputWidget(currentStateEvaluatorType);

    // TODO: Also show current reading of state monitor
}

void StateMonitorCreatorWidget::updateEvaluatorDataInputWidget(StateEvaluatorType stateEvaluatorType) {
    currentStateEvaluatorType = stateEvaluatorType;
    switch (stateEvaluatorType) {
        case InRange:
            evaluatorDataInput1->setEnabled(true);
            evaluatorDataInput2->setEnabled(true);
            break;
        default:
            evaluatorDataInput1->setEnabled(true);
            evaluatorDataInput2->setEnabled(false);

    }
}