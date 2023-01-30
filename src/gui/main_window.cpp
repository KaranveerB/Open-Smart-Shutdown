#include "main_window.h"

MainWindow::MainWindow() : QMainWindow() {
    setWindowTitle("Open Smart Shutdown");
    resize(600, 600);

    auto *mainWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(mainWidget);

    auto *mainStateMonitorWidget = new MainStateMonitorWidget;
    mainLayout->addWidget(mainStateMonitorWidget);

    auto *configureOptionsWidget = new QWidget(this);
    auto *configureOptionsLayout = new QHBoxLayout(configureOptionsWidget);
    mainLayout->addWidget(configureOptionsWidget);

    auto *addStateMonitorButton = new QPushButton("Add state monitor", this);
    auto *configureButton = new QPushButton("Configure", this);
    configureOptionsLayout->addWidget(addStateMonitorButton);
    configureOptionsLayout->addWidget(configureButton);

    auto *toggleActiveButton = new QPushButton("Start", this);
    mainLayout->addWidget(toggleActiveButton);


    QObject::connect(addStateMonitorButton, &QPushButton::pressed, mainStateMonitorWidget,
                     &MainStateMonitorWidget::createNewStateMonitor);
    QObject::connect(configureButton, &QPushButton::pressed, mainStateMonitorWidget,
                     &MainStateMonitorWidget::configure);
    QObject::connect(toggleActiveButton, &QPushButton::pressed, mainStateMonitorWidget,
                     &MainStateMonitorWidget::toggleStart);

    setCentralWidget(mainWidget);
}