#pragma once

#include "Settings.h"

#include <QtWidgets/qmainwindow.h>

class UiMainWindow : public QMainWindow
{
    Q_OBJECT

    void closeEvent(QCloseEvent *event) override;
    
    void saveGeometry();

public:
    UiMainWindow();
    void loadGeometry();

signals:
    void mainWindowClose();
};