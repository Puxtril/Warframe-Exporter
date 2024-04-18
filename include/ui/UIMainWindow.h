#pragma once

#include "Settings.h"

#include <QtWidgets/qmainwindow.h>

class UiMainWindow : public QMainWindow
{
    Q_OBJECT

    void closeEvent(QCloseEvent *event) override;
    
    void saveGeometry();
    void loadGeometry();

public:
    UiMainWindow();

signals:
    void mainWindowClose();
};