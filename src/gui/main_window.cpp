#include "main_window.h"

MainWindow::MainWindow() : QMainWindow() {
    setWindowTitle("Open Smart Shutdown");
    resize(500, 500);

    auto *mainWidget = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);

    auto *mainStateMonitorWidget = new MainStateMonitorWidget;
    mainLayout->addWidget(mainStateMonitorWidget);

    auto *createLayoutButton = new QPushButton("Add state monitor", this);
    mainLayout->addWidget(createLayoutButton);

    QObject::connect(createLayoutButton, &QPushButton::pressed, mainStateMonitorWidget,
                     &MainStateMonitorWidget::createNewStateMonitor);

    setCentralWidget(mainWidget);
}