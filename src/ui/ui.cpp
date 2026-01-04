#include "ui/ui.h"

int main(int argc, char** argv)
{
    // I encountered this bug when packaging as an appimage.
    // QT dynamically attempts to load GStreamer instead of ffmpeg.
    // It was more difficult to replace this in the appimage itself,
    //   and this program only supports ffmpeg, anyway.
    char forceFfmpegEnv[] = "QT_MEDIA_BACKEND=ffmpeg";
    putenv(forceFfmpegEnv);

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
    picker.loadSettings();
    dialog.show();
    
    return app.exec();
}