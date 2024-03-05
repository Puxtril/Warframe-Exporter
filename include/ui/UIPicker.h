#pragma once

#include "ui/ui_Picker.h"
#include "ui/UIExporter.h"
#include "Extractor.h"
#include "ui/Settings.h"
#include "Meta.h"

#include <qdialog.h>
#include <qmainwindow.h>
#include <QMessageBox>
#include <qtmetamacros.h>
#include <qfiledialog.h>

class UiPicker : public QObject, private Ui_WindowPicker
{
    Q_OBJECT

public:
    UiPicker(QObject *parent = nullptr);
    virtual ~UiPicker() {}

    void setupUi(QDialog *WindowPicker);
    void connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter);

private:
    void loadSettings();
    void loadVersion();

signals:
    void pickerDone(std::filesystem::path cachePath, std::filesystem::path exportPath, WarframeExporter::ExtractorType viewTypes, WarframeExporter::ExtractorType extractTypes);

public slots:
    void parsePickerOptions();
    void browseCacheWindows();
    void browseExportPath();
};