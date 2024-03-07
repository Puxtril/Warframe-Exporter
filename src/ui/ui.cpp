#include "ui/ui.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    QMainWindow mainWindow;
    UiExporter window;
    window.setup(&mainWindow);
    
    QDialog dialog;
    UiPicker picker;
    picker.setupUi(&dialog);
    picker.connect(&dialog, &mainWindow, &window);
    dialog.show();
    
    return app.exec();
}