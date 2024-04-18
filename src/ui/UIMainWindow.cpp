#include "ui/UIMainWindow.h"

UiMainWindow::UiMainWindow()
    : QMainWindow()
{
    loadGeometry();
}

void
UiMainWindow::closeEvent(QCloseEvent *event)
{
    UiMainWindow::saveGeometry();
    
    emit mainWindowClose();

    QMainWindow::closeEvent(event);
}
    
void
UiMainWindow::saveGeometry()
{
    UiSettings& settings = UiSettings::getInstance();

    settings.saveMainWindowData(QMainWindow::saveGeometry(), QMainWindow::saveState());
}

void
UiMainWindow::loadGeometry()
{
    UiSettings& settings = UiSettings::getInstance();
    
    restoreGeometry(settings.getMainWindowGeometry());
    restoreState(settings.getMainWindowState());
}