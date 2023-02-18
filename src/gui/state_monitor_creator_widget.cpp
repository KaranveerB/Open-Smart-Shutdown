#include "state_monitor_creator_widget.h"

/*
 * -----------------------------
 * | [state monitor name]      |
 * | Buffer Sizes:             |
 * | [buffer size           ]  | <-- general config area
 * | Polling Rate (ms):        |
 * | [polling rate]            |
 * |---------------------------|
 * | [sel reader type]         | <-- StateReader config area
 * | [data for reader       ]  |
 * | ------------------------- |
 * | [sel eval type]           | <-- StateEvaluator config area
 * | [data for eval         ]  |
 * | ------------------------- |
 * |        [confirm] [cancel] | <-- state monitor creator button area
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
    setMinimumSize(350, 350); // TODO: Don't manually set this

    mainLayout = new QVBoxLayout(this);

    // general config area
    nameLineEdit = new QLineEdit("State monitor name", this);
    mainLayout->addWidget(nameLineEdit);

    auto *bufferSizeLabel = new QLabel("Buffer Size:");

    bufferSizeSpinBox = new QSpinBox;
    bufferSizeSpinBox->setMinimum(0);
    bufferSizeSpinBox->setMaximum(INT_MAX);
    bufferSizeSpinBox->setSingleStep(1);
    bufferSizeSpinBox->setValue(3);

    mainLayout->addWidget(bufferSizeLabel);
    mainLayout->addWidget(bufferSizeSpinBox);

    auto *pollingIntervalLabel = new QLabel("Polling Rate (ms):");

    pollingIntervalSpinBox = new QSpinBox;
    pollingIntervalSpinBox->setMinimum(0);
    pollingIntervalSpinBox->setMaximum(INT_MAX);
    pollingIntervalSpinBox->setSingleStep(100);
    pollingIntervalSpinBox->setValue(1000);

    mainLayout->addWidget(pollingIntervalLabel);
    mainLayout->addWidget(pollingIntervalSpinBox);

    // divider
    insertDivider();

    // StateReader config area
    auto *readerTypeSelector = new QComboBox(this);
    readerTypeSelector->addItem("Time", QVariant(StateReaderType::Time));
    readerTypeSelector->addItem("Network usage", QVariant(StateReaderType::Network));
    readerTypeSelector->addItem("Disk usage", QVariant(StateReaderType::Disk));
    readerTypeSelector->addItem("CPU usage", QVariant(StateReaderType::Cpu));
    readerTypeSelector->addItem("Shell command (String output)", QVariant(StateReaderType::StrShell));
    readerTypeSelector->addItem("Shell command (Int/Float output)", QVariant(StateReaderType::FloatShell));
    mainLayout->addWidget(readerTypeSelector);

    QObject::connect(readerTypeSelector, &QComboBox::currentIndexChanged,
                     [this, readerTypeSelector]() {
                         auto type = readerTypeSelector->currentData().value<StateReaderType>();
                         updateDataInputs(type);
                     });


    readerDataInput = new QLineEdit("State reader data input", this);
    mainLayout->addWidget(readerDataInput);

    // divider
    insertDivider();

    // StateEvaluator config area
    auto *evaluatorTypeSelector = new QComboBox(this);
    evaluatorTypeSelector->addItem("Between", QVariant(StateEvaluatorType::InRange));
    evaluatorTypeSelector->addItem("Equal", QVariant(StateEvaluatorType::Equal));
    evaluatorTypeSelector->addItem("Greater than", QVariant(StateEvaluatorType::Greater));
    evaluatorTypeSelector->addItem("Greater than or equal", QVariant(StateEvaluatorType::GreaterOrEqual));
    evaluatorTypeSelector->addItem("Less than", QVariant(StateEvaluatorType::Less));
    evaluatorTypeSelector->addItem("Less than or equal", QVariant(StateEvaluatorType::LessOrEqual));

    mainLayout->addWidget(evaluatorTypeSelector);

    QObject::connect(evaluatorTypeSelector, &QComboBox::currentIndexChanged,
                     [this, evaluatorTypeSelector]() {
                         auto type = evaluatorTypeSelector->currentData().value<StateEvaluatorType>();
                         updateDataInputs(type);
                     });

    evaluatorDataInputLayout = new QHBoxLayout();
    mainLayout->addLayout(evaluatorDataInputLayout);

    evaluatorDataInput1 = new QTimeEdit(this);
    evaluatorDataInput2 = new QTimeEdit(this);

    ((QTimeEdit *) evaluatorDataInput1)->setTime(truncateQTimeToMinutes(QTime::currentTime()));
    ((QTimeEdit *) evaluatorDataInput2)->setTime(truncateQTimeToMinutes(QTime::currentTime()));

    evaluatorDataInputLayout->addWidget(evaluatorDataInput1);
    evaluatorDataInputLayout->addWidget(evaluatorDataInput2);

    currentStateEvaluatorType = evaluatorTypeSelector->currentData().value<StateEvaluatorType>();
    updateDataInputs(readerTypeSelector->currentData().value<StateReaderType>());


    // state monitor (sm) creator button area
    // no divider because this area only contains buttons
    auto *smCreatorButtonLayout = new QHBoxLayout();
    smCreatorButtonLayout->setAlignment(Qt::AlignRight);
    mainLayout->addLayout(smCreatorButtonLayout);

    auto *addButton = new QPushButton("Add state monitor", this);
    QObject::connect(addButton, &QPushButton::pressed, this, &QDialog::accept);
    auto *cancelButton = new QPushButton("Cancel", this);
    QObject::connect(cancelButton, &QPushButton::pressed, this, &QDialog::reject);

    smCreatorButtonLayout->addWidget(addButton);
    smCreatorButtonLayout->addWidget(cancelButton);
}


IStateMonitor *StateMonitorCreatorWidget::getStateMonitor() const {
    auto pollingInterval = std::chrono::milliseconds(pollingIntervalSpinBox->value());
    switch (currentStateReaderType) {
        case Time: {
            typedef QTime T;
            auto *stateReader = new TimeStateReader();
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator, pollingInterval);
        }
        case Cpu: {
            typedef float T;
            auto *stateReader = new CpuStateReader();
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator, pollingInterval);
        }
        case Disk: {
            typedef float T;
            auto *stateReader = new DiskStateReader("_Total"); // TODO: Allow selecting disk
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator, pollingInterval);
        }
        case Network: {
            typedef float T;
            auto *stateReader = new NetStateReader();
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator, pollingInterval);
        }
        case StrShell: {
            typedef std::string T;
            std::string readerData = readerDataInput->text().toStdString();
            auto *stateReader = new StringShellStateReader(readerData);
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator, pollingInterval);
        }
        case FloatShell: {
            typedef float T;
            std::string readerData = readerDataInput->text().toStdString();
            auto *stateReader = new FloatShellStateReader(readerData);
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator, pollingInterval);
        }
    }

    throw std::runtime_error("could not create state monitor");
}

StateMonitorCreatorWidget::StateMonitorMetaInfo StateMonitorCreatorWidget::getStateMonitorMetaInfo() const {
    StateMonitorMetaInfo metaInfo;
    metaInfo.name = nameLineEdit->text();
    metaInfo.bufferSize = (unsigned int) bufferSizeSpinBox->value();
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

void StateMonitorCreatorWidget::updateDataInputs(StateReaderType newStateReaderType) {
    // reader data input is currently only used for shell
    readerDataInput->setEnabled(newStateReaderType == StateReaderType::FloatShell ||
                                newStateReaderType == StateReaderType::StrShell);
    // TODO: Also use reader data input for disk selection

    currentStateReaderType = newStateReaderType;

    delete evaluatorDataInput1;
    delete evaluatorDataInput2;

    switch (newStateReaderType) {
        case Time: {
            evaluatorDataInput1 = new QTimeEdit(this);
            evaluatorDataInput2 = new QTimeEdit(this);

            ((QTimeEdit *) evaluatorDataInput1)->setTime(truncateQTimeToMinutes(QTime::currentTime()));
            ((QTimeEdit *) evaluatorDataInput2)->setTime(truncateQTimeToMinutes(QTime::currentTime()));
        }
            break;
        case FloatShell:
        case Cpu:
        case Disk:
        case Network: {
            evaluatorDataInput1 = new QDoubleSpinBox(this);
            evaluatorDataInput2 = new QDoubleSpinBox(this);
        }
            break;
        case StrShell: {
            // TODO: Set it so str shell only works with exactly as otherwise, it doesn't really make sense
            evaluatorDataInput1 = new QLineEdit(this);
            evaluatorDataInput2 = new QLineEdit(this);
        }

    }

    evaluatorDataInputLayout->addWidget(evaluatorDataInput1);
    evaluatorDataInputLayout->addWidget(evaluatorDataInput2);

    updateDataInputs(currentStateEvaluatorType);

    // TODO: Also show current reading of state monitor
}

void StateMonitorCreatorWidget::updateDataInputs(StateEvaluatorType newStateEvaluatorType) {
    currentStateEvaluatorType = newStateEvaluatorType;
    switch (newStateEvaluatorType) {
        case InRange:
            evaluatorDataInput1->setEnabled(true);
            evaluatorDataInput2->setEnabled(true);
            break;
        default:
            evaluatorDataInput1->setEnabled(true);
            evaluatorDataInput2->setEnabled(false);

    }
}

void StateMonitorCreatorWidget::insertDivider() {
    auto *divider = new QFrame();
    divider->setFrameShape(QFrame::HLine);
    divider->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(divider);
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
    data2 = (float) data2Input->value();

    return createStateEvaluator<float>(data1, data2);

}

template<>
StateEvaluator<std::string> *StateMonitorCreatorWidget::createStateEvaluator<std::string>() const {
    std::string data1;
    std::string data2;

    auto *data1Input = (QLineEdit *) evaluatorDataInput1;
    auto *data2Input = (QLineEdit *) evaluatorDataInput2;
    data1 = data1Input->text().toStdString();
    data2 = data2Input->text().toStdString();

    return createStateEvaluator<std::string>(data1, data2);

}

template<>
StateEvaluator<QTime> *StateMonitorCreatorWidget::createStateEvaluator<QTime>() const {
    QTime data1;
    QTime data2;

    auto *data1Input = (QTimeEdit *) evaluatorDataInput1;
    auto *data2Input = (QTimeEdit *) evaluatorDataInput2;
    data1 = data1Input->time();
    data2 = data2Input->time();

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
    throw std::runtime_error("could not create state evaluator");
}


QTime StateMonitorCreatorWidget::truncateQTimeToMinutes(QTime time) {
    time = time.addSecs(-time.second());
    time = time.addMSecs(-time.msec());
    return time;
}