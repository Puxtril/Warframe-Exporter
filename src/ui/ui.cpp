#include "ui/ui.h"
#include <qapplication.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    Ui_MainWindow window;
    QMainWindow mainWindow;
    window.setupUi(&mainWindow);
    mainWindow.show();
    return app.exec();
}