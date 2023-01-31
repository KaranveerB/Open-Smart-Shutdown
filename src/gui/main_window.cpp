#include "main_window.h"

MainWindow::MainWindow() : QMainWindow() {
    setWindowTitle("Open Smart Shutdown");
    resize(600, 600);

    auto *mainWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(mainWidget);

    mainStateMonitorWidget = new MainStateMonitorWidget;
    mainLayout->addWidget(mainStateMonitorWidget);

    QObject::connect(mainStateMonitorWidget, &MainStateMonitorWidget::timeTillEventTriggerUpdated,
                     this, &MainWindow::updateTimeTillEventTrigger);

    auto *configureOptionsWidget = new QWidget(this);
    auto *configureOptionsLayout = new QHBoxLayout(configureOptionsWidget);
    mainLayout->addWidget(configureOptionsWidget);

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

    setCentralWidget(mainWidget);

    if (!EventTriggers::acquireShutdownPrivilege()) {
        QMessageBox::critical(this, "Error",
                              "Could not acquire shutdown privilege. Shutdown, hibernate, and sleep may not work. "
                              "Running as admin may help.");
    }
}

void MainWindow::updateTimeTillEventTrigger(QTime timeRemaining) {
    QString str = "Running (";
    str += timeRemaining.toString("mm:ss");
    str += ")";
    toggleActiveButton->setText(str);
}

void MainWindow::toggleStart() {
    if (!mainStateMonitorWidget->toggleStart()) {
        toggleActiveButton->setText("Start");
    }
}