#include "ui/UIMainWindow.h"
#include <QShortcut>
#include <QApplication>

UiMainWindow::UiMainWindow()
    : QMainWindow()
{
    QShortcut* quitShortcut = new QShortcut(QKeySequence("Ctrl+Q"), this);
    connect(quitShortcut, &QShortcut::activated, this, &QWidget::close);

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

    settings.saveMainWindowData(QMainWindow::size());
}

void
UiMainWindow::loadGeometry()
{
    UiSettings& settings = UiSettings::getInstance();
    
    resize(settings.getMainWindowSize());
}