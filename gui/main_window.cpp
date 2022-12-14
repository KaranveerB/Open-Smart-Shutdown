#include <QListWidget>
#include "main_window.h"

MainWindow::MainWindow() : QMainWindow() {
    auto *mainWidget = new QWidget(this);
    auto *mainLayout = new QHBoxLayout(mainWidget);

    stateListWidget = new QListWidget(mainWidget);

    setCentralWidget(mainWidget);
    setWindowTitle("Open Smart Shutdown");
    resize(500, 500);
    testStateMonitorManager();
    refreshStateList();
    QObject::connect(&stateMonitorManager, SIGNAL(stateChanged()), this, SLOT(refreshStateList()));
}