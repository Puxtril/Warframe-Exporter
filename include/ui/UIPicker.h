#pragma once

#include "ui/ui_Picker.h"
#include "ui/UIExporter.h"
#include "Extractor.h"

#include <qdialog.h>
#include <qmainwindow.h>
#include <QMessageBox>
#include <qtmetamacros.h>

class UiPicker : public QObject, private Ui_WindowPicker
{
    Q_OBJECT

public:
    UiPicker(QObject *parent = nullptr);
    virtual ~UiPicker() {}

    void setupUi(QDialog *WindowPicker);
    void connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter);

signals:
    void pickerDone(std::filesystem::path cachePath, WarframeExporter::ExtractorType extractTypes);

public slots:
    void sendDataToExporter();
};