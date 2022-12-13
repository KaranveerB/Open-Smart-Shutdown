#include "main_window.h"

MainWindow::MainWindow() : QMainWindow() {
    testButton = new QPushButton("Start monitoring");
    setCentralWidget(testButton);
    setWindowTitle("Open Smart Shutdown");
    resize(500, 500);
    testStateMonitorManager();
    QObject::connect(&smman, SIGNAL(stateChanged()), this, SLOT(refreshStateStatuses()));
}