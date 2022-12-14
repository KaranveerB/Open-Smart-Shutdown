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
    readerTypeSelector->addItem("Date/Time", QVariant(StateReaderType::Time));
    readerTypeSelector->addItem("Network usage", QVariant(StateReaderType::Network));
    readerTypeSelector->addItem("Disk usage", QVariant(StateReaderType::Disk));
    readerTypeSelector->addItem("CPU usage", QVariant(StateReaderType::Cpu));
    readerTypeSelector->addItem("GPU usage", QVariant(StateReaderType::Gpu));
    readerTypeSelector->addItem("Shell command", QVariant(StateReaderType::Shell));
    mainWidgetLayout->addWidget(readerTypeSelector);

    QObject::connect(readerTypeSelector, &QComboBox::currentIndexChanged,
                     [this, readerTypeSelector]() {
                         auto type = readerTypeSelector->currentData().value<StateReaderType>();
                         updateReaderDataInputWidget(type);
                     });


    readerDataInput = new QLineEdit("State reader data input", mainWidget);
    mainWidgetLayout->addWidget(readerDataInput);

    updateReaderDataInputWidget(readerTypeSelector->currentData().value<StateReaderType>());

    auto *evaluatorTypeSelector = new QComboBox(mainWidget);
    evaluatorTypeSelector->addItem("Between", QVariant(StateEvaluatorType::InRange));
    evaluatorTypeSelector->addItem("Exactly", QVariant(StateEvaluatorType::Boolean));
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

    evaluatorDataInput1 = new QDateTimeEdit(evaluatorDataInputWidget);
    evaluatorDataInput2 = new QDateTimeEdit(evaluatorDataInputWidget);

    ((QDateTimeEdit *) evaluatorDataInput1)->setMinimumDateTime(QDateTime::currentDateTime());
    ((QDateTimeEdit *) evaluatorDataInput2)->setMinimumDateTime(QDateTime::currentDateTime());

    evaluatorDataInputLayout->addWidget(evaluatorDataInput1);
    evaluatorDataInputLayout->addWidget(evaluatorDataInput2);

    updateEvaluatorDataInputWidget(evaluatorTypeSelector->currentData().value<StateEvaluatorType>());

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

IStateMonitor *StateMonitorCreatorWidget::getStateMonitor() const {
    switch (currentStateReaderType) {
        case Time: {
            // FIXME: time state monitor is off by ~30 seconds
            typedef time_point<system_clock> T;
            auto *stateReader = createStateReader<T>();
            auto *stateEvaluator = createStateEvaluator<T>();
            return new StateMonitor<T>(stateReader, stateEvaluator);
        }
        case Cpu:
            break;
        case Gpu:
            break;
        case Disk:
            break;
        case Network:
            break;
        case Shell:
            break;
    }
}

template<class T>
StateReader<T> *StateMonitorCreatorWidget::createStateReader() const {
    switch (currentStateReaderType) {
        case Time:
            return new TimeStateReader();
        case Cpu:
            break;
        case Gpu:
            break;
        case Disk:
            break;
        case Network:
            break;
        case Shell:
            break;
    }
    throw std::logic_error("can't create state reader of invalid type");
}


template<class T>
StateEvaluator<T> *StateMonitorCreatorWidget::createStateEvaluator() const {
    T data1;
    T data2;

    if (currentStateReaderType == StateReaderType::Time) {
        auto *data1Input = (QDateTimeEdit *) evaluatorDataInput1;
        auto *data2Input = (QDateTimeEdit *) evaluatorDataInput2;
        data1 = convertQDateTimeToTimePoint(data1Input->dateTime());

        if (currentStateEvaluatorType == StateEvaluatorType::InRange) {
            data2 = convertQDateTimeToTimePoint(data2Input->dateTime());
        }
    }

    switch (currentStateEvaluatorType) {
        case InRange:
            return new InRangeStateEvaluator<T>(data1, data2);
        case Greater:
            return new GreaterThanStateEvaluator<T>(data1);
        case Less:
            break;
        case GreaterOrEqual:
            break;
        case LessOrEqual:
            break;
        case Boolean:
            break;
    }
    throw std::logic_error("can't create state evaluator of invalid type");
}

time_point<system_clock> StateMonitorCreatorWidget::convertQDateTimeToTimePoint(QDateTime qdt) {
    return time_point<system_clock>(std::chrono::milliseconds(qdt.toMSecsSinceEpoch()));
}

QString StateMonitorCreatorWidget::getStateMonitorName() const {
    return nameLineEdit->text();
}

void StateMonitorCreatorWidget::updateReaderDataInputWidget(StateReaderType stateReaderType) {
    readerDataInput->setEnabled(stateReaderType == StateReaderType::Shell); // currently, only relevant for shell

    currentStateReaderType = stateReaderType;

    // TODO: Also show current reading of state monitor

}

void StateMonitorCreatorWidget::updateEvaluatorDataInputWidget(StateEvaluatorType stateEvaluatorType) {
    currentStateEvaluatorType = stateEvaluatorType;
    // TODO
}


