#include "configure_widget.h"

ConfigureWidget::ConfigureWidget(StateMonitorManager::Configuration config, QWidget *parent) : QDialog(parent) {
    setWindowTitle("Configure");

    auto *mainLayout = new QVBoxLayout(this);

    auto *activationDelayLayout = new QHBoxLayout();
    mainLayout->addLayout(activationDelayLayout);

    auto *activationDelayLabel = new QLabel("Activation delay: ", this);

    activationDelayEdit = new QTimeEdit(this);
    activationDelayEdit->setDisplayFormat("mm:ss");
    activationDelayEdit->setMinimumTime(QTime(0, 0, 0));
    activationDelayEdit->setTime(config.activationDelay);

    activationDelayLayout->addWidget(activationDelayLabel);
    activationDelayLayout->addWidget(activationDelayEdit);

    auto *triggerActionLayout = new QHBoxLayout();
    mainLayout->addLayout(triggerActionLayout);

    auto *triggerActionLabel = new QLabel("Trigger action: ", this);
    triggerActionLayout->addWidget(triggerActionLabel);

    triggerActionComboBox = new QComboBox(this);
    triggerActionComboBox->addItem("Hibernate", QVariant::fromValue(EventTriggers::Action::Hibernate));
    triggerActionComboBox->addItem("Notify", QVariant::fromValue(EventTriggers::Action::Notify));
    triggerActionComboBox->addItem("Sleep", QVariant::fromValue(EventTriggers::Action::Sleep));
    triggerActionComboBox->addItem("Shutdown", QVariant::fromValue(EventTriggers::Action::Shutdown));
    triggerActionComboBox->addItem("Shell", QVariant::fromValue(EventTriggers::Action::Shell));

    triggerActionLayout->addWidget(triggerActionComboBox);

    shellTextEdit = new QTextEdit(this);
    shellTextEdit->setPlaceholderText("Shell command");
    shellTextEdit->setEnabled(false);
    shellTextEdit->setText(QLatin1String(config.shellCommand)); // do this regardless of if shell is selected
    mainLayout->addWidget(shellTextEdit);

    QObject::connect(triggerActionComboBox, &QComboBox::currentIndexChanged, [=](const QVariant &data) {
        auto triggerAction = data.value<EventTriggers::Action>();
        if (triggerAction == EventTriggers::Action::Shell) {
            shellTextEdit->setEnabled(true);
        } else {
            shellTextEdit->setEnabled(false);
        }
    });

    triggerActionComboBox->setCurrentIndex(
          triggerActionComboBox->findData(QVariant::fromValue(config.triggerAction))
    );

    auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

StateMonitorManager::Configuration ConfigureWidget::getData() {
    StateMonitorManager::Configuration config;
    config.activationDelay = activationDelayEdit->time();
    config.triggerAction = triggerActionComboBox->currentData().value<EventTriggers::Action>();
    config.shellCommand = shellTextEdit->toPlainText().toStdString();
    return config;
}
