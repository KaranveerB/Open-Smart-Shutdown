#include "main_window.h"

MainWindow::MainWindow() : QMainWindow() {
    setWindowTitle("Open Smart Shutdown");
    resize(600, 600);

    // set up main (central) widget
    auto *mainWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(mainWidget);
    setCentralWidget(mainWidget);

    mainStateMonitorWidget = new MainStateMonitorWidget;
    mainLayout->addWidget(mainStateMonitorWidget);

    auto *configureOptionsLayout = new QHBoxLayout(mainWidget);
    mainLayout->addLayout(configureOptionsLayout);

    auto *addStateMonitorButton = new QPushButton("Add state monitor", this);
    auto *configureButton = new QPushButton("Configure", this);
    configureOptionsLayout->addWidget(addStateMonitorButton);
    configureOptionsLayout->addWidget(configureButton);

    toggleActiveButton = new QPushButton("Start", this);
    mainLayout->addWidget(toggleActiveButton);


    QObject::connect(addStateMonitorButton, &QPushButton::pressed, mainStateMonitorWidget,
                     &MainStateMonitorWidget::createNewStateMonitor);
    QObject::connect(configureButton, &QPushButton::pressed, mainStateMonitorWidget,
                     &MainStateMonitorWidget::configure);
    QObject::connect(toggleActiveButton, &QPushButton::pressed, this,
                     &MainWindow::toggleStart);

    // connect signals to update the countdown timer on the start button
    QObject::connect(mainStateMonitorWidget, &MainStateMonitorWidget::timeTillEventTriggerUpdated,
                     this, &MainWindow::updateTimeTillEventTrigger);

    // other operations (such as notification and shell) will still work even if shutdown privileges aren't obtained
    if (!EventTriggers::acquireShutdownPrivilege()) {
        QMessageBox::critical(this, "Error",
                              "Could not acquire shutdown privilege. Shutdown, hibernate, and sleep may not work. "
                              "Running as admin may help.");
    }
}

void MainWindow::updateTimeTillEventTrigger(QTime timeRemaining) {
    QString str = "Running (" + timeRemaining.toString("mm:ss") + ")";
    toggleActiveButton->setText(str);
}

void MainWindow::toggleStart() {
    if (!mainStateMonitorWidget->toggleStart()) {
        toggleActiveButton->setText("Start");
    }
}