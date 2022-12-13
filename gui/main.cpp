#include <QApplication>

#include "main_window.h"

using namespace std::chrono;

int main(int argc, char **argv) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();


}