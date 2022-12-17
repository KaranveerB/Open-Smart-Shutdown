#include "main_window.h"

MainWindow::MainWindow() : QMainWindow() {
    auto *mainWidget = new MainStateMonitorWidget;
    setCentralWidget(mainWidget);

    setWindowTitle("Open Smart Shutdown");
    resize(500, 500);
}