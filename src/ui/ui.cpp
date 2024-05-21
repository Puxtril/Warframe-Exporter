#include "ui/ui.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette palette = app.palette();
    palette.setColor(QPalette::Highlight, QColor(99, 114, 250));
    app.setPalette(palette);
	
	QIcon appIcon(":/AppIcon.png");
    
    UiMainWindow mainWindow;
	mainWindow.setWindowIcon(appIcon);
    UiExporter window;
    window.setup(&mainWindow);
    mainWindow.loadGeometry();
    
    QDialog dialog;
	dialog.setWindowIcon(appIcon);
    UiPicker picker;
    picker.setupUi(&dialog);
    picker.connect(&dialog, &mainWindow, &window);
    dialog.show();
    
    return app.exec();
}